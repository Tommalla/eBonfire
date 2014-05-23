#include <iostream>
#include "UDPClient.hpp"

using std::string;
using namespace boost;

UDPClient::UDPClient(asio::io_service& io_service, const uint16_t& port,
		     const asio::ip::udp::endpoint& targetEndpoint)
: socket{io_service, asio::ip::udp::endpoint{asio::ip::address::from_string("127.0.0.1"), port}}	// FIXME
, targetEndpoint{targetEndpoint} {
}

void UDPClient::initUDP(const uint32_t& clientId) {
	start_receive();
	boost::shared_ptr<std::string> msg(new string("CLIENT " + std::to_string(clientId) + "\n"));

	socket.async_send_to(boost::asio::buffer(*msg), targetEndpoint,
			      boost::bind(&UDPClient::handle_send, this, msg,
					  boost::asio::placeholders::error,
		     boost::asio::placeholders::bytes_transferred));
}


void UDPClient::start_receive() {
	socket.async_receive_from(
		boost::asio::buffer(recv_buffer_), remote_endpoint_,
				   boost::bind(&UDPClient::handle_receive, this,
					       boost::asio::placeholders::error,
		   boost::asio::placeholders::bytes_transferred));
}

void UDPClient::handle_receive(const system::error_code& error, std::size_t length) {
	if (!error || error == boost::asio::error::message_size)
	{
// 		if (remote_endpoint_ == targetEndpoint)
		for (size_t i = 0; i < length; ++i)
			printf("%c", recv_buffer_[i]);
		start_receive();
	}
}

void UDPClient::handle_send(shared_ptr< std::string >, const system::error_code&, std::size_t) {}


