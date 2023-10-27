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
#include "disassembly.hpp"
#include "parser.hpp"
#include <fstream>
#include <iostream>

////////////////////////////////////////////////////////////
const constexpr int INPUT_FILE_ARG_NUMBER = 1;
const constexpr int SYMBOL_FILE_ARG_NUMBER = 2;
const constexpr int ONE_BYTE = 1;
const constexpr int NO_HALF_BYTE = 0;
const constexpr int NO_SYMBOL = 0;
const constexpr int PLUS_HALF_BYTE = 1;
const constexpr int NUMBER_OF_HEX_CHARS_IN_ONE_BYTE = 2;
const constexpr int INITIAL_BASE = 0;
const constexpr char* OUTPUT_FILE_NAME = "out.lst";
const constexpr bool FOUND_SYMBOL(int bytes) {return bytes > 0;}
const constexpr bool STILL_MORE_BYTES(int bytes) {return bytes > 0;}
const int BYTES_IN_HEX_STRING(const std::string& hex_str) {return hex_str.size() / NUMBER_OF_HEX_CHARS_IN_ONE_BYTE;}
///////////////////////////////////////////////////////////

/* Call our parser for all our info, print it, and return how many bytes we traversed */
ParsingResult parseInstruction(std::ifstream& inputFile, const Parser& parser)
{
    const std::string firstTwelveBits =           FileHandling::readInBytes(inputFile, ONE_BYTE, PLUS_HALF_BYTE);
    const std::string opCode =                    parser.determineOpCode(firstTwelveBits);
    const AddressingFormat format =               parser.determineFormat(firstTwelveBits);
    const AddressingMode addresingMode =          parser.determineAddressingMode(firstTwelveBits);
    const bool isIndexed =                        parser.isIndexed(firstTwelveBits);
    const TargetAddressMode targetAddressMode =   parser.determineTargetAddressMode(firstTwelveBits);
    const std::string objectCode =                parser.readInFullInstruction(inputFile, firstTwelveBits, format);
    const ParsedInstruction parsedInstruction     {opCode, format, addresingMode, isIndexed, targetAddressMode, objectCode};
    return ParsingResult                          {parsedInstruction, BYTES_IN_HEX_STRING(objectCode)} ;  // Return the total number of bytes traversed
}

using printToConsole = void;
printToConsole generateOutput(const DisassemblerState& state, const int bytesReadIn, std::ofstream& outputFile)
{
    const AddressingInfo ADDRESSMODES   {state.instruction.addresingMode, state.instruction.targetAddressMode};
    const OffsetInfo  OFFSETS           {0, state.LOCCTR + bytesReadIn};
    const std::string LOCCTR_OUTPUT     = CREATE_LOCCTR_OUTPUT(state.LOCCTR);
    const std::string SYMBOL_OUTPUT     = CREATE_SYMBOL_OUTPUT(state.LOCCTR, state.table);
    const std::string OPCODE_OUTPUT     = CREATE_OPCODE_OUTPUT(state.instruction.opCode, state.instruction.format);
    const std::string ADDRESS_OUTPUT    = CREATE_ADDRESS_OUTPUT(ADDRESSMODES, OFFSETS, state.instruction.objectCode);
    const std::string OBJECT_OUTPUT     = CREATE_OBJECT_OUTPUT(state.instruction.objectCode);
    outputFile                          << Output{LOCCTR_OUTPUT, SYMBOL_OUTPUT, OPCODE_OUTPUT, ADDRESS_OUTPUT, OBJECT_OUTPUT};
}

const int handleSymbol(const DisassemblerContext& context, const int LOCCTR)
{
    const LITTAB_Entry entry = context.symbolTable.find(LOCCTR)->second;
    const int labelBytes = std::stoi(entry.length)/2;
    FileHandling::readInBytes(context.inputFile, labelBytes, NO_HALF_BYTE);
    outputSymbol(context, LOCCTR, entry, context.outputFile);
    return labelBytes;
}

const int handleInstruction(DisassemblerContext& context, const int LOCCTR)
{
    const ParsingResult parseResult = parseInstruction(context.inputFile, context.parser);
    const DisassemblerState state = DisassemblerState{LOCCTR, parseResult.instruction, context.symbolTable};
    generateOutput(state, parseResult.bytesReadIn, context.outputFile);           
    return parseResult.bytesReadIn;
}

void recurseTextSection(DisassemblerContext& context, const int textBytesRemaining, const int LOCCTR) 
{  
    if (STILL_MORE_BYTES(textBytesRemaining)) {
        int bytesTraversed;
        if (checkForSymbol(LOCCTR, context.symbolTable)) {
            bytesTraversed = handleSymbol(context, LOCCTR);
        }
        else {
            bytesTraversed = handleInstruction(context, LOCCTR);
        }
        recurseTextSection(context, textBytesRemaining-bytesTraversed, LOCCTR+bytesTraversed);
    }
}

// Set up input/output files and traverse input instructions
int main(const int argc, const char* argv[])
{
    std::ifstream inputFile                 = FileHandling::openFile(argv[INPUT_FILE_ARG_NUMBER]);
    std::ofstream outputFile                (OUTPUT_FILE_NAME);                 
    const SymbolTable symbolTable           = createTable(printHeader(argv, outputFile));
    const Parser parser;
    DisassemblerContext                     context{inputFile, outputFile, symbolTable, parser, INITIAL_BASE};

    while (!inputFile.eof()) {
        const TextSectionDescriptor descriptor = FileHandling::locateTextSection(inputFile); 
        recurseTextSection(context, descriptor.textSectionSize, descriptor.LOCCTR_START);  
    }   

    return FileHandling::close(inputFile, outputFile); 
}
