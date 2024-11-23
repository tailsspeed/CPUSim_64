#include "ALU.h"

uint64_t ALU::execute(uint64_t opcode, uint64_t operand1, uint64_t operand2) {
    switch (opcode) {
    case 0x1: // Addition
        return add(operand1, operand2);
    case 0x2: // Subtraction
        return subtract(operand1, operand2);
    case 0x3: // Multiplication
        return multiply(operand1, operand2);
    case 0x4: // Bitwise AND
        return bitwise_and(operand1, operand2);
    case 0x5: // Bitwise OR
        return bitwise_or(operand1, operand2);
    case 0x6: // Bitwise XOR
        return bitwise_xor(operand1, operand2);
    case 0x7: // Shift Left
        return shift_left(operand1, operand2);
    case 0x8: // Shift Right
        return shift_right(operand1, operand2);
    default:
        std::cerr << "Invalid opcode: 0x" << std::hex << opcode << std::endl;
        return 0;
    }
}

