/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#ifndef MIXER_CONTROLLER_HPP
#define MIXER_CONTROLLER_HPP
#include <functional>

#include <boost/asio.hpp>

#include "ConnectionsController.hpp"

class MixerController {
public:
	MixerController(boost::asio::io_service& io_service, ConnectionsController* connectionsController, const uint16_t& txInterval, const std::function<void(std::string)>& handleDataProduced);

private:
	void produceData();

	ConnectionsController* connectionsController;
	boost::asio::deadline_timer timer;
	uint16_t txInterval;
	std::function<void(std::string)> handleDataProduced;
};

#endif // MIXER_CONTROLLER_HPP