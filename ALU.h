#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <cstdint>
#include <inttypes.h>
#define MEMORY_SIZE 1024

using namespace std;

class ALU
{
private:
    uint64_t *reg_ptr; // pointer to the CPU's registers
public:
    ALU();
    ALU(uint64_t reg[]);
    ~ALU();
    /* FLAG Instructions */
    void clc(); // clear carry flag
    void cld(); // clear direction flag
    void cli(); // clear interrupt flag
    void cmc(); // complement carry flag
    void stc(); // set carry flag
    void std(); // set direction flag
    void sti(); // set interrupt flag
    /* Helper functions to set flags during arithmetic */
    void set_overflow();
    void clear_overflow();
    void set_sign();
    void clear_sign();
    void set_zero();
    void clear_zero();
    void set_parity();
    void clear_parity();
    /*
    adc
    add
    and (logical)
    cmp
    dec (decrement)
    div (unsigned)
    idiv (signed)
    imul (signed)
    inc (increment)
    mul
    neg (2's complement)
    not (1's complement)
    or (logical)
    rcl (rotate carry left)
    rcr (rotate carry right)
    rol (rotate left)
    ror (rotate right)
    sal (arithmetic left)
    sar (arithmetic right)
    shl (logical left)
    shr (logical right)
    sub
    */
};
