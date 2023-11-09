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
#include "byte_operations.hpp"
#include <fstream>
#include <iostream>

////////////////////////////////////////////////////////////
using PrintToConsole = void;
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

// This function will use the current state variables like LOCCTR, pc counter, and the last read instruction
// to be able to output to our text file the correct information
PrintToConsole generateOutput(const DisassemblerState& state, const int bytesReadIn, std::ofstream& outputFile)
{
    const AddressingInfo ADDRESSMODES   {state.instruction.addresingMode, state.instruction.targetAddressMode};
    const OffsetInfo  OFFSETS           {state.BASE, state.LOCCTR + bytesReadIn};
    const std::string LOCCTR_OUTPUT     = CREATE_LOCCTR_OUTPUT(state.LOCCTR);
    const std::string SYMBOL_OUTPUT     = CREATE_SYMBOL_OUTPUT(state.LOCCTR, state.symmap, state.litmap);
    const std::string OPCODE_OUTPUT     = CREATE_OPCODE_OUTPUT(state.instruction.opCode, state.instruction.format);
    const std::string ADDRESS_OUTPUT    = CREATE_ADDRESS_OUTPUT(ADDRESSMODES, OFFSETS, state);
    const std::string OBJECT_OUTPUT     = CREATE_OBJECT_OUTPUT(state.instruction.objectCode);
    outputFile                          << Output{LOCCTR_OUTPUT, SYMBOL_OUTPUT, OPCODE_OUTPUT, ADDRESS_OUTPUT, OBJECT_OUTPUT};
}

// The disassembler will behave differently if it's a symbol instead of an instruction
// here it will have to look for an entry in the table and find and output the appropriate information
// like the handle instruction function it will also have to keep track of the amount of bytes traversed
const int handleSymbol(const DisassemblerContext& context, const int LOCCTR)
{
    const LITTAB_Entry entry = context.litmap.find(LOCCTR)->second;
    const int labelBytes = std::stoi(entry.length)/NUMBER_OF_HEX_CHARS_IN_ONE_BYTE;
    FileHandling::readInBytes(context.inputFile, labelBytes, NO_HALF_BYTE);
    outputSymbol(context, LOCCTR, entry, context.outputFile);
    return labelBytes;
}

// If no symbol is found will go through with our default behaviour which is to parsing instruction and output the disassembled code
const int handleInstruction(DisassemblerContext& context, const int LOCCTR)
{
    const ParsingResult parseResult = parseInstruction(context.inputFile, context.parser);
    const DisassemblerState state = DisassemblerState{context.baseAddress, LOCCTR, parseResult.instruction, context.symmap, context.litmap};
    generateOutput(state, parseResult.bytesReadIn, context.outputFile); 
    FileHandling::handleBaseDirective(parseResult.instruction.opCode, parseResult.instruction.objectCode, context);
    return parseResult.bytesReadIn;
}
 
// This recursive function will allow us to avoid mutable state while looping through text file.
// It will Naturally keep track of the LOCCTR because the handle symbol and handle instruction functions return bytes traversed
void recurseTextSection(DisassemblerContext& context, const int textBytesRemaining, const int LOCCTR) 
{  
    if (STILL_MORE_BYTES(textBytesRemaining)) {
        int bytesTraversed;
        if (checkForSymbol(LOCCTR, context.litmap)) {
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
    const auto symbolEntries                = printHeader(argv, outputFile);          
    const auto literals                     = GET_LITERALS(symbolEntries);
    const LITMAP litmap                     = CREATE_LITMAP(symbolEntries);
    const SYMMAP symmap                     = CREATE_SYMMAP(symbolEntries);
    const Parser parser;
    DisassemblerContext                     context{inputFile, outputFile, symmap, litmap, parser, INITIAL_BASE};

    while (!inputFile.eof()) {
        const TextSectionDescriptor descriptor = FileHandling::locateTextSection(inputFile); 
        recurseTextSection(context, descriptor.textSectionSize, descriptor.LOCCTR_START);  
    }   

    FileHandling::printEnd(outputFile, FileHandling::getProgramName(argv[INPUT_FILE_ARG_NUMBER]));
    return FileHandling::close(inputFile, outputFile); 
}
