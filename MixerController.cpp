/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#include "MixerController.hpp"
#include "mixer.hpp"

using std::string;

MixerController::MixerController(boost::asio::io_service& io_service, ConnectionsController* connectionsController, const uint16_t& txInterval, const std::function<void(string)>& handleDataProduced)
: connectionsController{connectionsController}
, timer{io_service, boost::posix_time::milliseconds(txInterval)}
, txInterval{txInterval}
, handleDataProduced{handleDataProduced} {
	timer.async_wait(boost::bind(&MixerController::produceData, this));
}

void MixerController::produceData() {
	timer.expires_from_now(boost::posix_time::milliseconds(txInterval));
	timer.async_wait(boost::bind(&MixerController::produceData, this));

	auto queues = connectionsController->getQueues(true);

	mixer_input* inputs = new mixer_input[queues.size()];
	char* output = new char[200 * txInterval];
	size_t size = 200 * txInterval;

	size_t i = 0;
	for (auto& q: queues) {
		inputs[i].data = (void*) q->getData();
		inputs[i].len = q->getLength();
		++i;
	}

	mixer(inputs, queues.size(), output, &size, txInterval);
	i = 0;
	for (auto& q: queues)
		q->consume(inputs[i++].consumed);

	handleDataProduced(string(output, output + size));

	delete inputs;
	delete output;
}
