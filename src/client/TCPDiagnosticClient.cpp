/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#include <iostream>

#include <boost/bind.hpp>

#include "../logger.hpp"
#include "ProblematicConnectionException.hpp"
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
		throw ProblematicConnectionException();
	}
}

void TCPDiagnosticClient::handleHeader(const boost::system::error_code& error) {
	if (!error) {
		string command;
		uint32_t clientId;

		istringstream input(boost::asio::buffer_cast<const char*>(message_buffer.data()));
		input >> command >> clientId;
		logger::info << command << " " << clientId << std::endl;

		clientCallback(clientId);

		message_buffer.consume(message_buffer.size());
		getNextMessage();
	} else
		throw ProblematicConnectionException();
}

void TCPDiagnosticClient::getNextMessage() {
	boost::asio::async_read_until(socket, message_buffer, '\n',
				      boost::bind(&TCPDiagnosticClient::handleMessage, this,
						  boost::asio::placeholders::error));
}


void TCPDiagnosticClient::handleMessage(const boost::system::error_code& error) {
	if (!error) {
		const char* ptr = boost::asio::buffer_cast<const char*>(message_buffer.data());
		string str(ptr, ptr + message_buffer.size());
		logger::info << str;
		message_buffer.consume(message_buffer.size());

		getNextMessage();
	} else {
		throw ProblematicConnectionException();
	}
}