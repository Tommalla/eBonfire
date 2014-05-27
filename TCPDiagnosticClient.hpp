/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#ifndef TCP_DIAGNOSTIC_CLIENT_HPP
#define TCP_DIAGNOSTIC_CLIENT_HPP
#include <functional>

#include <boost/asio.hpp>

class TCPDiagnosticClient {
	typedef std::function<void(const uint32_t&)> CallbackType;
public:
	TCPDiagnosticClient(boost::asio::io_service& io, boost::asio::ip::tcp::resolver::iterator endpoints, const CallbackType& clientCallback);

private:
	void handleConnect(const boost::system::error_code& error);
	void handleHeader(const boost::system::error_code& error);
	void getNextMessage();
	void handleMessage(const boost::system::error_code& error);

	boost::asio::ip::tcp::socket socket;
	boost::asio::streambuf message_buffer;
	CallbackType clientCallback;
};

#endif // TCP_DIAGNOSTIC_CLIENT_HPP