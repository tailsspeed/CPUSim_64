#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// Function prototypes
uint64_t RegisterToRegister(string opcode, string source, string destination);
uint64_t ImmediateToRegister(string opcode, string source, string destination);
uint64_t MemoryToRegister(string opcode, string source, string destination);
uint64_t RegisterToMemory(string opcode, string source, string destination);
uint64_t Jump(string opcode, string destination);

uint16_t opcodeStringToNumber(string opcode);
uint8_t registerStringToNumber(string reg);

// Helper to determine if a word is a register (e.g., R1, R2, etc.)
bool isRegister(const std::string& word) {
    return word.size() > 1 && word[0] == 'R' && std::isdigit(word[1]);
}

// Helper to determine if a word is a numeric immediate
bool isImmediate(const std::string& word) {
    return !word.empty() && std::isdigit(word[0]);
}

void processCodeSection(std::ifstream& file) {
    std::string line;
    bool inCodeSection = false;

    while (std::getline(file, line)) {
        // Ignore lines starting with "#"
        if (line.find("#") == 0) continue;

        // Check for ".code" section
        if (!inCodeSection) {
            if (line == ".code") {
                inCodeSection = true;
                continue;
            } else {
                continue; // Ignore lines before ".code"
            }
        }

        // Ignore comments starting with "#"
        auto commentPos = line.find("#");
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos); // Remove everything after "#"
        }

        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t")); // Left trim
        line.erase(line.find_last_not_of(" \t") + 1); // Right trim

        // Split into words
        std::istringstream iss(line);
        std::vector<std::string> words;
        std::string word;
        while (iss >> word) {
            words.push_back(word);
        }

        // Check word count
        if (words.empty()) continue;
        if (words.size() > 3) {
            std::cerr << "Error: Line exceeds 3 words: \"" << line << "\"" << std::endl;
            continue;
        }

        // Determine instruction type and call the appropriate function
        const std::string& firstWord = words[0];

        if (words.size() == 3 && isRegister(words[1]) && isRegister(words[2])) {
            // Register to Register
            cout << hex << RegisterToRegister(words[0],words[1],words[2]) << endl;
        } else if (words.size() == 3 && isImmediate(words[1]) && isRegister(words[2])) {
            // Immediate to Register
            ImmediateToRegister(words[0],words[1],words[2]);
        } else if (words.size() == 3 && !isRegister(words[1]) && isRegister(words[2])) {
            // Memory to Register
            MemoryToRegister(words[0],words[1],words[2]);
        } else if (words.size() == 3 && isRegister(words[1]) && !isRegister(words[2])) {
            // Register to Memory
            RegisterToMemory(words[0],words[1],words[2]);
        } else if (words.size() == 2) {
            // Jump
            Jump(words[0],words[1]);
        } else {
            std::cerr << "Error: Invalid instruction format: \"" << line << "\"" << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    // Determine the file name
    std::string fileName = (argc > 1) ? argv[1] : "source.asm";

    std::ifstream file(fileName); // Open the file
    if (!file.is_open()) {        // Check if the file opened successfully
        std::cerr << "Unable to open file: " << fileName << std::endl;
        return 1;
    }

    // Process the file
    processCodeSection(file);

    file.close(); // Close the file
    return 0;
}

uint16_t opcodeStringToNumber(string opcode) {
    static std::unordered_map<std::string, uint8_t> opcodeMap = {
        {"MOV", 1}, {"ADD", 2}, {"SUB", 3}, {"MUL", 4}, {"DIV", 5},
        {"AND", 6}, {"OR", 7}, {"INC", 8}, {"DEC", 9}, {"SHL", 10},
        {"SHR", 11}
    };

    auto it = opcodeMap.find(opcode);
    if (it != opcodeMap.end()) {
        return it->second;
    } else {
        throw std::invalid_argument("Invalid opcode: " + opcode);
    }
}

uint8_t registerStringToNumber(string reg) {
    if (reg.size() < 2 || reg[0] != 'R') {
        throw std::invalid_argument("Invalid register format: " + reg);
    }
    return std::stoi(reg.substr(1)); // Convert the substring after 'R' to integer
}

// Placeholder function definitions
uint64_t RegisterToRegister(string opcode, string source, string destination) {
    uint64_t instruction = 0;

    try {
        // Convert source and destination registers to integers
        uint8_t sauce = registerStringToNumber(source);
        uint8_t desti = registerStringToNumber(destination);

        // Convert opcode to its numeric value
        uint8_t opcodeNum = opcodeStringToNumber(opcode);

        // Construct the instruction
        instruction = 
            (static_cast<uint64_t>(opcodeNum) << 52) | // Opcode in bits [63:56]
            (static_cast<uint64_t>(sauce) << 44)     | // Source register in bits [55:48]
            (static_cast<uint64_t>(desti) << 36);      // Destination register in bits [47:40]
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return instruction;
}

uint64_t ImmediateToRegister(string opcode, string source, string destination) {
	
    return 0; // Placeholder return value
}

uint64_t MemoryToRegister(string opcode, string source, string destination) {
    
    return 0; // Placeholder return value
}

uint64_t RegisterToMemory(string opcode, string source, string destination) {
   
    return 0; // Placeholder return value
}

uint64_t Jump(string opcode, string destination) {
    
    return 0; // Placeholder return value
}
