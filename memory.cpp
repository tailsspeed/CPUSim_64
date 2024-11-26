#pragma once
#include <stdio.h>
#include <stdlib.h>

namespace m6502
{
	using SByte = char;
	using Byte = unsigned char;
	using Word = unsigned short;

	using u32 = unsigned int;
	using s32 = signed int;

	struct Mem;
	struct CPU;
	struct StatusFlags;
}