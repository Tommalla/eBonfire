/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#include <cstring>
#include <boost/concept_check.hpp>
#include "Queue.hpp"

Queue::Queue(const size_t& size, const size_t& lowMark, const size_t& highMark)
: size{size}
, end{0}
, minSize{0}
, maxSize{0}
, lowMark{lowMark}
, highMark{highMark}
, container{new char[size]} {}

Queue::~Queue() {
	delete[] container;
}

void Queue::addData(char* data, const size_t& length) {
	memcpy(container + end, data, length);
	end += length;
	if (end >= highMark)
		active = true;
	maxSize = std::max(maxSize, end);
}

const char* Queue::getData() const {
	return container;
}

void Queue::consume(const size_t& qty) {
	memmove(container, container + qty, end - qty);
	end -= qty;
	if (end <= lowMark)
		active = false;
	minSize = std::min(minSize, end);
}

Queue::Stats Queue::getStats() {
	Stats res;
	res.size = size;
	res.end = end;
	res.maxSize = maxSize;
	res.minSize = minSize;
	minSize = maxSize = end;
	return res;
}

bool Queue::isActive() const {
	return active;
}

size_t Queue::getFreeSpace() const {
	return size - end;
}

size_t Queue::getLength() const {
	return end;
}



