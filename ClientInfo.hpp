/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#ifndef CLIENT_INFO_HPP
#define CLIENT_INFO_HPP
#include <memory>

#include <boost/asio.hpp>

struct ClientInfo {
	std::shared_ptr<boost::asio::ip::tcp::socket> tcpSocket;
	boost::asio::ip::udp::endpoint udpEndpoint;
};

#endif // CLIENT_INFO_HPP