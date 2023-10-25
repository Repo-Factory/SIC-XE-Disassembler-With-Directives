#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <fstream>
#include <string>
#include <functional>
#include "symbol_table.hpp"

namespace FileHandling
{
    std::string readInBytes(std::ifstream& stream, int numBytes, bool readInHalfByte=false);
    std::ifstream openFile(const char* filename = nullptr);
    const SymbolTable readSymbolTableFile(const char* filename);
    int locateTextSection(std::ifstream& stream);
    int close(std::ifstream& inputFile, std::ofstream& outputFile);
}

#endif