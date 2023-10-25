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

#define INPUT_FILE_ARG_NUMBER 1
#define SYMBOL_FILE_ARG_NUMBER 2
#define ONE_BYTE 1
#define PLUS_HALF_BYTE 1
#define NUMBER_OF_HEX_CHARS_IN_ONE_BYTE 2
#define HEX_CHARS_IN_STRING(hex_str) hex_str.size()
#define BYTES_IN_HEX_STRING(hex_str) HEX_CHARS_IN_STRING(hex_str)/NUMBER_OF_HEX_CHARS_IN_ONE_BYTE // Assuming string is a stream of hex chars
#define STILL_MORE_BYTES(bytes) bytes>0
#define OUTPUT_FILE_NAME "out.lst"

/* Call our parser for all our info, print it, and return how many bytes we traversed */
int extractInfoToOutput(std::ifstream& inputFile, std::ofstream& outputFile, const std::unique_ptr<Parser>& parser)
{
    const std::string firstTwelveBits =           FileHandling::readInBytes(inputFile, ONE_BYTE, PLUS_HALF_BYTE);
    const std::string opCode =                    parser->determineOpCode(firstTwelveBits);
    const AddressingFormat format =               parser->determineFormat(firstTwelveBits);
    const AddressingMode addresingMode =          parser->determineAddressingMode(firstTwelveBits);
    const bool isIndexed =                        parser->isIndexed(firstTwelveBits);
    const TargetAddressMode targetAddressMode =   parser->determineTargetAddressMode(firstTwelveBits);
    const std::string fullInstruction =           parser->readInFullInstruction(inputFile, firstTwelveBits, format);
    // outputFile <<                                 Output{opCode, format, addresingMode, isIndexed, targetAddressMode, fullInstruction};
    return BYTES_IN_HEX_STRING(fullInstruction);  // Return the total number of bytes traversed
}

// Set up input/output files and traverse input instructions
int main(int argc, char* argv[])
{
    std::ifstream inputFile = FileHandling::openFile(argv[INPUT_FILE_ARG_NUMBER]);
    const SymbolTable symbolFile = FileHandling::readSymbolTableFile(argv[SYMBOL_FILE_ARG_NUMBER]);
    // for (auto s : symbolFile.SYMTAB) std::cout << s.symbol << std::endl;
    // for (auto s : symbolFile.LITTAB) std::cout << s.name << std::endl;

    std::ofstream outputFile(OUTPUT_FILE_NAME);                 
    FileHandling::print_column_names(outputFile);
    const auto parser = std::unique_ptr<Parser>(new Parser());
    const std::function<void(int)> recurseTextSection ([&](int textBytes) {  // While there are still bytes, extract them
        if (STILL_MORE_BYTES(textBytes)) recurseTextSection(textBytes-extractInfoToOutput(inputFile, outputFile, parser));
    });
    while (!inputFile.eof()) recurseTextSection(FileHandling::locateTextSection(inputFile));  // Here we will actually call our lambda, which will execute bulk of program
    return FileHandling::close(inputFile, outputFile); 
}