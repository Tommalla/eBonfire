/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#ifndef CONNECTIONS_CONTROLLER_HPP
#define CONNECTIONS_CONTROLLER_HPP
#include <functional>
#include <memory>
#include <list>
#include <unordered_map>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "ClientInfo.hpp"
#include "../common.hpp"

class ConnectionsController {
public:
	typedef std::unordered_map<ClientId, std::shared_ptr<ClientInfo>> ClientContainer;
	typedef std::map<boost::asio::ip::udp::endpoint, ClientId> IdContainer;

	ConnectionsController(const size_t& fifoSize, const size_t& fifoLow, const size_t& fifoHigh);
	ClientId registerTCPClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
	void registerUDPClient(const ClientId& clientId, const boost::asio::ip::udp::endpoint& endpoint);
	void removeClient(ClientContainer::iterator iter);
	ClientContainer& getClients();
	const IdContainer& getIds() const;
	std::list<Queue*> getQueues(bool activeOnly=false) const;

private:
	ClientId nextId;
	size_t fifoSize, fifoLow, fifoHigh;
	ClientContainer clientsMap;
	IdContainer idMap;
};

#endif // CONNECTIONS_CONTROLLER_HPP