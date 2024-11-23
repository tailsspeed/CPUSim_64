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
    uint64_t add(uint64_t num1, uint64_t num2);
    uint64_t subtract(uint64_t num1, uint64_t num2);
    uint64_t multiply(uint64_t num1, uint64_t num2);
    uint64_t bitwise_and(uint64_t num1, uint64_t num2);
    uint64_t bitwise_or(uint64_t num1, uint64_t num2);
    uint64_t bitwise_xor(uint64_t num1, uint64_t num2);
    uint64_t shift_left(uint64_t num1, uint64_t num2);
    uint64_t shift_right(uint64_t num1, uint64_t num2);
};

#endif
