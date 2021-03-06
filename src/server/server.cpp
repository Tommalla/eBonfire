/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#include <functional>
#include <iostream>

#include <boost/program_options.hpp>

#include "../common.hpp"
#include "ConnectionsController.hpp"
#include "../logger.hpp"
#include "MixerController.hpp"
#include "TCPDiagnosticServer.hpp"
#include "UDPServer.hpp"

using std::cout;
using namespace boost;

int main(int argc, char **argv) {
	Port port;
	size_t fifoSize, fifoLow, fifoHigh, bufferLength;
	uint16_t txInterval;

	program_options::options_description description("Program options");
	description.add_options()
	("help,h", "help")
	(",p", program_options::value<Port>(&port)->default_value(defaultPort), "port number")
	(",F", program_options::value<size_t>(&fifoSize)->default_value(defaultFifoSize), "FIFO size")
	(",L", program_options::value<size_t>(&fifoLow)->default_value(0), "FIFO low watermark")
	(",H", program_options::value<size_t>(), "FIFO high watermark")
	(",X", program_options::value<size_t>(&bufferLength)->default_value(defaultBufferLength), "buffer length")
	(",i", program_options::value<uint16_t>(&txInterval)->default_value(defaultTXInterval), "tx interval");

	program_options::variables_map vm;
	program_options::store(program_options::parse_command_line(argc, argv, description), vm);
	program_options::notify(vm);

	if (vm.count("help")) {
		cout << description << "\n";
		return 0;
	}

	if (vm.count("-H"))
		fifoHigh = vm["-H"].as<uint16_t>();
	else
		fifoHigh = fifoSize;

	logger::info << "PORT: " << port << "\nFIFO SIZE: " << fifoSize << "\nFIFO LOW WATERMARK: "
	<< fifoLow << " FIFO HIGH WATERMARK: " << fifoHigh << "\nBUF LEN: "
	<< bufferLength << " TX INTERVAL: " << txInterval << "\n";

	try {
		boost::asio::io_service ioService;

		ConnectionsController connectionsController{fifoSize, fifoLow, fifoHigh};
		TCPDiagnosticServer tcpServer{ioService, port, &connectionsController};
		UDPServer udpServer{ioService, port, bufferLength, &connectionsController};
		MixerController mixerController{ioService, &connectionsController, txInterval, std::bind(&UDPServer::handleDataProduced, &udpServer, std::placeholders::_1)};

		ioService.run();
	} catch (const std::exception& e) {
		logger::error << e.what() << "\n";
	}

	return 0;
}