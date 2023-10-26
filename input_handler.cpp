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
#define HEADER_SIZE 2
#define WHITE_SPACE_IDENTIFIER " "

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
    uint32_t hexInt;
    std::istringstream converter(str);
    converter >> std::hex >> hexInt;
    return hexInt;
}

/* Looks for T section and grabs in description Bytes. Then we output the size in bytes of our text section to be used later to know how long to iterate */
TextSectionDescriptor FileHandling::locateTextSection(std::ifstream& stream)
{
    while (stream.peek() != TEXT_SECTION_IDENTIFIER && !stream.eof())    // Read in lines until we see T
        readInLine(stream);
    readInChar(stream);                                                  // Grab 'T'
    const std::string LOCCTR_START = readInBytes(stream, NUM_ADDRESS_DESCRIPTION_BYTES); // Read in 3 descriptor bytes
    std::cout << LOCCTR_START << std::endl;

    const uint32_t LOCCTR = hexStringToInt(LOCCTR_START);
    const uint32_t TEXT_SIZE = convertStringToHex(readInByte(stream));
    return TextSectionDescriptor{LOCCTR, TEXT_SIZE};      // Read in next byte and return the size it indicates
    // This will place you at beginning of instructions
}

namespace
{
    const bool isNewLine(const std::string& line)
    {
        return line.find_first_not_of(WHITE_SPACE_IDENTIFIER) == std::string::npos;
    }

    const std::vector<std::string> GET_TOKENS(const std::string line)
    {
        std::string tokenHolder;
        std::vector<std::string> tokens;
        std::istringstream stream(line);
        while (stream >> tokenHolder) {
            tokens.push_back(tokenHolder);
        }
        return tokens;
    }

    std::ifstream advanceXLines(const uint32_t X, const char* filename)
    {
        uint32_t counter = X;
        std::string line;
        std::ifstream stream = FileHandling::openFile(filename);
        while (counter-- > 0)
        {
            std::getline(stream, line);
        }
        return stream;
    }

    const int GET_SYMTAB_START(const char* filename)
    {
        return HEADER_SIZE;
    }

    const int GET_LITTAB_START(const char* filename)
    {
        uint32_t counter = 0;
        std::string line;
        std::ifstream stream = FileHandling::openFile(filename);
        while (std::getline(stream, line))
        {
            if (isNewLine(line)) // Skip empty lines
                return counter+3;  
            counter++;
        }
        stream.close();
    }

    const int GET_SYMTAB_SIZE(const char* filename)
    {
        int32_t counter = 0;
        std::string line;
        std::ifstream stream = advanceXLines(GET_SYMTAB_START(filename), filename);
        while (std::getline(stream, line))
        {
            if (isNewLine(line)) {
                stream.close();
                return counter;  
            }
            counter++;
        }
        stream.close();
    }

    const int GET_LITTAB_SIZE(const char* filename)
    {
        uint32_t counter = 0;
        std::string line;
        std::ifstream stream = advanceXLines(GET_LITTAB_START(filename), filename);
        while (std::getline(stream, line))
        {
            counter++;
        }
        stream.close();
        return counter;
    }
    
    const SYMTAB_Entry CREATE_SYMTAB_ENTRY(const std::string line)
    {
        const std::vector<std::string> tokens = GET_TOKENS(line);
        return SYMTAB_Entry{tokens[0], tokens[1], tokens[2]};
    }

    const LITTAB_Entry CREATE_LITTAB_ENTRY(const std::string line)
    {
        const std::vector<std::string> tokens = GET_TOKENS(line);
        return LITTAB_Entry{tokens[0], tokens[1], tokens[2], tokens[3]};
    }

    const std::vector<SYMTAB_Entry> CREATE_SYMTAB(const int symtab_size, std::ifstream& stream)
    {
        std::vector<SYMTAB_Entry> symtab;
        std::string line;
        for (int i = 0; i < symtab_size; i++)
        {
            std::getline(stream, line);
            symtab.push_back(CREATE_SYMTAB_ENTRY(line));
        }
        return symtab;
    }

    const std::vector<LITTAB_Entry> CREATE_LITTAB(const int littab_size, std::ifstream& stream)
    {
        std::vector<LITTAB_Entry> littab;
        std::string line;
        for (int i = 0; i < littab_size; i++)
        {
            std::getline(stream, line);
            littab.push_back(CREATE_LITTAB_ENTRY(line));
        }
        return littab;
    }
}

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

const SymbolTable FileHandling::readSymbolTableFile(const char* filename)
{
    std::ifstream startSym = advanceXLines(GET_SYMTAB_START(filename), filename);
    std::ifstream startLit = advanceXLines(GET_LITTAB_START(filename), filename);
    return SymbolTable{CREATE_SYMTAB(GET_SYMTAB_SIZE(filename), startSym), CREATE_LITTAB(GET_LITTAB_SIZE(filename), startLit)};
}

int FileHandling::close(std::ifstream& inputFile, std::ofstream& outputFile)
{
    inputFile.close();
    outputFile.close();
    return EXIT_SUCCESS;
}