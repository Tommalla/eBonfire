/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#include "TCPDiagnosticServer.hpp"
#include "logger.hpp"

using std::shared_ptr;
using std::cerr;
using namespace boost::asio;

TCPDiagnosticServer::TCPDiagnosticServer(boost::asio::io_service& io_service, const uint16_t& port)
: io_service(io_service)
, acceptor{io_service, ip::tcp::endpoint(ip::tcp::v6(), port)}
, timer{io_service, boost::posix_time::seconds(1)} {
	startAccepting();
}

void TCPDiagnosticServer::startAccepting() {
	shared_ptr<ip::tcp::socket> socket = shared_ptr<ip::tcp::socket>{new ip::tcp::socket{io_service}};

	acceptor.async_accept(*socket,
			       boost::bind(&TCPDiagnosticServer::handleAccept, this, socket,
					   boost::asio::placeholders::error));
}

void TCPDiagnosticServer::handleAccept(shared_ptr<ip::tcp::socket> socket, const boost::system::error_code& error) {
	if (!error) {
		//TODO create a new client
		logger::info << "TCP: A new client has connected!\n";
	}

	startAccepting();
}

