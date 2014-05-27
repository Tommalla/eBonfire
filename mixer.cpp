/* Tomasz Zakrzewski, tz336079
 * SIK2013/2014, eBonfire
 */
#include <cstdint>

#include "mixer.hpp"

void mixer(mixer_input* inputs, size_t n, void* output_buf, size_t* output_size, long unsigned int tx_interval_ms) {
	*output_size = 176.4f * tx_interval_ms;

	for (size_t i = 0; i < n; ++i)
		inputs[i].consumed = 0;

	for (size_t i = 0; i < *output_size / 2; ++i) {
		int32_t sum = 0;

		for (size_t j = 0; j < n; ++j) {
			if (inputs[j].consumed + 2 <= inputs[j].len) {	//can fit some more data
				sum += ((int16_t *)inputs[j].data)[i];
				inputs[j].consumed += 2;
			}
		}

		((int16_t *)output_buf)[i] = (int16_t)(sum < INT16_MIN ? INT16_MIN : (sum > INT16_MAX ? INT16_MAX : sum));
	}
}
