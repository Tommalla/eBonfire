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

#include "common.hpp"

class TCPDiagnosticServer {
	typedef boost::asio::ip::tcp::socket TCPSocket;
	typedef std::function<ClientId(std::shared_ptr<TCPSocket>)> RegisterFunc;
public:
	TCPDiagnosticServer(boost::asio::io_service& io_service, const uint16_t& port, const RegisterFunc& registerFunc);

private:
	void startAccepting();
	void handleAccept(std::shared_ptr<TCPSocket> socket,
			   const boost::system::error_code& error);
	void sendStats();

	boost::asio::io_service& io_service;
	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::deadline_timer timer;
	RegisterFunc registerFunc;
};

#endif // TCP_DIAGNOSTIC_SERVER_HPP
