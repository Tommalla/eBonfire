/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP

#include <ctime>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include "common.hpp"

class UDPClient {
public:
	UDPClient(boost::asio::io_service& io_service, const boost::asio::ip::udp::endpoint& targetEndpoint);
	void initUDP(const ClientId& clientId);

private:
	void handleClientStarted(const boost::system::error_code& error /*bytes_transferred*/);
	void startReceive();
	void handleReceive(const boost::system::error_code& error,
		    std::size_t length);
	void readInput();
	void handleEndReadInput(const boost::system::error_code& error, size_t size);
	void sendKeepalive();
	void checkConnection();

	boost::asio::ip::udp::socket socket;
	boost::asio::ip::udp::endpoint remoteEndpoint, targetEndpoint;
	boost::array<char, 1<<16> receiveBuffer;
	boost::array<char, 12000> inputBuffer;
	boost::asio::posix::stream_descriptor dataInput;
	bool isReading, isAlive;
	uint16_t lastAck, lastWin, lastData, lastId;
	boost::asio::deadline_timer keepaliveTimer, connectionTimer;
};

#endif // UDP_CLIENT_HPP