#pragma once

#include <cstdint>

struct Move {
	Move() {
		reset();
	}

	Move(std::uint8_t fromIdx, std::uint8_t toIdx) : fromIdx(fromIdx), toIdx(toIdx) {}

	inline void reset() {
		for (std::uint8_t &i : indices)
			i = 64; // Everything above 63 or below 0 is ignored.
	}

	inline bool isNull() const {
		return indices[0] == 64;
	}

	union {
		struct {
			std::uint8_t fromIdx;
			std::uint8_t toIdx;
		};

		uint8_t indices[2];
	};
};
