#ifndef DISASSEMBLY_H
#define DISASSEMBLY_H

#include "input_handler.hpp"
#include "output_handler.hpp"
#include "parser.hpp"

struct DisassemblerContext 
{
    std::ifstream& inputFile;
    std::ofstream& outputFile;
    const SymbolTable& symbolTable;
    const Parser& parser;
    int BASE_DIRECTIVE;
};

struct DisassemblerState
{
    const int LOCCTR; 
    const ParsedInstruction& instruction;
    const SymbolTable& table;
};

const SymbolEntries printHeader(const char* argv[], std::ofstream& outputFile);
int checkForSymbol(const int LOCCTR, const SymbolTable& symbolTable);
void outputSymbol(const DisassemblerContext& context, const int LOCCTR, const LITTAB_Entry& entry, std::ofstream& outputFile);

#endif