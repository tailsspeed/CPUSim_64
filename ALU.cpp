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

/* overflow flag */
void ALU::set_overflow()
{
    reg_ptr[255] = reg_ptr[255] | 0x0000000000000800;
}

void ALU::clear_overflow()
{
    reg_ptr[255] = reg_ptr[255] & 0xFFFFFFFFFFFFF7FF;
}

/* sign flag */
void ALU::set_sign()
{
    reg_ptr[255] = reg_ptr[255] | 0x0000000000000080;
}

void ALU::clear_sign()
{
    reg_ptr[255] = reg_ptr[255] & 0xFFFFFFFFFFFFFF7F;
}

/* zero flag */
void ALU::set_zero()
{
    reg_ptr[255] = reg_ptr[255] | 0x0000000000000040;
}

void ALU::clear_zero()
{
    reg_ptr[255] = reg_ptr[255] & 0xFFFFFFFFFFFFFFBF;
}

/* parity flag */
void ALU::set_parity()
{
    reg_ptr[255] = reg_ptr[255] | 0x0000000000000004;
}

void ALU::clear_parity()
{
    reg_ptr[255] = reg_ptr[255] & 0xFFFFFFFFFFFFFFFB;
}