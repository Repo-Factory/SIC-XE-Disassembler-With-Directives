#ifndef OUTPUT_HANDLER_H
#define OUTPUT_HANDLER_H

#include <fstream>
#include <fstream>
#include "parser.hpp"
#include "symbol_table.hpp"

namespace FileHandling
{
    std::ostream& print_column_names(std::ofstream& stream, const std::string& programName, const std::string& startAddress);
}

/* Consolidates our output into a single struct to overload our << operator */

struct ParsedInstruction
{
    const std::string opCode;
    const AddressingFormat format;
    const AddressingMode addresingMode;
    const bool isIndexed; 
    const TargetAddressMode targetAddressMode; 
    const std::string objectCode;
};

struct Output
{
    const std::string LOCCTR;
    const std::string symbol;
    const std::string opcode;
    const std::string value;
    const std::string objectCode;
};

struct ParsingResult
{
    const ParsedInstruction instruction;
    const int bytesReadIn;
};

/* Wrapper so we can operator overload our bool value */
struct Indexed
{
    const bool indexed; 
};

const std::string CREATE_LOCCTR_OUTPUT(const int LOCCTR);
const std::string CREATE_SYMBOL_OUTPUT(const int LOCCTR, const SymbolTable& table);
const std::string CREATE_OPCODE_OUTPUT(const std::string& opcode, const AddressingFormat format);
const std::string CREATE_ADDRESS_OUTPUT(const AddressingMode addressingMode, const TargetAddressMode targetAddressMode);
const std::string CREATE_OBJECT_OUTPUT(const std::string& objectCode);
const std::string prependString(const std::string& prependStr, const std::string& str);


std::ostream& operator<<(std::ostream& stream, const AddressingMode addressingMode);

std::ostream& operator<<(std::ostream& stream, const Indexed wrapper);

std::ostream& operator<<(std::ostream& stream, const TargetAddressMode targetAddressMode);

std::ostream& operator<<(std::ostream& stream, const AddressingFormat instructionFormat);

std::ostream& operator<<(std::ostream& stream, const Output output);

#endif