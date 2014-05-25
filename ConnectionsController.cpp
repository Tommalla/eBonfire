/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#include "ConnectionsController.hpp"
#include "logger.hpp"

using std::shared_ptr;

ConnectionsController::ConnectionsController(const size_t& fifoSize, const size_t& fifoLow, const size_t& fifoHigh)
: nextId{0}
, fifoSize{fifoSize}
, fifoLow{fifoLow}
, fifoHigh{fifoHigh} {}

ClientId ConnectionsController::registerTCPClient(shared_ptr< boost::asio::ip::tcp::socket > socket) {
	auto ptr = shared_ptr<ClientInfo>{new ClientInfo{fifoSize}};
	ptr->tcpSocket = socket;
	clientsMap[nextId++] = ptr;
	logger::info << "New client with id " << nextId - 1 << "\n";

	return nextId - 1;
}

void ConnectionsController::registerUDPClient(const ClientId& clientId, const boost::asio::ip::udp::endpoint& endpoint) {
	if (clientId >= nextId) {
		logger::warn << "Got a CLIENT datagram with incorrect id (" << clientId << ")\n";
		return;
	} else if (clientsMap.find(clientId) == clientsMap.end()) {
		logger::warn << "Got a CLIENT datagram from a client considered dead (id = " << clientId << ")\n";
		return;
	}

	clientsMap.at(clientId)->udpEndpoint = endpoint;
	idMap[endpoint] = clientId;

	logger::info << "Register client's " << clientId << " UDP endpoint\n";
}

void ConnectionsController::removeClient(ClientContainer::iterator iter) {
	auto it = idMap.find(iter->second->udpEndpoint);
	logger::info << "Removing client " << it->second << "\n";
	idMap.erase(it);
	clientsMap.erase(iter);
}


ConnectionsController::ClientContainer& ConnectionsController::getClients() {
	return clientsMap;
}


