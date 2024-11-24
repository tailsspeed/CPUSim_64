// g++ -o main main.cpp CPU.cpp ALU.cpp 
#include "CPU.h"


//Variables
uint64_t reg[256]; //Our registers


// using namespace std;

uint64_t Opcode(string instruction);

//RegisterToRegister related functions 0xA
uint64_t RegisterToRegister(uint64_t instruction);
uint64_t MOVRegister(int Src, int Dst); //0xA01
uint64_t ADDRegister(int Src, int Dst); //0xA02


//ImmediateToRegister related functions 0xD
uint64_t ImmediateToRegister(uint64_t instruction); //Opcode D
uint64_t MOVImmediate(uint64_t Immediate, int SelectedRegister);
uint64_t ADDImmediate(uint64_t Immediate, int SelectedRegister);


//void DumpRegs();
void InitRegs();
void ReportNonZero();


int main(int argc, char const *argv[])
{
	InitRegs();
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
        if (line.length() > 16)
        {
            // cout << line << " is not a valid instruction" << endl;
            printf("%s is too many hex digits\n", line.c_str()); // .c_str() returns pointer to C-style string
        }
		else if(line.length() < 16)
		{
			printf("%s is too few hex digits\n", line.c_str());	
		}
        else
        {
           Opcode(line); 
        }
    }
    CPU ts;
    // ts.test_print();
    //DumpRegs();
	ReportNonZero();

    in_stream.close();
    return 0;
}

uint64_t Opcode(string instruction)
{
	uint64_t OpcodeResult=0; //Result of whatever operation was done.
			try
            {
                /*
                stoull throws exception when argument is not in hex or exceeds range
                    ex. terminate called after throwing an instance of 'std::invalid_argument'
                        what():  stoull
                 */
                uint64_t temp = stoull(instruction, nullptr, 16);
                uint64_t full = temp; //Save the full instruction
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
                        uint64_t dest = (temp << 12) >> 56;
                        uint64_t src = (temp << 20) >> 56;
                        printf("Type: 0x%lX\n    Dest: 0x%lX\n    Src: 0x%lX\n", inst_type, src, dest);
                        OpcodeResult=RegisterToRegister(full);
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
                        OpcodeResult=ImmediateToRegister(full);
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
                std::cerr << e.what() << " - " << instruction <<" could not be converted or exceeds 64-bit range" << '\n';
                // printf("%s could not be converted or exceeds 64-bit range\n", line.c_str());
            }
	
	return OpcodeResult;
}

//A instructions
uint64_t RegisterToRegister(uint64_t instruction)
{
	uint64_t temp = (instruction & 0xFFF0000000000000)>>52;	
	uint64_t TheRest = (instruction & 0x000FFFF000000000) >> 36;
	int Src = (TheRest & 0xFF00) >> 8; //Source
	int Dst = TheRest & 0xFF; //Destination
	
	
	uint64_t Reg2Reg=0; //Whatever value was transferred to the register
	switch(temp)
	{
		case  0xA01:
			printf("MOV register instruction detected! \n");
			Reg2Reg=MOVRegister(Src,Dst);
			break;
		case 0xA02:
			printf("ADD register instruction detected! \n");
			Reg2Reg=ADDRegister(Src,Dst);
			break;
		default:
			printf("Oops! Unimplemented instruction! \n");
	}
	
	return Reg2Reg;
}



uint64_t MOVRegister(int Source, int Destination)
{
	reg[Destination]=reg[Source];
	return reg[Destination];
}

uint64_t ADDRegister(int Src, int Dst)
{
	reg[Dst]=reg[Src]+reg[Dst];
	return reg[Dst];
}


//D instructions
uint64_t ImmediateToRegister(uint64_t instruction)
{
	uint64_t temp = (instruction & 0xFFF0000000000000)>>52;	
	uint64_t TheRest = (instruction & 0x000FFFFFFFFFF000) >> 12;
	
	int Sel = TheRest & 0xFF; //Selected Register
	uint64_t Imm = (TheRest & 0xFFFFFFFF00) >> 8; //Immediate
	//cout << "The selected register is " << SelectedRegister << endl;
	//cout << "The immediate is " << hex << Immediate << endl;
	
	uint64_t Imm2Reg=0; //Whatever value was transferred to the register
		
	switch(temp)
	{
	case 0xD01:
		printf("MOV immediate instruction detected! \n");
		Imm2Reg=MOVImmediate(Imm, Sel);
		break;
	case 0xD02:
		printf("ADD immediate instruction detected! \n");
		Imm2Reg=ADDImmediate(Imm, Sel);
		break;
	default:
		printf("Oops! Unimplemented instruction \n");
	}
	return Imm2Reg;
}

uint64_t MOVImmediate(uint64_t Immediate, int SelectedRegister)
{
	reg[SelectedRegister] = Immediate;
	return Immediate;
}

uint64_t ADDImmediate(uint64_t Immediate, int SelectedRegister)
{
	reg[SelectedRegister] = reg[SelectedRegister] + Immediate;
	return reg[SelectedRegister];
}

/* Trash
void DumpRegs()
{
	cout << "Register contents:";
	for(int i=0; i<32; i++)
	{
		for(int j=0; j<8; j++)
		{
			cout << " R " << j*(i+0) << hex << reg[j*(i+0)]; 
			cout << " R " << j*(i+1) << hex << reg[j*(i+1)]; 
			cout << " R " << j*(i+2) << hex << reg[j*(i+2)]; 
			cout << " R " << j*(i+3) << hex << reg[j*(i+3)]; 
			cout << " R " << j*(i+4) << hex << reg[j*(i+4)]; 
			cout << " R " << j*(i+5) << hex << reg[j*(i+5)]; 
			cout << " R " << j*(i+6) << hex << reg[j*(i+6)];
			cout << " R " << j*(i+7) << hex << reg[j*(i+7)];  
		}
		cout << endl;
	}
}*/

void InitRegs()
{
	//Initialize all registers after 0xA to zero
	for (int i=11; i<256; i++)
	{
		reg[i]=0;
	}
}

//Function that prints general purpose registers that aren't 0
void ReportNonZero()
{
	for (int i=11; i<256; i++)
	{
		if(reg[i]!=0)
		{
			cout << "Register " << i << " has a value of " << hex << reg[i] << endl;
		}
	}
}