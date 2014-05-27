/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP
#include <queue>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "common.hpp"
#include "ConnectionsController.hpp"

class UDPServer {
public:
	UDPServer(boost::asio::io_service& io_service, const Port& port, const size_t& bufferLength, ConnectionsController* connectionsController);
	void handleDataProduced(std::string data);

private:
	void startReceive();
	void handleReceive(const boost::system::error_code& error, std::size_t size);
	void retransmit(const size_t& begin, const boost::asio::ip::udp::endpoint& endpoint);
	void sendData(const std::string& data, const size_t& id, const std::shared_ptr< ClientInfo >& client);
	void addToSend(const std::string& msg, const boost::asio::ip::udp::endpoint& endpoint);
	void sendMessage();

	ConnectionsController* connectionsController;
	boost::asio::ip::udp::socket socket;
	boost::asio::ip::udp::endpoint remoteEndpoint;
	boost::array<char, 1<<16> messageBuffer;
	const size_t bufferLength;
	size_t nextDataId;
	std::list<std::pair<size_t, std::string>> serverBuffer;
	std::queue<std::pair<boost::asio::ip::udp::endpoint, std::string>> messagesQueue;
	bool isSending;
};

#endif // UDP_SERVER_HPP