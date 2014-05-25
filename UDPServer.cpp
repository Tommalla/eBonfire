/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#include "UDPServer.hpp"
#include "logger.hpp"

using std::istringstream;
using std::string;
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


		input >> command;

		if (command == "CLIENT") {
			ClientId id;
			input >> id;
			connectionsController->registerUDPClient(id, remoteEndpoint);
		} else if (command == "UPLOAD") {
			//TODO
		} else if (command == "RETRANSMIT") {
			//TODO
		} else if (command == "KEEPALIVE") {
		} else
			logger::warn << "Bad message: " << command << "\n";

	}
	startReceive();
}
