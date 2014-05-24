/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#ifndef TCP_DIAGNOSTIC_SERVER_HPP
#define TCP_DIAGNOSTIC_SERVER_HPP

#include <functional>
#include <memory>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

class TCPDiagnosticServer {
public:
	TCPDiagnosticServer(boost::asio::io_service& io_service, const uint16_t& port);

private:
	void startAccepting();
	void handleAccept(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
			   const boost::system::error_code& error);
	void sendStats();

	boost::asio::io_service& io_service;
	boost::asio::ip::tcp::acceptor acceptor;

	boost::asio::deadline_timer timer;
};

#endif // TCP_DIAGNOSTIC_SERVER_HPP
