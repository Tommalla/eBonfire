/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#include <list>

#include <boost/lexical_cast.hpp>

#include "TCPDiagnosticServer.hpp"
#include "logger.hpp"

using std::list;
using std::shared_ptr;
using std::cerr;
using std::string;;
using namespace boost::asio;

TCPDiagnosticServer::TCPDiagnosticServer(boost::asio::io_service& io_service, const uint16_t& port, ConnectionsController* connectionsController)
: connectionsController{connectionsController}
, io_service(io_service)
, acceptor{io_service, ip::tcp::endpoint(ip::tcp::v6(), port)}
, timer{io_service, boost::posix_time::seconds(1)} {
	startAccepting();
	timer.async_wait(boost::bind(&TCPDiagnosticServer::sendStats, this));
}

void TCPDiagnosticServer::startAccepting() {
	shared_ptr<ip::tcp::socket> socket = shared_ptr<ip::tcp::socket>{new ip::tcp::socket{io_service}};

	acceptor.async_accept(*socket,
			       boost::bind(&TCPDiagnosticServer::handleAccept, this, socket,
					   boost::asio::placeholders::error));
}

void TCPDiagnosticServer::handleAccept(shared_ptr<ip::tcp::socket> socket, const boost::system::error_code& error) {
	if (!error) {
		ClientId id = connectionsController->registerTCPClient(socket);
		auto message = "CLIENT " + std::to_string(id) + "\n";
		boost::asio::write(*socket, boost::asio::buffer(message));
	}

	startAccepting();
}

void TCPDiagnosticServer::sendStats() {
	string message = "\n";

	auto& clients = connectionsController->getClients();

	for (auto iter = begin(clients); iter != end(clients);) {
		boost::system::error_code error;
		string part = boost::lexical_cast<string>(iter->second->tcpSocket->remote_endpoint(error))
		+ " TODO - stats\n";

		if (error) {
			auto copy = iter;
			++copy;
			clients.erase(iter);
			iter = copy;
		} else {
			++iter;
			message += part;
		}

	}

	logger::info << "Sending to all clients: " << message;

	for (const auto &k: clients)
		boost::asio::async_write(*k.second->tcpSocket, boost::asio::buffer(message), [](const boost::system::error_code&, size_t){});

	timer.expires_from_now(boost::posix_time::seconds(1));
	timer.async_wait(boost::bind(&TCPDiagnosticServer::sendStats, this));
}