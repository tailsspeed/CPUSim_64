// g++ -o main main.cpp CPU.cpp ALU.cpp 
#include "CPU.h"

// using namespace std;

int main(int argc, char const *argv[])
{
    for (size_t i = 0; i < argc; i++)
    {
        // cout << argv[i] << endl;
        printf("%s\n", argv[i]);
    }
    if (argc != 2)
    {
        printf("USAGE: ./main sample_file\n");
        exit(1);
    }
    ifstream in_stream;
    in_stream.open(argv[1]);
    if (in_stream.fail())
    {
        // cout << "Could not open " << argv[1] << endl;
        printf("Could not open %s\n", argv[1]);
        exit(1);
    }
    string line;
    /* FETCH */
    while (getline(in_stream, line))
    {
        printf("length %lu\n", line.length());
        if (line.length() != 16)
        {
            // cout << line << " is not a valid instruction" << endl;
            printf("%s is too many hex digits\n", line.c_str()); // .c_str() returns pointer to C-style string
        }
        else
        {
            try
            {
                /*
                stoull throws exception when argument is not in hex or exceeds range
                    ex. terminate called after throwing an instance of 'std::invalid_argument'
                        what():  stoull
                 */
                uint64_t temp = stoull(line, nullptr, 16);
                /* DECODE */
                uint64_t opcode = temp & 0xFFF0000000000000; // upper 12 bits
                // uint64_t format = temp & 0xF000000000000000; // upper 4 bits
                uint64_t format = temp >> 60; // upper 4 bits
                printf("0x%016lX\n     Opcode: 0x%lX\n     Format: 0x%lX ", temp, opcode, format);
                uint64_t inst_type = (temp << 4) >> 56;
                switch (format)
                {
                case 0xA:
                    {
                        printf("Register->Register\n");
                        uint64_t src = (temp << 12) >> 56;
                        uint64_t dest = (temp << 20) >> 56;
                        printf("Type: 0x%lX\n    Src: 0x%lX\n    Dest: 0x%lX\n", inst_type, src, dest);
                        break;
                    }
                case 0xB:
                    {
                        printf("Register->Memory\n");
                        uint64_t src = (temp << 12) >> 56;
                        uint64_t dest_mem = (temp << 20) >> 32;
                        printf("Type: 0x%lX\n    Src: 0x%lX\n    Dest Mem: 0x%lX\n", inst_type, src, dest_mem);
                        break;
                    }
                    break;
                case 0xC:
                    {
                        printf("Memory->Register\n");
                        uint64_t src_mem = (temp << 12) >> 32;
                        uint64_t dest = (temp << 44) >> 56;
                        printf("Type: 0x%lX\n    Src Mem: 0x%lX\n    Dest: 0x%lX\n", inst_type, src_mem, dest);
                    }
                    break;
                case 0xD:
                    {
                        printf("Immediate->Register\n");
                        uint64_t imm = (temp << 12) >> 32;
                        uint64_t dest = (temp << 44) >> 56;
                        printf("Type: 0x%lX\n    Imm: 0x%lX\n    Dest: 0x%lX\n", inst_type, imm, dest);
                    }
                    break;
                case 0xE:
                    {
                        printf("Jump\n");
                        uint64_t addr = (temp << 12) >> 32;
                        printf("Type: 0x%lX\n    Addr: 0x%lX\n", inst_type, addr);
                    }
                    break;
                default:
                    printf("Invalid format\n");
                    break;
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << " - " << line <<" could not be converted or exceeds 64-bit range" << '\n';
                // printf("%s could not be converted or exceeds 64-bit range\n", line.c_str());
            }
        }
    }
    CPU ts;
    // ts.test_print();

    in_stream.close();
    return 0;
}