#pragma once
#include <vector>
namespace Memory {
	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
	void Nop(BYTE* dst, unsigned int size);
	void NopEx(BYTE* dst, unsigned int size, HANDLE hProcess);
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
	std::uint8_t* SigScan(void* module, const char* signature);
}