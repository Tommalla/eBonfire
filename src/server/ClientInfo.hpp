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
	: queue{fifoSize, lowMark, highMark}
	, ack{0}
	, isUDPReady{false}
	, inactiveFor{0} {}

	std::shared_ptr<boost::asio::ip::tcp::socket> tcpSocket;
	boost::asio::ip::udp::endpoint udpEndpoint;
	Queue queue;
	size_t ack;
	bool isUDPReady;
	uint8_t inactiveFor;
};

#endif // CLIENT_INFO_HPP