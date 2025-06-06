#pragma once
#include "pch.h"
#include "everything.h"
#include <vector>
namespace mem


{

	void patch(BYTE* dst, BYTE* src, unsigned int size);
	void nop(BYTE* dst, unsigned int size);
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
}
