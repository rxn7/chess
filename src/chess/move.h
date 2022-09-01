#pragma once

#include <cstdint>

struct Move {
	union {
		struct {
			uint8_t fromIdx;
			uint8_t toIdx;
		};

		uint8_t indices[2];
	};

	Move() {
		reset();
	}

	Move(uint8_t fromIdx, uint8_t toIdx) : fromIdx(fromIdx), toIdx(toIdx) {
	}

	inline void reset() {
		for(uint8_t &i : indices)
			i = 64; // Everything above 63 or below 0 is ignored.
	}

	inline bool isNull() {
		return indices[0] == 64;
	}
};