/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#ifndef COMMON_HPP
#define COMMON_HPP
#include <cstdint>
#include <cstddef>

typedef uint32_t ClientId;
typedef uint16_t Port;

const Port defaultPort = 16079;
const size_t defaultFifoSize = 10560;
const size_t defaultBufferLength = 10;
const uint16_t defaultTXInterval = 5;
const size_t defaultRetransmitLimit = 10;

#endif // COMMON_HPP