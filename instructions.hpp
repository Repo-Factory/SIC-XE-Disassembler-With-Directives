#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <map>
#include <string>

#define NUM_INSTRUCTIONS 59
typedef std::string OpCode;

struct InstructionConstants
{
    const std::string ops[NUM_INSTRUCTIONS] = {
    "18", "58", "90", "40", "B4", "28",
    "88", "A0", "24", "64", "9C", "C4",
    "C0", "F4", "3C", "30", "34", "38",
    "48", "00", "68", "50", "70", "08",
    "6C", "74", "04", "D0", "20", "60",
    "98", "C8", "44", "D8", "AC", "4C",
    "A4", "A8", "F0", "EC", "0C", "78",
    "54", "80", "D4", "14", "7C", "E8",
    "84", "10", "1C", "5C", "94", "B0",
    "E0", "F8", "2C", "B8", "DC"
    };

    const std::string mnemonics[NUM_INSTRUCTIONS] = {
    "ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP",
    "COMPF", "COMPR", "DIV", "DIVF", "DIVR", "FIX",
    "FLOAT", "HIO", "J", "JEQ", "JGT", "JLT",
    "JSUB", "LDA", "LDB", "LDCH", "LDF", "LDL",
    "LDS", "LDT", "LDX", "LPS", "MUL", "MULF",
    "MULR", "NORM", "OR", "RD", "RMO", "RSUB",
    "SHIFTL", "SHIFTR", "SIO", "SSK", "STA", "STB",
    "STCH", "STF", "STI", "STL","STS", "STSW",
    "STT", "STX", "SUB", "SUBF", "SUBR", "SVC",
    "TD", "TIO", "TIX", "TIXR", "WD"
    };

    const bool format2[NUM_INSTRUCTIONS] = {
        false,false,true,false,true,false,
        false,true,false,false,true,false,
        false,false,false,false,false,false,
        false,false,false,false,false,false,
        false,false,false,false,false,false,
        true,false,false,false,true,false,
        true,true,false,false,false,false,
        false,false,false,false,false,false,
        false,false,false,false,true,true,
        false,false,false,true,false
    };   
};

struct InstructionDescription 
{
    std::string mnemonics;
    bool isFormat2;
};

/* We will have a hash map data member that ties our arrays together and provides a public API to be used */
class InstructionBindings
{
    public:
        InstructionBindings();
        std::string getMnemonic(const std::string& mnemonic);
        bool isFormat2(const std::string& mnemonic);
    private:
        InstructionConstants constants;
        std::map<OpCode, InstructionDescription> instructionBindings; // Tie our opcode to other info on the instruction 
};

#endif