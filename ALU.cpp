#include "ALU.h"

ALU::ALU(uint64_t reg[])
{
    reg_ptr = &reg[0];
}

ALU::~ALU()
{
}

/* carry flag */
void ALU::clc()
{
    reg_ptr[255] = reg_ptr[255] & 0xFFFFFFFFFFFFFFFE;
}

void ALU::stc()
{
    reg_ptr[255] = reg_ptr[255] | 0x0000000000000001;
}

void ALU::cmc()
{
    reg_ptr[255] = reg_ptr[255] ^ 0x0000000000000001;
}

/* direction flag */
void ALU::cld()
{
    reg_ptr[255] = reg_ptr[255] & 0xFFFFFFFFFFFFFBFF;
}

void ALU::std()
{
    reg_ptr[255] = reg_ptr[255] | 0x0000000000000400;
}

/* interrupt flag */
void ALU::cli()
{
    reg_ptr[255] = reg_ptr[255] & 0xFFFFFFFFFFFFFDFF;
}

void ALU::sti()
{
    reg_ptr[255] = reg_ptr[255] | 0x0000000000000200;
}