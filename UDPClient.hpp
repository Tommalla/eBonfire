/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

class UDPClient {
public:
	UDPClient(boost::asio::io_service& io_service, const uint16_t& port,
		  const boost::asio::ip::udp::endpoint& targetEndpoint);

private:
	void start_receive();
	void initUDP(const uint32_t& clientId);

	void handle_receive(const boost::system::error_code& error,
		    std::size_t length);

	void handle_send(boost::shared_ptr<std::string> /*message*/,
		 const boost::system::error_code& /*error*/,
		 std::size_t /*bytes_transferred*/);

	boost::asio::ip::udp::socket socket;
	boost::asio::ip::udp::endpoint remote_endpoint_, targetEndpoint;
	boost::array<char, 1<<16> recv_buffer_;
};

#endif // UDP_CLIENT_HPP