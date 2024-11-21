// g++ -o main main.cpp
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
            printf("%s is not a valid instruction\n", line.c_str()); // .c_str() returns pointer to C-style string
        }
        else
        {
            uint64_t temp = stoull(line, nullptr, 16);
            /* DECODE */
            uint64_t opcode = temp & 0xFFF0000000000000; // upper 12 bits
            uint64_t format = temp & 0xF000000000000000; // upper 4 bits
            printf("0x%016lX\n  Opcode: 0x%lX\n Format: 0x%lX\n", temp, opcode, format);
        }
    }
    CPU ts;
    ts.test_print();

    // getline(in_stream, c);
    // cout << c << endl;
    // if (c.length() != 16)
    // {
    //     cout << c << "is not a valid instruction" << endl;
    //     exit(1);
    // }
    // string up = c.substr(0, 16);
    // string down = c.substr(16, 32);
    // cout << "up " << up << endl << "down " << down << endl;

    // cout << "Value in hex: 0x" << hex << test << endl;
    // test = test | stoull(down, nullptr, 16);
    // cout << "Value in hex: 0x" << hex << test << endl;
    in_stream.close();
    return 0;
}