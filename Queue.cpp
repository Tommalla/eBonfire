/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#include <cstring>
#include "Queue.hpp"

Queue::Queue(const size_t& size)
: size{size}
, end{0}
, container{new char[size]} {}

Queue::~Queue() {
	delete[] container;
}

void Queue::addData(char* data, const size_t& length) {
	memcpy(container + end, data, length);
	end += length;
}

void Queue::consume(const size_t& qty) {
	memmove(container, container, end - qty);
	end -= qty;
}



