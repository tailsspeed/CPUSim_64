// g++ -o main main.cpp CPU.cpp ALU.cpp 
#include "CPU.h"


using Byte 	= uint8_t;
using Word 	= uint16_t;
using DWord = uint32_t;
using QWord = uint64_t;

//Variables
uint64_t reg[256]; //Our registers

//reg[0] is the zero register--should always be zero
//reg[1] is the accumulator
//reg[2] is base register
//reg[3] is our counter for things like loops
//reg[4] is for data
//reg[5] is for stack pointer
//reg[6] is for stack base pointer
//reg[7] is for destination index
//reg[8] is for source index
//reg[9] is for program counter/instruction pointer
//reg[10] is for instruction register
//reg[255] is for flags

/* Sample Functions for Set/Clear Flag Instructions */
/* carry flag */
void ClearCarryFlag()		// 0xF01
{
    reg[255] = reg[255] & 0xFFFFFFFFFFFFFFFE;
}

void SetCarryFlag()			// 0xF02
{
    reg[255] = reg[255] | 0x0000000000000001;
}

void ComplementCarryFlag()	// 0xF03
{
    reg[255] = reg[255] ^ 0x0000000000000001;
}

/* direction flag */
void ClearDirectionFlag()	// 0xF04
{
    reg[255] = reg[255] & 0xFFFFFFFFFFFFFBFF;
}

void SetDirectionFlag()		// 0xF05
{
    reg[255] = reg[255] | 0x0000000000000400;
}

/* interrupt flag */
void ClearInterruptFlag()	// 0xF06
{
    reg[255] = reg[255] & 0xFFFFFFFFFFFFFDFF;
}

void SetInterruptFlag()		// 0xF07
{
    reg[255] = reg[255] | 0x0000000000000200;
}

void ClearOverflowFlag()	// 0xF08
{
    reg[255] = reg[255] & 0xFFFFFFFFFFFFF7FF;
}

/* overflow flag */
void SetOverflowFlag()		// 0xF09
{
    reg[255] = reg[255] | 0x0000000000000800;
}

void ClearSignFlag()		// 0xF0A
{
    reg[255] = reg[255] & 0xFFFFFFFFFFFFFF7F;
}

/* sign flag */
void SetSignFlag()			// 0xF0B
{
    reg[255] = reg[255] | 0x0000000000000080;
}

void ClearZeroFlag()		// 0xF0C
{
    reg[255] = reg[255] & 0xFFFFFFFFFFFFFFBF;
}

/* zero flag */
void SetZeroFlag()			// 0xF0D
{
    reg[255] = reg[255] | 0x0000000000000040;
}

void ClearParityFlag()		// 0xF0E
{
    reg[255] = reg[255] & 0xFFFFFFFFFFFFFFFB;
}

/* parity flag */
void SetParityFlag()		// 0xF0F
{
    reg[255] = reg[255] | 0x0000000000000004;
}

/* to be used in ALU logical operations */
void SetZeroAndNegativeFlags(uint64_t Register)
{
	(Register == 0) ? SetZeroFlag() : ClearZeroFlag();
	(Register & 0x8000000000000000) ? SetSignFlag() : ClearSignFlag(); // Bit 63
}


//Memory
static constexpr uint64_t MAX_MEM = 0xFFFFFF; //Arbitrary amount of RAM. This is 16MB. Kept it low so it doesn't take long
Byte* Mem = (Byte*) malloc(MAX_MEM);

void LoadProgram(uint64_t instruction, int32_t Dst); //Specifically to load lines of code into memory

// using namespace std;

uint64_t Opcode(uint64_t instruction);

