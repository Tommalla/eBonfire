#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

#include "TCPDiagnosticClient.hpp"

using std::string;
using std::istringstream;

TCPDiagnosticClient::TCPDiagnosticClient(boost::asio::io_service& io, boost::asio::ip::basic_resolver< boost::asio::ip::tcp >::iterator endpoints, const TCPDiagnosticClient::CallbackType& clientCallback)
: socket{io}
, clientCallback{clientCallback} {
	boost::asio::async_connect(socket, endpoints,
				   boost::bind(&TCPDiagnosticClient::handleConnect, this,
					       boost::asio::placeholders::error));
}

void TCPDiagnosticClient::handleConnect(const boost::system::error_code& error) {
	if (!error) {
		//get the header
		boost::asio::async_read_until(socket, message_buffer, '\n',
					      boost::bind(&TCPDiagnosticClient::handleHeader, this,
							  boost::asio::placeholders::error));
	} else {
		throw std::exception();
	}
}

void TCPDiagnosticClient::handleHeader(const boost::system::error_code& error) {
	if (!error) {
		string command;
		uint32_t clientId;

		istringstream input(boost::asio::buffer_cast<const char*>(message_buffer.data()));
		input >> command >> clientId;
		std::cerr << command << " " << clientId << std::endl;

		clientCallback(clientId);

		message_buffer.consume(message_buffer.size());
		getNextMessage();
	} else
		throw std::exception();
}

void TCPDiagnosticClient::getNextMessage() {
	boost::asio::async_read_until(socket, message_buffer, '\n',
				      boost::bind(&TCPDiagnosticClient::handleMessage, this,
						  boost::asio::placeholders::error));
}


void TCPDiagnosticClient::handleMessage(const boost::system::error_code& error) {
	if (!error) {
		std::cerr << boost::asio::buffer_cast<const char*>(message_buffer.data());
		message_buffer.consume(message_buffer.size());

		getNextMessage();
	} else {
		throw std::exception();
	}
}