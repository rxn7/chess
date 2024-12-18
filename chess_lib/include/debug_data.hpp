#pragma once

#include <chrono>

struct DebugData {
	uint32_t legalMovesCount = 0;
	std::chrono::duration<float> legalMovesCalculationTime;
};
