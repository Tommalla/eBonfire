/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>

class Queue {
public:
	Queue(const size_t& size);
	Queue(const Queue& other) = delete;
	Queue(Queue&& other) = delete;
	Queue& operator=(const Queue& other) = delete;
	Queue&& operator=(Queue&& other) = delete;
	~Queue();

	void addData(char* data, const size_t& length);
	void consume(const size_t& qty);

private:
	size_t size, end;
	char* container;

};

#endif // QUEUE_HPP