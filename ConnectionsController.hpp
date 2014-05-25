/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#ifndef CONNECTIONS_CONTROLLER_HPP
#define CONNECTIONS_CONTROLLER_HPP
#include <functional>
#include <memory>
#include <unordered_map>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "common.hpp"
#include "ClientInfo.hpp"

class ConnectionsController {
public:
	typedef std::unordered_map<ClientId, std::shared_ptr<ClientInfo>> ClientContainer;
	typedef std::map<boost::asio::ip::udp::endpoint, ClientId> IdContainer;

	ConnectionsController();
	ClientId registerTCPClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
	void registerUDPClient(const ClientId& clientId, const boost::asio::ip::udp::endpoint& endpoint);
	void removeClient(ClientContainer::iterator iter);
	ClientContainer& getClients();

private:
	ClientId nextId;
	ClientContainer clientsMap;
	IdContainer idMap;
};

#endif // CONNECTIONS_CONTROLLER_HPP