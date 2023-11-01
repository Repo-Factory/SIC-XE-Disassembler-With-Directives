#ifndef DISASSEMBLY_H
#define DISASSEMBLY_H

#include "input_handler.hpp"
#include "output_handler.hpp"
#include "parser.hpp"

struct ParsedInstruction;

struct DisassemblerContext 
{
    std::ifstream& inputFile;
    std::ofstream& outputFile;
    const SymbolTable& symbolTable;
    const Parser& parser;
    int baseAdress;
};

struct DisassemblerState
{
    const int BASE;
    const int LOCCTR; 
    const ParsedInstruction& instruction;
    const SymbolTable& table;
};


#endif