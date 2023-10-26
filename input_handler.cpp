/*
 *  @brief
 *          Defines operations for input handling to keep it out of main code
 *
 *  There are simple operations like opening a file but we also have special use cases here for reading in lines,
 *  characters, or even a number of bytes. Wrapping these into easyily callable functions allows us to think more
 *  about the other logic in our program instead of thinking so hard about reading in exactly the right amount of input.
 */


#include "input_handler.hpp"
#include "byte_operations.hpp"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cstring>
#include <regex>
#include <cctype> 


#define FILE_OPEN_FAILURE_MESSAGE "Failed to open file: " 
#define TEXT_SECTION_IDENTIFIER 'T'
#define NUM_ADDRESS_DESCRIPTION_BYTES 3
#define LOOP_COUNTER_FINISH 0
#define REMAINING_TEXT_SECTION_BYTES 0

namespace // Reading From Input
{
    char readInChar(std::ifstream& input) 
    {
        char character;
        input.get(character);
        return character;
    }

    std::string readInLine(std::ifstream& input) 
    {
        std::string line;
        std::getline(input, line);  
        return line;
    }

    std::string readInByte(std::ifstream& stream) // Byte has two hex digits
    {
        std::string str;
        str += readInChar(stream);
        str += readInChar(stream);
        return str;
    }
}

std::ifstream FileHandling::openFile(const char* filename)
{
    std::ifstream sourceCode(filename);
    if (!sourceCode) {
        std::cerr << FILE_OPEN_FAILURE_MESSAGE << filename << std::endl;
        exit(EXIT_FAILURE);
    }
    return sourceCode;
}    

/* Our nice wrapper function that allows us to read in any number of bytes and even half bytes (one hex digit) if we want */
std::string FileHandling::readInBytes(std::ifstream& stream, int numBytes, bool readInHalfByte)
{
    std::string byteStream;
    while (numBytes-- > LOOP_COUNTER_FINISH)
    {
        byteStream += readInByte(stream);
    }
    if (readInHalfByte) byteStream += readInChar(stream);
    return byteStream;
}

int hexStringToInt(const std::string& str)
{
    int hexInt;
    std::istringstream converter(str);
    converter >> std::hex >> hexInt;
    return hexInt;
}

/* Looks for T section and grabs in description Bytes. Then we output the size in bytes of our text section to be used later to know how long to iterate */
TextSectionDescriptor FileHandling::locateTextSection(std::ifstream& stream)
{
    while (stream.peek() != TEXT_SECTION_IDENTIFIER && !stream.eof())                       // Read in lines until we see T
        readInLine(stream);
    readInChar(stream);                                                                     // Grab 'T'
    const int LOCCTR = hexStringToInt(readInBytes(stream, NUM_ADDRESS_DESCRIPTION_BYTES));  // Read in 3 descriptor bytes
    const int TEXT_SIZE = convertStringToHex(readInByte(stream));
    return TextSectionDescriptor{LOCCTR, TEXT_SIZE};                                        // Read in next byte and return the size it indicates
}   // This will place you at beginning of instructions

const std::string FileHandling::getProgramName(const char* assemblyFile)
{
    std::string programName;
    std::ifstream assembly = openFile(assemblyFile);
    readInChar(assembly); // Skip 'H' header indicator
    while (std::isdigit(assembly.peek()) == false) //!= numeric
    {
        programName += readInChar(assembly);
    }
    return programName;
}

int FileHandling::close(std::ifstream& inputFile, std::ofstream& outputFile)
{
    inputFile.close();
    outputFile.close();
    return EXIT_SUCCESS;
}