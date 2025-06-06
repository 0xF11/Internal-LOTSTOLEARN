#include "pch.h"
#include "everything.h"
#include "mem.h"
#include <vector>


	




void mem::patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);

}
// Hey, At this spot in code
// our byte array size
// Handle to our process
void mem::nop(BYTE* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);

}


uintptr_t mem::FindDMAAddy(uintptr_t ptr,std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr; // Assigning our base address to a variable

	for (int i = 0; i < offsets.size(); i++) // I will loop through our offsets until completed
	{
		
		addr = *(uintptr_t*)addr; // Cast address to a pointer and dereference it
		addr += offsets[i];
	}
	return addr;

}
/*
hProcess	The door/key to your house	Permission to enter and do stuff inside
dst			A specific room in your house	The exact memory address you want to access
src			The new furniture or paint	The data you want to write into that room
size		The amount of space you change	How many bytes (how big of an area) you want to overwrite
*/
