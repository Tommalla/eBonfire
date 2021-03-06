/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>

#include "../common.hpp"
#include "../logger.hpp"
#include "TCPDiagnosticClient.hpp"
#include "UDPClient.hpp"

using std::string;
using std::cout;
using namespace boost;

int main(int argc, char** argv) {
	string serverName;
	Port port;
	size_t retransmitLimit;
	program_options::options_description description("Program options");

	try {
		description.add_options()
		("help,h", "help")
		(",p", program_options::value<Port>(&port)->default_value(defaultPort), "port number")
		(",s", program_options::value<string>(&serverName)->required(), "server address")
		(",X", program_options::value<size_t>(&retransmitLimit)->default_value(defaultRetransmitLimit), "retransmit limit");

		program_options::variables_map vars;
		program_options::store(program_options::parse_command_line(argc, argv, description), vars);

		if (vars.count("help")) {
			cout << description << "\n";
			return 0;
		}

		program_options::notify(vars);

		logger::info << "SERVER: " << serverName << "\n" << "PORT: "
		<< port << " " << "RETRANSMIT LIMIT: " << retransmitLimit << "\n";

		while (true) {
			try {
				asio::io_service io_service;

				asio::ip::tcp::resolver tcpResolver{io_service};
				asio::ip::tcp::resolver::query tcpQuery{serverName, boost::lexical_cast<string>(port),
					boost::asio::ip::resolver_query_base::flags()};
				asio::ip::tcp::resolver::iterator tcpIter = tcpResolver.resolve(tcpQuery);

				asio::ip::udp::resolver udpResolver{io_service};
				asio::ip::udp::resolver::query udpQuery{serverName, boost::lexical_cast<string>(port),
					boost::asio::ip::resolver_query_base::flags()};
				asio::ip::udp::resolver::iterator udpIter = udpResolver.resolve(udpQuery);

				UDPClient udpClient{io_service, *udpIter, retransmitLimit};
				TCPDiagnosticClient tcpClient{io_service, tcpIter, std::bind(&UDPClient::initUDP, &udpClient, std::placeholders::_1)};

				io_service.run();
			} catch (std::exception& e) {
				logger::error << e.what() << "\n";
			}
		}

	} catch(program_options::error& e) {
		logger::error << e.what() << "\n\n" << description << "\n";
		return -1;
	}

	return 0;
}
