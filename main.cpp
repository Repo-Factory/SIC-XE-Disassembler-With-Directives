/*******************************************************************************
 * @author Conner Sommerfield
 * @name disassembler
 * 
 * @details Reads instructions from hex file and outputs description of each
 * 
 * Program requires command line argument to path of hex file of SIC/XE program
 * 
 * Relevant entities
 * - input handler
 * - output handler
 * - parser : depends on byte_operations.hpp and instructions.hpp
 * See cpp file of each for more details in each respective area
 ******************************************************************************/

#include "input_handler.hpp"
#include "output_handler.hpp"
#include "parser.hpp"
#include <fstream>
#include <iostream>

////////////////////////////////////////////////////////////
const constexpr int INPUT_FILE_ARG_NUMBER = 1;
const constexpr int SYMBOL_FILE_ARG_NUMBER = 2;
const constexpr int ONE_BYTE = 1;
const constexpr int PLUS_HALF_BYTE = 1;
const constexpr int NUMBER_OF_HEX_CHARS_IN_ONE_BYTE = 2;
const constexpr char* OUTPUT_FILE_NAME = "out.lst";
const int BYTES_IN_HEX_STRING(const std::string& hex_str) 
{
    return hex_str.size() / NUMBER_OF_HEX_CHARS_IN_ONE_BYTE;
}
const constexpr bool STILL_MORE_BYTES(int bytes) 
{
    return bytes > 0;
}
///////////////////////////////////////////////////////////

struct DisassemblerState
{
    const int LOCCTR; 
    const ParsedInstruction& instruction;
    const SymbolTable& table;
};

/* Call our parser for all our info, print it, and return how many bytes we traversed */
ParsingResult parseInstruction(std::ifstream& inputFile, const std::unique_ptr<Parser>& parser)
{
    const std::string firstTwelveBits =           FileHandling::readInBytes(inputFile, ONE_BYTE, PLUS_HALF_BYTE);
    const std::string opCode =                    parser->determineOpCode(firstTwelveBits);
    const AddressingFormat format =               parser->determineFormat(firstTwelveBits);
    const AddressingMode addresingMode =          parser->determineAddressingMode(firstTwelveBits);
    const bool isIndexed =                        parser->isIndexed(firstTwelveBits);
    const TargetAddressMode targetAddressMode =   parser->determineTargetAddressMode(firstTwelveBits);
    const std::string objectCode =                parser->readInFullInstruction(inputFile, firstTwelveBits, format);
    const ParsedInstruction parsedInstruction     {opCode, format, addresingMode, isIndexed, targetAddressMode, objectCode};
    return ParsingResult                          {parsedInstruction, BYTES_IN_HEX_STRING(objectCode)} ;  // Return the total number of bytes traversed
}

using printToConsole = void;
printToConsole generateOutput(const DisassemblerState state, std::ofstream& outputFile)
{
    const std::string LOCCTR_OUTPUT     = CREATE_LOCCTR_OUTPUT(state.LOCCTR);
    const std::string SYMBOL_OUTPUT     = CREATE_SYMBOL_OUTPUT(state.LOCCTR, state.table);
    const std::string OPCODE_OUTPUT     = CREATE_OPCODE_OUTPUT(state.instruction.opCode, state.instruction.format);
    const std::string ADDRESS_OUTPUT    = CREATE_ADDRESS_OUTPUT(state.instruction.addresingMode, state.instruction.targetAddressMode);
    const std::string OBJECT_OUTPUT     = CREATE_OBJECT_OUTPUT(state.instruction.objectCode);
    outputFile                          << Output{LOCCTR_OUTPUT, SYMBOL_OUTPUT, OPCODE_OUTPUT, ADDRESS_OUTPUT, OBJECT_OUTPUT};
}

const SymbolEntries printHeader(const char* argv[], std::ofstream& outputFile)
{
    const SymbolEntries symbolEntries = FileHandling::readSymbolTableFile(argv[SYMBOL_FILE_ARG_NUMBER]);   
    const std::string programName = FileHandling::getProgramName(argv[INPUT_FILE_ARG_NUMBER]);
    const std::string startAddress = symbolEntries.SYMTAB[0].address;
    FileHandling::print_column_names(outputFile, programName, startAddress);
    return symbolEntries;
}

using bytesUsed = int;
bytesUsed checkForSymbol(const int LOCCTR, const SymbolTable& symbolTable, std::ofstream& outputFile)
{
    auto it = symbolTable.find(LOCCTR);
    if (it != symbolTable.end()) {
        const LITTAB_Entry entry = it->second;
        outputFile << Output{CREATE_LOCCTR_OUTPUT(LOCCTR), CREATE_SYMBOL_OUTPUT(LOCCTR, symbolTable), "BYTE", entry.lit_const, entry.lit_const.substr(2, std::stoi(entry.length))};
        return std::stoi(entry.length)/2;
    }
    return false;
}

// Set up input/output files and traverse input instructions
int main(const int argc, const char* argv[])
{
    std::ifstream inputFile = FileHandling::openFile(argv[INPUT_FILE_ARG_NUMBER]);
    std::ofstream outputFile(OUTPUT_FILE_NAME);                 
    const SymbolEntries symbolEntries = printHeader(argv, outputFile);
    const SymbolTable symbolTable = createTable(symbolEntries);
    const auto parser = std::unique_ptr<Parser>(new Parser());

    const std::function<void(int, int)> recurseTextSection ([&](const int textBytes, const int LOCCTR) {  // While there are still bytes, extract them
        if (STILL_MORE_BYTES(textBytes)) {
            //////////////SYMBOL////////////
            const int symbolBytesUsed = checkForSymbol(LOCCTR, symbolTable, outputFile);
            if (symbolBytesUsed > 0) {
                FileHandling::readInBytes(inputFile, symbolBytesUsed, 0);
                recurseTextSection(textBytes-symbolBytesUsed, LOCCTR+symbolBytesUsed);
                return;
            }
            //////////////INSTRUCTION////////////
            const ParsingResult parseResult = parseInstruction(inputFile, parser);
            generateOutput(DisassemblerState{LOCCTR, parseResult.instruction, symbolTable}, outputFile);           
            recurseTextSection(textBytes-parseResult.bytesReadIn, LOCCTR+parseResult.bytesReadIn);
        }
    });

    while (!inputFile.eof()) { // Here we will actually call our lambda, which will execute bulk of program
        const TextSectionDescriptor descriptor = FileHandling::locateTextSection(inputFile); 
        recurseTextSection(descriptor.numberBytesReadIn, descriptor.LOCCTR_START);  
    }   
    return FileHandling::close(inputFile, outputFile); 
}
