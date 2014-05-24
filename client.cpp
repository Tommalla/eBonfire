/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/thread/thread.hpp>

#include "common.hpp"
#include "UDPClient.hpp"
#include "TCPDiagnosticClient.hpp"

using std::string;
using std::cout;
using std::cerr;
using namespace boost;

int main(int argc, char** argv) {
	string serverName;
	Port port;
	uint16_t retransmitLimit;
	program_options::options_description description("Program options");

	try {
		description.add_options()
		("help,h", "help")
		(",p", program_options::value<Port>(&port)->default_value(10000 + (336079 % 10000)), "port number")
		(",s", program_options::value<string>(&serverName)->required(), "server address")
		(",X", program_options::value<uint16_t>(&retransmitLimit)->default_value(10), "retransmit limit");

		program_options::variables_map vars;
		program_options::store(program_options::parse_command_line(argc, argv, description), vars);

		if (vars.count("help")) {
			cout << description << "\n";
			return 0;
		}

		program_options::notify(vars);

		cerr << "SERVER: " << serverName << "\n" << "PORT: "
		<< port << " " << "RETRANSMIT LIMIT: " << retransmitLimit << "\n";


		//TODO
		// retransmission

		try
		{
			asio::io_service io_service;

			asio::ip::tcp::resolver tcpResolver{io_service};
			asio::ip::tcp::resolver::query tcpQuery{serverName, boost::lexical_cast<string>(port),
								boost::asio::ip::resolver_query_base::flags()};
			asio::ip::tcp::resolver::iterator tcpIter = tcpResolver.resolve(tcpQuery);

			asio::ip::udp::resolver udpResolver{io_service};
			asio::ip::udp::resolver::query udpQuery{serverName, boost::lexical_cast<string>(port),
								boost::asio::ip::resolver_query_base::flags()};
			asio::ip::udp::resolver::iterator udpIter = udpResolver.resolve(udpQuery);

			UDPClient udpClient{io_service, *udpIter};
			TCPDiagnosticClient tcpClient{io_service, tcpIter, std::bind(&UDPClient::initUDP, &udpClient, std::placeholders::_1)};

			io_service.run();
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}

	} catch(program_options::error& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
		std::cerr << description << std::endl;
		return -1;
	}


	return 0;
}