//RegisterToRegister related functions 0xA
uint64_t RegisterToRegister(uint64_t instruction);
uint64_t MOVRegister(int32_t Src, int32_t Dst); 	//0xA01
uint64_t ADDRegister(int32_t Src, int32_t Dst); 	//0xA02
uint64_t SUBRegister(int32_t Src, int32_t Dst); 	//0xA03
uint64_t MULRegister(int32_t Src, int32_t Dst); 	//0xA04
uint64_t DIVRegister(int32_t Src, int32_t Dst); 	//0xA05
uint64_t ANDRegister(int32_t Src, int32_t Dst); 	//0xA06
uint64_t ORRegister(int32_t Src, int32_t Dst);  	//0xA07
uint64_t INC(int32_t Dst);							//0xA08
uint64_t DEC(int32_t Dst);							//0xA09
uint64_t SHLRegister(int32_t Src, int32_t Dst); 	//0xA0A
uint64_t SHRRegister(int32_t Src, int32_t Dst); 	//0xA0B

//RegisterToMemory related functions 0xB
uint64_t RegisterToMemory(uint64_t instruction);
void WriteMem(int32_t Src, int32_t Dst);

uint64_t ReadMem(int32_t Dst);
uint64_t ReadQWord(int32_t Dst); //To check if I did it right

uint64_t MOVReg2Mem(int32_t Src, int32_t Dst);		//0xB01
uint64_t ADDReg2Mem(int32_t Src, int32_t Dst);		//0xB02
uint64_t SUBReg2Mem(int32_t Src, int32_t Dst);		//0xB03
uint64_t MULReg2Mem(int32_t Src, int32_t Dst);		//0xB04
uint64_t DIVReg2Mem(int32_t Src, int32_t Dst);		//0xB05

//MemoryToRegister related functions 0xC
uint64_t MemoryToRegister(uint64_t instruction);

uint64_t MOVMem2Reg(int32_t Src, int32_t Dst);		//0xC01
uint64_t ADDMem2Reg(int32_t Src, int32_t Dst);		//0xC02
uint64_t SUBMem2Reg(int32_t Src, int32_t Dst);		//0xC03
uint64_t MULMem2Reg(int32_t Src, int32_t Dst);		//0xC04
uint64_t DIVMem2Reg(int32_t Src, int32_t Dst);		//0xC05



//ImmediateToRegister related functions 0xD
uint64_t ImmediateToRegister(uint64_t instruction);
uint64_t MOVImmediate(uint64_t Imm, int32_t Sel);	//0xD01
uint64_t ADDImmediate(uint64_t Imm, int32_t Sel); 	//0xD02
uint64_t SUBImmediate(uint64_t Imm, int32_t Sel); 	//0xD03
uint64_t MULImmediate(uint64_t Imm, int32_t Sel); 	//0xD04
uint64_t DIVImmediate(uint64_t Imm, int32_t Sel); 	//0xD05
uint64_t ANDImmediate(uint64_t Imm, int32_t Sel); 	//0xD06
uint64_t ORImmediate(uint64_t Imm, int32_t Sel);  	//0xD07
uint64_t SHLImmediate(uint64_t Imm, int32_t Sel); 	//0xD0A (I'm keeping the lower part of opcode the same as Reg2Reg for consistency)
uint64_t SHRImmediate(uint64_t Imm, int32_t Sel); 	//0xD0B


//Jump related functions 0xE
uint64_t JUMPStuff(uint64_t instruction);
uint64_t JUMP(int32_t Dst); 						//0xE01

// Set/Clear Flag Instructions
void Flags(uint64_t instruction);					//0xFXX

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
    int32_t Dst = 0; //Memory destination
    while (getline(in_stream, line))
    {
        //printf("length %lu\n", line.length());
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
           	uint64_t temp = stoull(line, nullptr, 16);
           	LoadProgram(temp, Dst); 
           	Dst = Dst + 8;
        }
    }
    
    uint64_t Rez;
    while(((reg[255] >> 15) != 1) && reg[9] < Dst) // checks if Bit 15 is set--indicates HALT
    {
    	//Maybe use a register as another indication of when execution should stop
    	Rez = Opcode(ReadMem(reg[9]));
    	reg[9] = reg[9] + 8;
	}
    
    CPU ts;
    // ts.test_print();
    //DumpRegs();
	ReportNonZero();

    in_stream.close();
    
    /*cout <<  ReadQWord(0xFEED) << endl;
    cout << "Damn! " << int(Mem[0xFEED]) << int(Mem[0xFEED+1]) << endl;*/
    return 0;
}

