#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <fstream>
#include <string>
#include <functional>
#include "symbol_table.hpp"

struct TextSectionDescriptor
{
    int LOCCTR_START;
    int numberBytesReadIn;
};

namespace FileHandling
{
    std::string readInBytes(std::ifstream& stream, int numBytes, bool readInHalfByte=false);
    std::ifstream openFile(const char* filename = nullptr);
    const std::string getProgramName(const char* assemblyFile);
    const SymbolEntries readSymbolTableFile(const char* filename);
    TextSectionDescriptor locateTextSection(std::ifstream& stream);
    int close(std::ifstream& inputFile, std::ofstream& outputFile);
}

#endif