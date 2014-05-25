/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#ifndef CLIENT_INFO_HPP
#define CLIENT_INFO_HPP
#include <memory>

#include <boost/asio.hpp>

#include "Queue.hpp"

struct ClientInfo {
	ClientInfo(const size_t& fifoSize, const size_t& lowMark, const size_t& highMark)
	: queue{fifoSize, lowMark, highMark}{}

	std::shared_ptr<boost::asio::ip::tcp::socket> tcpSocket;
	boost::asio::ip::udp::endpoint udpEndpoint;
	Queue queue;
};

#endif // CLIENT_INFO_HPP