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

uint64_t ALU::add(uint64_t num1, uint64_t num2) {
    return num1 + num2;
}

uint64_t ALU::subtract(uint64_t num1, uint64_t num2) {
    return num1 - num2;
}

uint64_t ALU::multiply(uint64_t num1, uint64_t num2) {
    return num1 * num2;
}

uint64_t ALU::bitwise_and(uint64_t num1, uint64_t num2) {
    return num1 & num2;
}

uint64_t ALU::bitwise_or(uint64_t num1, uint64_t num2) {
    return num1 | num2;
}

uint64_t ALU::bitwise_xor(uint64_t num1, uint64_t num2) {
    return num1 ^ num2;
}

uint64_t ALU::shift_left(uint64_t num1, uint64_t num2) {
    return num1 << num2;
}

uint64_t ALU::shift_right(uint64_t num1, uint64_t num2) {
    return num1 >> num2;
}

