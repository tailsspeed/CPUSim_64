#include "ALU.h"
// #define MEMORY_SIZE 1024

// using namespace std;

class CPU
{
private:
    uint64_t reg[256];
    ALU au;
    uint64_t mem[MEMORY_SIZE];
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
