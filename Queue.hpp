/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <cstddef>

class Queue {
public:
	struct Stats {
		size_t size, end, minSize, maxSize;
	};

	Queue(const size_t& size, const size_t& lowMark, const size_t& highMark);
	Queue(const Queue& other) = delete;
	Queue(Queue&& other) = delete;
	Queue& operator=(const Queue& other) = delete;
	Queue&& operator=(Queue&& other) = delete;
	~Queue();

	void addData(char* data, const size_t& length);
	void consume(const size_t& qty);
	Stats getStats();
	bool isActive() const;

private:
	size_t size, end, minSize, maxSize, lowMark, highMark;
	char* container;
	bool active;

};

#endif // QUEUE_HPP