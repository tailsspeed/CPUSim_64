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

struct m6502::Mem
{
	static constexpr u32 MAX_MEM = 1024 * 64;
	Byte Data[MAX_MEM];

	void Initialise()
	{
		for ( u32 i = 0; i < MAX_MEM; i++ )
		{
			Data[i] = 0;
		}
	}

	/** read 1 byte */
	Byte operator[]( u32 Address ) const
	{
		// assert here Address is < MAX_MEM
		return Data[Address];
	}

	/** write 1 byte */
	Byte& operator[]( u32 Address )
	{
		// assert here Address is < MAX_MEM
		return Data[Address];
	}
};

struct m6502::StatusFlags
{	
	Byte C : 1;	// Carry Flag	
	Byte Z : 1;	//Zero Flag
	Byte I : 1; //Interrupt disable
	Byte D : 1; //Decimal mode
	Byte B : 1; //Break
	Byte Unused : 1; //Unused
	Byte V : 1; //Overflow
	Byte N : 1; //Negative
};