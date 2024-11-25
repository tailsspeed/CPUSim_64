#include "ALU.h"

class CPU
{
private:
    uint64_t reg[256];
    uint64_t mem[MEMORY_SIZE]; // could be declared in a seperate file as its own class
public:
    ALU au = ALU(reg);
    CPU(/* external memory, ifstream? */);
    void test_print();
    /*
    start - begin reading from input stream and executing instructions
    fetch - read from input stream and convert from 
    decode - get operands and instruction type
    execute - CPU executes instruction
    store - store in register/memory
     */
    ~CPU();
};
