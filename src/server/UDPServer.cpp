/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#include "../logger.hpp"
#include "UDPServer.hpp"

using std::istringstream;
using std::string;
using std::to_string;
using std::shared_ptr;
using namespace boost::asio::ip;

UDPServer::UDPServer(boost::asio::io_service& io_service, const Port& port, const size_t& bufferLength, ConnectionsController* connectionsController)
: connectionsController{connectionsController}
, socket{io_service, udp::endpoint(udp::v6(), port)}
, bufferLength{bufferLength}
, nextDataId{0}
, isSending{false} {
	startReceive();
}

void UDPServer::handleDataProduced(string data) {
	auto& clients = connectionsController->getClients();
	for (auto client: clients)
		if (client.second->isUDPReady) {
			sendData(data, nextDataId, client.second);
		}

	serverBuffer.push_back({nextDataId, data});
	while (serverBuffer.size() > bufferLength)
		serverBuffer.pop_front();
	nextDataId++;
}

void UDPServer::startReceive() {
	socket.async_receive_from(boost::asio::buffer(messageBuffer), remoteEndpoint,
				   boost::bind(&UDPServer::handleReceive, this,
					       boost::asio::placeholders::error,
		   boost::asio::placeholders::bytes_transferred));
}

void UDPServer::handleReceive(const boost::system::error_code& error, std::size_t size) {
	if (!error) {
		auto data = messageBuffer.data();
		istringstream input(string(data, data + size));
		string command;
		typename ConnectionsController::IdContainer::const_iterator clientId;

		input >> command;

		if (command == "CLIENT") {
			ClientId id;
			input >> id;
			connectionsController->registerUDPClient(id, remoteEndpoint);
		} else if ((clientId = connectionsController->getIds().find(remoteEndpoint)) != connectionsController->getIds().end()) {	//client is connected
			if (command == "UPLOAD") {
				size_t nr;
				input >> nr;
				auto client = connectionsController->getClients().at(clientId->second);

				if (client->ack != nr)
					logger::warn << "Received an unwanted UPLOAD from " << clientId->second << "\nExpected: " <<
						client->ack << " Got: " << nr << "\n";
				else {
					auto pos = std::find(data, data + size, '\n');
					if (pos == data + size)
						logger::warn << "Bad UPLOAD datagram\n";
					else {
						if (size - (pos - data + 1) <= client->queue.getFreeSpace()) {
							client->queue.addData(pos + 1, size - (pos - data + 1));

							client->ack = nr + 1;

							string response = "ACK " + to_string(client->ack) + " " + to_string(client->queue.getFreeSpace()) + "\n";
							addToSend(response, remoteEndpoint);
						} else
							logger::warn << "Too much data from " << clientId->second << ". Dumping datagram...\n";
					}
				}
			} else if (command == "RETRANSMIT") {
				size_t begin;
				input >> begin;

				retransmit(begin, remoteEndpoint);
			} else if (command == "KEEPALIVE") {
				auto client = connectionsController->getClients().at(clientId->second);
				client->inactiveFor = 0;
			} else
				logger::warn << "Bad message: " << command << "\n";
		}

	}
	startReceive();
}

void UDPServer::retransmit(const size_t& begin, const udp::endpoint& endpoint) {
	auto client = connectionsController->getClients().at(connectionsController->getIds().at(endpoint));
	for (const auto& p: serverBuffer)
		if (p.first >= begin)
			sendData(p.second, p.first, client);
}

void UDPServer::sendData(const string& data, const size_t& id, const shared_ptr< ClientInfo >& client) {
	string header = "DATA " + to_string(id) + " " + to_string(client->ack) + " "
		+ to_string(client->queue.getFreeSpace()) + "\n";
	addToSend(header + data, client->udpEndpoint);
}

void UDPServer::addToSend(const string& msg, const udp::endpoint& endpoint) {
	messagesQueue.push({endpoint, msg});
	if (!isSending)
		sendMessage();
}

void UDPServer::sendMessage() {
	if (isSending)
		return;

	isSending = true;

	auto p = messagesQueue.front();
	messagesQueue.pop();
	socket.send_to(boost::asio::buffer(p.second), p.first);

	if (!messagesQueue.empty())
		sendMessage();
	else
		isSending = false;
}
