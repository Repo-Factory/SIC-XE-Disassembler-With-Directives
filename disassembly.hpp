#ifndef DISASSEMBLY_H
#define DISASSEMBLY_H

#include "input_handler.hpp"
#include "output_handler.hpp"
#include "parser.hpp"
#include <set>

struct ParsedInstruction;

struct DisassemblerContext 
{
    std::ifstream& inputFile;
    std::ofstream& outputFile;
    const SYMMAP& symmap;
    const LITMAP& litmap;
    const Parser& parser;
    int baseAddress;
};

struct DisassemblerState
{
    const int BASE;
    const int LOCCTR; 
    const ParsedInstruction& instruction;
    const SYMMAP& symmap;
    const LITMAP& litmap;
};

enum class Registers
{
    A =  0,
    X =  1,
    L =  2,
    B =  3,
    S =  4,
    T =  5,
    F =  6,
    PC = 8,
};

#endif