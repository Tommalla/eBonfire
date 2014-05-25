/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#include "UDPServer.hpp"
#include "logger.hpp"

using std::istringstream;
using std::string;
using std::to_string;
using namespace boost::asio::ip;

UDPServer::UDPServer(boost::asio::io_service& io_service, const Port& port, ConnectionsController* connectionsController)
: connectionsController{connectionsController}
, socket{io_service, udp::endpoint(udp::v6(), port)} {
	startReceive();
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
				uint16_t nr;
				input >> nr;
				auto client = connectionsController->getClients().at(clientId->second);
				if (client->ack != nr)
					logger::warn << "Received an unwanted UPLOAD from " << clientId->second << "\nExpected: " <<
						client->ack << " Got: " << nr << "\n";
				else {
					auto pos = std::find(data, data + size, '\n');
					if (pos == data + size)
						logger::warn << "Bad UPLOAD\n";
					else
						client->queue.addData(pos + 1, size);

					client->ack = nr + 1;

					string response = "ACK " + to_string(client->ack) + " " + to_string(client->queue.getFreeSpace()) + "\n";
					socket.send_to(boost::asio::buffer(response), remoteEndpoint);
				}
			} else if (command == "RETRANSMIT") {
			//TODO
			} else if (command == "KEEPALIVE") {
			} else
				logger::warn << "Bad message: " << command << "\n";
		}

	}
	startReceive();
}
