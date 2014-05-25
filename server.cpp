/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#include <functional>
#include <iostream>
#include <boost/program_options.hpp>

#include "common.hpp"
#include "TCPDiagnosticServer.hpp"
#include "UDPServer.hpp"
#include "ConnectionsController.hpp"
#include "logger.hpp"

using std::cout;
using namespace boost;

int main(int argc, char **argv) {
	Port port;
	uint16_t fifoSize, fifoLow, fifoHigh, bufferLength, txInterval;

	program_options::options_description description("Program options");
	description.add_options()
	("help,h", "help")
	(",p", program_options::value<Port>(&port)->default_value(10000 + (336079 % 10000)), "port number")
	(",F", program_options::value<uint16_t>(&fifoSize)->default_value(10560), "FIFO size")
	(",L", program_options::value<uint16_t>(&fifoLow)->default_value(0), "FIFO low watermark")
	(",H", program_options::value<uint16_t>(), "FIFO high watermark")
	(",X", program_options::value<uint16_t>(&bufferLength)->default_value(10), "buffer length")
	(",i", program_options::value<uint16_t>(&txInterval)->default_value(5), "tx interval");

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

		ConnectionsController connectionsController;
		TCPDiagnosticServer tcpServer{ioService, port, &connectionsController};
		UDPServer udpServer{ioService, port, &connectionsController};

		ioService.run();
	} catch (const std::exception& e) {
		logger::error << e.what() << "\n";
	}

	return 0;
}