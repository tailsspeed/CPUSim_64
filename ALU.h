#ifndef ALU_H
#define ALU_H


#include <cstdint>
#include <iostream>

class ALU {
public:
    //constructor
    ALU() {}

    //execute function
   uint64_t execute(uint64_t opcode, uint64_t operand1, uint64_t operand2);

//abstract method
private:
   
};

#endif
