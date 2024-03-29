#ifndef OUTPUT_HANDLER_H
#define OUTPUT_HANDLER_H

#include <fstream>
#include <fstream>
#include "parser.hpp"
#include "symbol_table.hpp"
#include "disassembly.hpp"

struct DisassemblerContext;
struct DisassemblerState;

namespace FileHandling
{
    std::ostream& print_column_names(std::ofstream& stream, const std::string& programName, const std::string& startAddress);
    void handleBaseDirective(const std::string& opcode, const std::string& objectCode, DisassemblerContext& context);
    std::ostream& printEnd(std::ofstream& stream, const std::string& programName);
}

const std::string prependString(const std::string& prependStr, const std::string& str);
const SymbolEntries printHeader(const char* argv[], std::ofstream& outputFile);
void outputSymbol(DisassemblerContext& context, const int LOCCTR, const LITTAB_Entry& entry, std::ofstream& outputFile);

struct AddressingInfo
{
    const AddressingMode addressingMode;
    const TargetAddressMode targetAddressMode;
    const bool is_indexed;
};

struct OffsetInfo
{
    const int BASE;
    const int PC;
};


struct ParsedInstruction
{
    const std::string opCode;
    const AddressingFormat format;
    const AddressingMode addresingMode;
    const bool isIndexed; 
    const TargetAddressMode targetAddressMode; 
    const std::string objectCode;
};

/* Consolidates our output into a single struct to overload our << operator */
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
const std::string CREATE_SYMBOL_OUTPUT(const int LOCCTR, const SYMMAP& symmap, const LITMAP& litmap);
const std::string CREATE_OPCODE_OUTPUT(const std::string& opcode, const AddressingFormat format);
const std::string CREATE_ADDRESS_OUTPUT(const AddressingInfo& addressingInfo, const OffsetInfo& offsetInfo, const DisassemblerState& state);
const std::string CREATE_OBJECT_OUTPUT(const std::string& objectCode);
void HANDLE_RESB_DIRECTIVE(const int32_t sectionGap, const int32_t LOCCTR, const DisassemblerContext& context);
void OUTPUT_LTORG(DisassemblerContext& context);

std::ostream& operator<<(std::ostream& stream, const AddressingMode addressingMode);

std::ostream& operator<<(std::ostream& stream, const Indexed wrapper);

std::ostream& operator<<(std::ostream& stream, const TargetAddressMode targetAddressMode);

std::ostream& operator<<(std::ostream& stream, const AddressingFormat instructionFormat);

std::ostream& operator<<(std::ostream& stream, const Output output);

#endif