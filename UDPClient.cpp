/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#include <iostream>
#include <string>

#include "UDPClient.hpp"

using std::string;
using namespace boost;

UDPClient::UDPClient(asio::io_service& io_service, const asio::ip::udp::endpoint& targetEndpoint)
: socket{io_service}
, targetEndpoint{targetEndpoint}
, dataInput{io_service, ::dup(STDIN_FILENO)}
, isReading{false}
, isAlive{true}
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
		std::cerr << "Error in handleClientStarted\n";
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
		input >> lastData >> lastAck >> lastWin;

		auto pos = std::find(data, data + length, '\n');
		if (pos == data + length)
			std::cerr << "Received bad DATA.";
		else
			std::cout << string{pos + 1, data + length};

		if (lastAck == lastId && lastWin > 0 && !isReading)
			readInput();
	} else if (command == "ACK") {
		input >> lastAck >> lastWin;

		if (lastAck == lastId && lastWin > 0 && !isReading)
			readInput();
	} else {
		std::cerr << "Bad message: " << command << '\n';
	}

	startReceive();
}

void UDPClient::readInput() {
	if (lastWin == 0)
		return;

	isReading = true;
	boost::asio::async_read(dataInput,
				boost::asio::buffer(inputBuffer, std::min(inputBuffer.size(), (size_t)lastWin)),
				boost::bind(&UDPClient::handleEndReadInput, this,
					    boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void UDPClient::handleEndReadInput(const system::error_code& error, size_t size) {
	if (!error) {
		string data(inputBuffer.begin(), inputBuffer.begin() + size);
		string head = "UPLOAD " + std::to_string(lastId++) + "\n";

		socket.send_to(boost::asio::buffer(head + data), targetEndpoint);
		lastId++;

		isReading = false;
	}
}

void UDPClient::sendKeepalive() {
	socket.send_to(boost::asio::buffer("KEEPALIVE\n"), targetEndpoint);

	keepaliveTimer.expires_from_now(boost::posix_time::milliseconds(100));
	keepaliveTimer.async_wait(boost::bind(&UDPClient::sendKeepalive, this));
}

void UDPClient::checkConnection() {
	if (!isAlive)
		throw std::exception(); //FIXME: problematic connection

	isAlive = false;

	connectionTimer.expires_from_now(boost::posix_time::seconds(1));
	connectionTimer.async_wait(boost::bind(&UDPClient::checkConnection, this));
}