uint64_t Opcode(uint64_t temp)
{
	uint64_t OpcodeResult = 0; //Result of whatever operation was done.
			try
            {
                /*
                stoull throws exception when argument is not in hex or exceeds range
                    ex. terminate called after throwing an instance of 'std::invalid_argument'
                        what():  stoull
                 */
                
                uint64_t full = temp; //Save the full instruction
                /* DECODE */
                uint64_t opcode = temp >> 52; // upper 12 bits of instruction
                uint64_t format = temp >> 60; // upper 4 bits of instruction
                printf("Instruction: 0x%016lX\n     Opcode: 0x%lX\n     Format: 0x%lX ", temp, opcode, format);
                uint64_t inst_type = (temp << 4) >> 56;
                switch (format)
                {
                case 0xA:
                    {
                        printf("Register->Register\n");
                        uint64_t dest = (temp << 12) >> 56;
                        uint64_t src = (temp << 20) >> 56;
                        printf("Type: 0x%lX\n    Dest: 0x%lX\n    Src: 0x%lX\n", inst_type, src, dest);
                        OpcodeResult = RegisterToRegister(full);
                        break;
                    }
                case 0xB:
                    {
                        printf("Register->Memory\n");
                        uint64_t src = (temp << 12) >> 56;
                        uint64_t dest_mem = (temp << 20) >> 32;
                        printf("Type: 0x%lX\n    Src: 0x%lX\n    Dest Mem: 0x%lX\n", inst_type, src, dest_mem);
                        OpcodeResult = RegisterToMemory(full);
                        break;
                    }
                    break;
                case 0xC:
                    {
                        printf("Memory->Register\n");
                        uint64_t src_mem = (temp << 12) >> 32;
                        uint64_t dest = (temp << 44) >> 56;
                        printf("Type: 0x%lX\n    Src Mem: 0x%lX\n    Dest: 0x%lX\n", inst_type, src_mem, dest);
                        OpcodeResult = MemoryToRegister(full);
                    }
                    break;
                case 0xD:
                    {
                        printf("Immediate->Register\n");
                        uint64_t imm = (temp << 12) >> 32;
                        uint64_t dest = (temp << 44) >> 56;
                        printf("Type: 0x%lX\n    Imm: 0x%lX\n    Dest: 0x%lX\n", inst_type, imm, dest);
                        OpcodeResult = ImmediateToRegister(full);
                    }
                    break;
                case 0xE:
                    {
                        printf("Jump\n");
                        uint64_t addr = (temp << 12) >> 32;
                        printf("Type: 0x%lX\n    Addr: 0x%lX\n", inst_type, addr);
                    }
                    break;
				case 0xF:
					{
						printf("Flag 0x%lX\n", temp);
						Flags(temp);
					}
					break;
                default:
                    printf("Invalid format\n");
                    break;
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << " - " << temp <<" cant be bothered to change this error " << '\n';
                
            }
	
	return OpcodeResult;
}


//Writing to Memory
void WriteMem(int32_t Src, int32_t Dst)
{
	uint64_t Cargo = reg[Src];
	Byte l7 = (Cargo >> 000) & 0xFF;  //Lol these are in octal. I found this off stack overflow.
	Byte l6 = (Cargo >> 010) & 0xFF;  //Hex would work too but it's funny looking at this.
	Byte l5 = (Cargo >> 020) & 0xFF;
	Byte l4 = (Cargo >> 030) & 0xFF;
	Byte l3 = (Cargo >> 040) & 0xFF;
	Byte l2 = (Cargo >> 050) & 0xFF;
	Byte l1 = (Cargo >> 060) & 0xFF;
	Byte l0 = (Cargo >> 070) & 0xFF;
	
	Mem[Dst] = l7;
	Mem[Dst+1] = l6;
	Mem[Dst+2] = l5;
	Mem[Dst+3] = l4;
	Mem[Dst+4] = l3;
	Mem[Dst+5] = l2;
	Mem[Dst+6] = l1;
	Mem[Dst+7] = l0;
	/*cout << "We are messing with register " << hex << Src << endl;
	cout << "Cargo is " << hex << Cargo << endl;
	cout << "Mem0 is " << hex << int(Mem[Dst]) << endl;
	cout << "Mem1 is " << hex << int(Mem[Dst+1]) << endl;
	cout << "Mem2 is " << hex << int(Mem[Dst+2]) << endl;
	cout << "Mem3 is " << hex << int(Mem[Dst+3]) << endl;
	cout << "Mem4 is " << hex << int(Mem[Dst+4]) << endl;
	cout << "Mem5 is " << hex << int(Mem[Dst+5]) << endl;
	cout << "Mem6 is " << hex << int(Mem[Dst+6]) << endl;
	cout << "Mem7 is " << hex << int(Mem[Dst+7]) << endl;*/
}

uint64_t ReadMem(int32_t Dst)
{
	uint64_t Cargo;
	Cargo =   ((uint64_t)Mem[Dst+7] << 56)
            | ((uint64_t)Mem[Dst+6] << 48)
            | ((uint64_t)Mem[Dst+5] << 40)
            | ((uint64_t)Mem[Dst+4] << 32)
            | ((uint64_t)Mem[Dst+3] << 24)
            | ((uint64_t)Mem[Dst+2] << 16)
            | ((uint64_t)Mem[Dst+1] <<  8)
            | ((uint64_t)Mem[Dst+0]);
	return Cargo;
}


//Specifically so I can read what's at a particular address
uint64_t ReadQWord(int32_t Dst)
{
    cout << "You are reading the memory at " << Dst << endl;
    uint64_t Cargo = 0;
    Cargo =   ((uint64_t)Mem[Dst+7] << 56)
            | ((uint64_t)Mem[Dst+6] << 48)
            | ((uint64_t)Mem[Dst+5] << 40)
            | ((uint64_t)Mem[Dst+4] << 32)
            | ((uint64_t)Mem[Dst+3] << 24)
            | ((uint64_t)Mem[Dst+2] << 16)
            | ((uint64_t)Mem[Dst+1] <<  8)
            | ((uint64_t)Mem[Dst+0]);
    return Cargo;
}

//A instructions
uint64_t RegisterToRegister(uint64_t instruction)
{
	uint64_t temp = (instruction & 0xFFF0000000000000) >> 52;	
	uint64_t TheRest = (instruction & 0x000FFFF000000000) >> 36;
	int32_t Src = (TheRest & 0xFF00) >> 8; //Source
	int32_t Dst = TheRest & 0xFF; //Destination
	
	
	uint64_t Reg2Reg = 0xF00F; //Putting this as F00F to warn that something went wrong 
	switch(temp)
	{
		case 0xA01:
			printf("MOV register instruction detected! \n");
			Reg2Reg = MOVRegister(Src,Dst);
			break;
		case 0xA02:
			printf("ADD register instruction detected! \n");
			Reg2Reg = ADDRegister(Src,Dst);
			break;
		case 0xA03:
			printf("SUB register instruction detected! \n");
			Reg2Reg = SUBRegister(Src,Dst);
			break;
		case 0xA04:
			printf("MUL register instruction detected! \n");
			Reg2Reg = MULRegister(Src,Dst);
			break;
		case 0xA05:
			printf("DIV register instruction detected! \n");
			Reg2Reg = DIVRegister(Src,Dst);
			break;
		case 0xA06:
			printf("AND register \n");
			Reg2Reg = ANDRegister(Src,Dst);
			break;
		case 0xA07:
			printf("OR register \n");
			Reg2Reg = ORRegister(Src,Dst);
			break;
		case 0xA08:
			printf("Increment register \n");
			Reg2Reg = INC(Dst);
			break;
		case 0xA09:
			printf("Decrement register \n");
			Reg2Reg = DEC(Dst);
			break;
		case 0xA0A:
			printf("SHL register \n");
			Reg2Reg = SHLRegister(Src,Dst);
			break;
		case 0xA0B:
			printf("SHR register \n");
			Reg2Reg = SHRRegister(Src,Dst);
			break;
		case 0xA0C:
			printf("HALT DETECTED!!!! \n");
			reg[255] = reg[255] | 0x0000000000008000; // set bit 15 instead for HALT
			// reg[255] = 1; //we'll make the first bit of this register to check for halting
			Reg2Reg = 0xDEAD;
			break;
		default:
			printf("Oops! Unimplemented instruction! \n");
	}
	
	return Reg2Reg;
}



uint64_t MOVRegister(int32_t Source, int32_t Destination)
{
	reg[Destination] = reg[Source];
	return reg[Destination];
}

uint64_t ADDRegister(int32_t Src, int32_t Dst)
{
	reg[Dst] = reg[Src] + reg[Dst];
	SetZeroAndNegativeFlags(reg[Dst]);
	return reg[Dst];
}

uint64_t SUBRegister(int32_t Src, int32_t Dst)
{
	reg[Dst] = reg[Src] - reg[Dst];
	SetZeroAndNegativeFlags(reg[Dst]);
	return reg[Dst];
}

uint64_t MULRegister(int32_t Src, int32_t Dst)
{
	reg[Dst] = reg[Src] * reg[Dst];
	SetZeroAndNegativeFlags(reg[Dst]);
	return reg[Dst];
}

uint64_t DIVRegister(int32_t Src, int32_t Dst)
{
	reg[Dst] = reg[Src] / reg[Dst];
	SetZeroAndNegativeFlags(reg[Dst]);
	return reg[Dst];
}

uint64_t ANDRegister(int32_t Src, int32_t Dst)
{
	reg[Dst] = reg[Src] and reg[Dst];
	SetZeroAndNegativeFlags(reg[Dst]);
	return reg[Dst];	
}

uint64_t ORRegister(int32_t Src, int32_t Dst)
{
	reg[Dst] = reg[Src] or reg[Dst];
	SetZeroAndNegativeFlags(reg[Dst]);
	return reg[Dst];
}

uint64_t INC(int32_t Dst)
{
	reg[Dst]++;
	SetZeroAndNegativeFlags(reg[Dst]);
	return reg[Dst];
}

uint64_t DEC(int32_t Dst)
{
	reg[Dst]--;
	SetZeroAndNegativeFlags(reg[Dst]);
	return reg[Dst];
}

uint64_t SHLRegister(int32_t Src, int32_t Dst)
{
	reg[Dst] = reg[Dst] << reg[Src];
	SetZeroAndNegativeFlags(reg[Dst]);
	return reg[Dst];
}

uint64_t SHRRegister(int32_t Src, int32_t Dst)
{
	reg[Dst] = reg[Dst] >> reg[Src];
	SetZeroAndNegativeFlags(reg[Dst]);
	return reg[Dst];
}






//B instructions
uint64_t RegisterToMemory(uint64_t instruction)
{
	//cout << "Instruction is " << hex << instruction << endl;
	uint64_t temp = (instruction & 0xFFF0000000000000) >> 52;	
	uint64_t S = (instruction & 0x000FF00000000000) >> 44;    //Source register
	uint64_t D = (instruction & 0x00000FFFFFFFF000) >> 12;  //Memory destination
	
	int32_t Src = S; //Just to match with the function call
	int32_t Dst = D; //I convert these to 32-bit
	//cout << "S is " << hex << S << endl;
	//cout << "D is " << hex << D << endl;
	
	uint64_t Reg2Mem = 0xF00F; //Putting this as F00F to warn that something went wrong 
	
	switch(temp)
	{
		case 0xB01:
			printf("MOV register to memory detected! \n");
			MOVReg2Mem(Src, Dst);
			break;
		case 0xB02:
			printf("ADD reg2mem detected! \n");
			ADDReg2Mem(Src, Dst);
			break;
		case 0xB03:
			printf("SUB reg2mem detected! \n");
			SUBReg2Mem(Src, Dst);
			break;
		case 0xB04:
			printf("MUL reg2mem detected! \n");
			MULReg2Mem(Src, Dst);
			break;
		default:
			printf("Oops! Unimplemented instruction! \n");
	}
	return Reg2Mem;
}

uint64_t MOVReg2Mem(int32_t Src, int32_t Dst)
{
	WriteMem(Src, Dst);
	return ReadMem(Dst);
}

uint64_t ADDReg2Mem(int32_t Src, int32_t Dst)
{
	reg[Src] = reg[Src] + ReadMem(Dst);
	WriteMem(Src, Dst);
	SetZeroAndNegativeFlags(Dst);
	return ReadMem(Dst);
}

uint64_t SUBReg2Mem(int32_t Src, int32_t Dst)
{
	reg[Src] = reg[Src] - ReadMem(Dst);
	WriteMem(Src, Dst);
	SetZeroAndNegativeFlags(Dst);
	return ReadMem(Dst);
}

uint64_t MULReg2Mem(int32_t Src, int32_t Dst)
{
	reg[Src] = reg[Src] * ReadMem(Dst);
	WriteMem(Src, Dst);
	SetZeroAndNegativeFlags(Dst);
	return ReadMem(Dst);
}

uint64_t DIVReg2Mem(int32_t Src, int32_t Dst)
{
	reg[Src] = reg[Src] / ReadMem(Dst);
	WriteMem(Src, Dst);
	SetZeroAndNegativeFlags(Dst);
	return ReadMem(Dst);
}

//C instrucitons
uint64_t MemoryToRegister(uint64_t instruction)
{
	uint64_t temp = (instruction & 0xFFF0000000000000) >> 52;
	uint64_t TheRest = (instruction & 0x000FFFFFFFFFF000) >> 12;
	
	int32_t Dst = TheRest & 0xFF; //Destination Registerr
	int32_t Mem = (TheRest & 0xFFFFFFFF00) >> 8; //Memory
	
	uint64_t Mem2Reg = 0xF00F; //Putting this as F00F to warn that something went wrong 
	
	switch(temp)
	{
		case 0xC01:
			printf("MOV mem2reg detected! \n");
			MOVMem2Reg(Mem, Dst);
			break;
		case 0xC02:
			printf("ADD mem2reg detected! \n");
			ADDMem2Reg(Mem, Dst);
			break;
		case 0xC03:
			printf("SUB mem2reg detected! \n");
			SUBMem2Reg(Mem, Dst);
			break; 
		case 0xC04:
			printf("MUL mem2reg detecte! \n");
			MULMem2Reg(Mem, Dst);
			break;
		case 0xD04:
			printf("DIV mem2reg detected! \n");
			DIVMem2Reg(Mem, Dst);
			break;
		default:
			printf("Oops! Unimplemented instruction! \n");
	}
	return Mem2Reg;
}

uint64_t MOVMem2Reg(int32_t Meme, int32_t Dst)
{
	reg[Dst] = ReadMem(Meme);
	//cout << "The value of register " << Dst << " is " << reg[Dst] << endl;
	return reg[Dst]; //Let's see if we got the right stuff
}


uint64_t ADDMem2Reg(int32_t Meme, int32_t Dst)
{
	reg[Dst] = ReadMem(Meme) + reg[Dst];
	SetZeroAndNegativeFlags(reg[Dst]);
	return reg[Dst];
}

uint64_t SUBMem2Reg(int32_t Meme, int32_t Dst)
{
	reg[Dst] = ReadMem(Meme) - reg[Dst];
	SetZeroAndNegativeFlags(reg[Dst]);
	return reg[Dst];
}

uint64_t MULMem2Reg(int32_t Meme, int32_t Dst)
{
	reg[Dst] = ReadMem(Meme) * reg[Dst];
	SetZeroAndNegativeFlags(reg[Dst]);
	return reg[Dst];
}

uint64_t DIVMem2Reg(int32_t Meme, int32_t Dst)
{
	reg[Dst] = ReadMem(Meme) / reg[Dst];
	SetZeroAndNegativeFlags(reg[Dst]);
	return reg[Dst];
}

//D instructions
uint64_t ImmediateToRegister(uint64_t instruction)
{
	uint64_t temp = (instruction & 0xFFF0000000000000) >> 52;	
	uint64_t TheRest = (instruction & 0x000FFFFFFFFFF000) >> 12;
	
	int32_t Sel = TheRest & 0xFF; //Selected Register
	uint64_t Imm = (TheRest & 0xFFFFFFFF00) >> 8; //Immediate
	//cout << "The selected register is " << SelectedRegister << endl;
	//cout << "The immediate is " << hex << Immediate << endl;
	
	uint64_t Imm2Reg = 0xF00F; //Putting this as F00F to warn that something went wrong 
		
	switch(temp)
	{
	case 0xD01:
		printf("MOV immediate instruction detected! \n");
		Imm2Reg = MOVImmediate(Imm, Sel);
		break;
	case 0xD02:
		printf("ADD immediate instruction detected! \n");
		Imm2Reg = ADDImmediate(Imm, Sel);
		break;
	case 0xD03:
		printf("SUB immediate instruction detected! \n");
		Imm2Reg = SUBImmediate(Imm, Sel);
		break;
	case 0xD04:
		printf("MUL immediate instruction detected! \n");
		Imm2Reg = MULImmediate(Imm, Sel);
		break;
	case 0xD05:
		printf("DIV immediate instruction detected! \n");
		Imm2Reg = DIVImmediate(Imm, Sel);
		break;
	case 0xD06:
		printf("AND immediate \n");
		Imm2Reg = ANDImmediate(Imm, Sel);
		break;
	case 0xD07:
		printf("OR immediate \n");
		Imm2Reg = ORImmediate(Imm, Sel);
		break;
	case 0xD0A:
		printf("SHL immediate \n");
		Imm2Reg = SHLImmediate(Imm, Sel);
		break;
	case 0xD0B:
		printf("SHR immediate \n");
		Imm2Reg = SHRImmediate(Imm, Sel);
		break;
	default:
		printf("Oops! Unimplemented instruction \n");
	}
	return Imm2Reg;
}

uint64_t MOVImmediate(uint64_t Imm, int32_t Sel)
{
	reg[Sel] = Imm;
	return reg[Sel];
}

uint64_t ADDImmediate(uint64_t Imm, int32_t Sel)
{
	reg[Sel] = reg[Sel] + Imm;
	SetZeroAndNegativeFlags(reg[Sel]);
	return reg[Sel];
}

uint64_t SUBImmediate(uint64_t Imm, int32_t Sel)
{
	reg[Sel] = reg[Sel] - Imm;
	SetZeroAndNegativeFlags(reg[Sel]);
	return reg[Sel];
}

uint64_t MULImmediate(uint64_t Imm, int32_t Sel)
{
	reg[Sel] = reg[Sel] * Imm;
	SetZeroAndNegativeFlags(reg[Sel]);
	return reg[Sel];
}

uint64_t DIVImmediate(uint64_t Imm, int32_t Sel)
{
	reg[Sel] = reg[Sel] / Imm;
	SetZeroAndNegativeFlags(reg[Sel]);
	return reg[Sel];
}

uint64_t ANDImmediate(uint64_t Imm, int32_t Sel)
{
	reg[Sel] = reg[Sel] and Imm;
	SetZeroAndNegativeFlags(reg[Sel]);
	return reg[Sel];
}

uint64_t ORImmediate(uint64_t Imm, int32_t Sel)
{
	reg[Sel] = reg[Sel] or Imm;
	SetZeroAndNegativeFlags(reg[Sel]);
	return reg[Sel];
}

uint64_t SHLImmediate(uint64_t Imm, int32_t Sel)
{
	reg[Sel] = reg[Sel] << Imm;
	SetZeroAndNegativeFlags(reg[Sel]);
	return reg[Sel];
}

uint64_t SHRImmediate(uint64_t Imm, int32_t Sel)
{
	reg[Sel] = reg[Sel] >> Imm;
	SetZeroAndNegativeFlags(reg[Sel]);
	return reg[Sel];
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
	for (int i = 11; i < 256; i++)
	{
		reg[i] = 0;
	}
}

//Function that prints general purpose registers that aren't 0
void ReportNonZero()
{
	for (int i = 11; i < 256; i++)
	{
		if(reg[i] != 0)
		{
			cout << "Register " << hex << i << " has a value of " << reg[i] << endl;
		}
	}
}

void LoadProgram(uint64_t Instruction, int32_t Dst)
{
	//This is the same as the WriteMem function except this time the Cargo 
	//is the instruction itself
	
	Byte l7 = (Instruction >> 000) & 0xFF;  //Lol these are in octal. I found this off stack overflow.
	Byte l6 = (Instruction >> 010) & 0xFF;  //Hex would work too but it's funny looking at this.
	Byte l5 = (Instruction >> 020) & 0xFF;
	Byte l4 = (Instruction >> 030) & 0xFF;
	Byte l3 = (Instruction >> 040) & 0xFF;
	Byte l2 = (Instruction >> 050) & 0xFF;
	Byte l1 = (Instruction >> 060) & 0xFF;
	Byte l0 = (Instruction >> 070) & 0xFF;
	
	Mem[Dst] = l7;
	Mem[Dst+1] = l6;
	Mem[Dst+2] = l5;
	Mem[Dst+3] = l4;
	Mem[Dst+4] = l3;
	Mem[Dst+5] = l2;
	Mem[Dst+6] = l1;
	Mem[Dst+7] = l0;
}

//E instrucitons
uint64_t JUMPStuff(uint64_t instruction)
{
	uint64_t temp = (instruction & 0xFFF0000000000000) >> 52;
	uint64_t Address = (instruction & 0xFFFFFFFF00000) >> 20;
	
	
	uint64_t Jump = 0xF00F; //Putting this as F00F to warn that something went wrong 
	
	switch(temp)
	{
		case 0xE01:
			printf("JUMP detected! \n");
			Jump = JUMP(Address);
			break;
		default:
			printf("Oops! Unimplemented instruction! \n");
	}
	return Jump;
}

uint64_t JUMP(int32_t Address)
{
	reg[9] = Address;
	return reg[9];
}

//F instructions
void Flags(uint64_t instruction)
{
	uint64_t temp = (instruction & 0xFFF0000000000000) >> 52;
	switch (temp)
	{
	case 0xF01: ClearCarryFlag();
		break;
	case 0xF02: SetCarryFlag();
		break;
	case 0xF03: ComplementCarryFlag();
		break;
	case 0xF04: ClearDirectionFlag();
		break;
	case 0xF05: SetDirectionFlag();
		break;
	case 0xF06: ClearInterruptFlag();
		break;
	case 0xF07: SetInterruptFlag();
		break;
	case 0xF08: ClearOverflowFlag();
		break;
	case 0xF09: SetOverflowFlag();
		break;
	case 0xF0A: ClearSignFlag();
		break;
	case 0xF0B: SetSignFlag();
		break;
	case 0xF0C: ClearZeroFlag();
		break;
	case 0xF0D: SetZeroFlag();
		break;
	case 0xF0E: ClearParityFlag();
		break;
	case 0xF0F: SetParityFlag();
		break;
	default:
		printf("Invalid flag format\n");
		break;
	}
}