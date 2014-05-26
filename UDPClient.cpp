/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#include <iostream>
#include <string>

#include "logger.hpp"
#include "UDPClient.hpp"

using std::string;
using std::to_string;
using namespace boost;

class ProblematicConnectionException : public std::exception {
	const char* what() const noexcept { return "The connection is problematic. Retrying..."; }
};

UDPClient::UDPClient(asio::io_service& io_service, const asio::ip::udp::endpoint& targetEndpoint, const size_t& retransmitLimit)
: socket{io_service}
, targetEndpoint{targetEndpoint}
, dataInput{io_service, ::dup(STDIN_FILENO)}
, isReading{false}
, isAlive{true}
, dataReceived{false}
, retransmitLimit{retransmitLimit}
, lastData{0}
, lastId{0}
, lastAck{0}
, lastWin{0}
, nrMaxSeen{0}
, dataQty{0}
, keepaliveTimer{io_service, boost::posix_time::milliseconds(100)}
, connectionTimer{io_service, boost::posix_time::seconds(1)} {
	socket.open(asio::ip::udp::v6());
}

void UDPClient::initUDP(const ClientId& clientId) {
	keepaliveTimer.async_wait(boost::bind(&UDPClient::sendKeepalive, this));
	connectionTimer.async_wait(boost::bind(&UDPClient::checkConnection, this));

	startReceive();

	boost::shared_ptr<std::string> msg(new string("CLIENT " + std::to_string(clientId) + "\n"));

	socket.async_send_to(boost::asio::buffer(*msg), targetEndpoint,
			      boost::bind(&UDPClient::handleClientStarted, this,
					  boost::asio::placeholders::error));
}

void UDPClient::handleClientStarted(const system::error_code& error) {
	if (!error)
		startReceive();
	else
		logger::error << "Error in handleClientStarted\n";
}

void UDPClient::startReceive() {
	socket.async_receive_from(
		boost::asio::buffer(receiveBuffer), remoteEndpoint,
				   boost::bind(&UDPClient::handleReceive, this,
					       boost::asio::placeholders::error,
		   boost::asio::placeholders::bytes_transferred));
}

void UDPClient::handleReceive(const system::error_code& error, std::size_t length) {
	const char* data = receiveBuffer.data();
	std::istringstream input(data);
	string command;

	input >> command;
	isAlive = true;

	if (command == "DATA") {
		size_t nr, newAck, newWin;
		input >> nr >> newAck >> newWin;

		if (newAck >= lastAck) {
			lastAck = newAck;
			lastWin = newWin;
		} else
			logger::warn << "Received an ACK smaller than the known: " << newAck << "\n";

		if (nr > nrMaxSeen)
			nrMaxSeen = nr;

		if (lastData + 1 < nr && lastData + 1 >= nr - retransmitLimit && dataReceived && nr == nrMaxSeen) {
			logger::warn << "Haven't received datagram " << lastData + 1 << " asking to RETRANSMIT\n";
			string msg = "RETRANSMIT " + to_string(lastData + 1) + "\n";
			socket.send_to(boost::asio::buffer(msg), remoteEndpoint);
		} else if (lastData + 1 <= nr) {
			lastData = nr;
			dataReceived = true;

			auto pos = std::find(data, data + length, '\n');
			if (pos == data + length)
				logger::warn << "Received bad DATA.";
			else
				std::cout << string{pos + 1, data + length};
		}

		if (lastAck == lastId && lastWin > 0 && !isReading)
			readInput();
		else if (lastAck < lastId && ++dataQty >= 2) {
			logger::warn << "Resending data to server\n";
			socket.send_to(boost::asio::buffer(lastInput), targetEndpoint);
			dataQty = 0;
		}
	} else if (command == "ACK") {
		size_t newAck, newWin;
		input >> newAck >> newWin;

		if (newAck >= lastAck) {
			lastAck = newAck;
			lastWin = newWin;
		} else
			logger::warn << "Received an ACK smaller than the known: " << newAck << "\n";

		if (lastAck == lastId && lastWin > 0 && !isReading)
			readInput();
	} else {
		logger::warn << "Bad message: " << command << '\n';
	}

	startReceive();
}

void UDPClient::readInput() {
	if (lastWin == 0)
		return;

	isReading = true;
	logger::info << "Reading from file " << lastWin << "bytes\n";
	boost::asio::async_read(dataInput,
				boost::asio::buffer(inputBuffer, std::min(inputBuffer.size(), (size_t)lastWin)),
				boost::bind(&UDPClient::handleEndReadInput, this,
					    boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void UDPClient::handleEndReadInput(const system::error_code& error, size_t size) {
	if (!error) {
		string data(inputBuffer.begin(), inputBuffer.begin() + size);
		string head = "UPLOAD " + std::to_string(lastId) + "\n";
		logger::info << "UPLOADING...\n";

		socket.send_to(boost::asio::buffer(head + data), targetEndpoint);
		lastInput = head + data;

		dataQty = 0;
		lastId++;
		isReading = false;
	} else
		logger::warn << "Error on reading input: " << error.message() << "\n";
}

void UDPClient::sendKeepalive() {
	socket.send_to(boost::asio::buffer("KEEPALIVE\n"), targetEndpoint);

	keepaliveTimer.expires_from_now(boost::posix_time::milliseconds(100));
	keepaliveTimer.async_wait(boost::bind(&UDPClient::sendKeepalive, this));
}

void UDPClient::checkConnection() {
	if (!isAlive)
		throw ProblematicConnectionException();

	isAlive = false;

	connectionTimer.expires_from_now(boost::posix_time::seconds(1));
	connectionTimer.async_wait(boost::bind(&UDPClient::checkConnection, this));
}
