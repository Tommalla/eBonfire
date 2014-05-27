/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "common.hpp"
#include "ProblematicConnectionException.hpp"

class UDPClient {
public:
	UDPClient(boost::asio::io_service& io_service, const boost::asio::ip::udp::endpoint& targetEndpoint, const size_t& retransmitLimit);
	void initUDP(const ClientId& clientId);

private:
	void handleClientStarted(const boost::system::error_code& error /*bytes_transferred*/);
	void startReceive();
	void handleReceive(const boost::system::error_code& error, std::size_t length);
	void readInput();
	void handleEndReadInput(const boost::system::error_code& error, size_t size);
	void sendKeepalive();
	void checkConnection();
	bool equal(boost::asio::ip::udp::endpoint a, boost::asio::ip::udp::endpoint b) const;

	boost::asio::ip::udp::socket socket;
	boost::asio::ip::udp::endpoint remoteEndpoint, targetEndpoint;
	boost::array<char, 1<<16> receiveBuffer;
	boost::array<char, (1<<16) - 1> inputBuffer;
	boost::asio::posix::stream_descriptor dataInput;
	bool isReading, isAlive, dataReceived;;
	size_t retransmitLimit, lastData, lastId, lastAck, lastWin, nrMaxSeen;
	uint8_t dataQty;
	boost::asio::deadline_timer keepaliveTimer, connectionTimer;
	std::string lastInput;
};

#endif // UDP_CLIENT_HPP
