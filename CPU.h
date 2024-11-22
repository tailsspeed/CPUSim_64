#include "ALU.h"

class CPU
{
private:
    uint64_t reg[256];
    ALU au;
    uint64_t mem[MEMORY_SIZE]; // could be declared in a seperate file as its own class
public:
    CPU(/* args */);
    void test_print();
    /*
    fetch
    decode
    execute
    store
     */
    ~CPU();
};
