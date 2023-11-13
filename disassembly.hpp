#ifndef DISASSEMBLY_H
#define DISASSEMBLY_H

#include "input_handler.hpp"
#include "output_handler.hpp"
#include "parser.hpp"
#include <set>

struct ParsedInstruction;

using REGMAP = std::map<int32_t, std::string>;

struct DisassemblerContext 
{
    std::ifstream& inputFile;
    std::ofstream& outputFile;
    const REGMAP& registers;
    const SYMMAP& symmap;
    const LITMAP& litmap;
    const Parser& parser;
    int baseAddress;
    bool LTORG;
};

struct DisassemblerState
{
    const int BASE;
    const int LOCCTR; 
    const ParsedInstruction& instruction;
    const REGMAP& registers;
    const SYMMAP& symmap;
    const LITMAP& litmap;
};

#endif