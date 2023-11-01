/* 
 * @brief Handle populating our symbol table from sym.obj 
 *
 * There are Symbol Entries and Symbol Table structs defined in header which will hold each entry (line).
 * We will use these to make a hash map where we map addresses to appropriate info
 */

#include "symbol_table.hpp"
#include "input_handler.hpp"
#include "byte_operations.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#define HEADER_SIZE 2
#define WHITE_SPACE_IDENTIFIER " "

namespace
{
    const bool isNewLine(const std::string& line)
    {
        return line.find_first_not_of(WHITE_SPACE_IDENTIFIER) == std::string::npos;
    }

    /* 
     * Convert line to vector of strings so we can reference each word separately
     */
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

    /* 
     * Place file pointer at line X
     */
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

    /* 
     * Return line number where respective table starts
     */
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
            if (isNewLine(line)) { // Skip empty lines
                stream.close();
                return counter+3;  
            }
            counter++;
        }
        exit(EXIT_FAILURE);
    }

    /* 
     * We will count the size of each table
     */
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
        exit(EXIT_FAILURE);
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
    
    /* 
     * Tokenize each line to store each word into appropriate field of struct
     */
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

    /* 
     * These functions will start at the first line of the appropriate table, then iterate through the table using its size
     * and store every entry as a struct into a vector
     */
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

/* Store our SymbolEntries into data structure */
const SymbolEntries FileHandling::readSymbolTableFile(const char* filename)
{
    std::ifstream startSym = advanceXLines(GET_SYMTAB_START(filename), filename);
    std::ifstream startLit = advanceXLines(GET_LITTAB_START(filename), filename);
    return SymbolEntries{CREATE_SYMTAB(GET_SYMTAB_SIZE(filename), startSym), CREATE_LITTAB(GET_LITTAB_SIZE(filename), startLit)};
}

/* Rearrange our data structure into a map to find symbols and their info easily from current LOCCTR */
SymbolTable createTable(const SymbolEntries& symbolEntries)
{
    SymbolTable symbolTableMap;
    for (const auto& entry : symbolEntries.LITTAB)
    {
        symbolTableMap.insert({hexStringToInt(entry.address), entry});
    }
    return symbolTableMap;
}

const bool addressPresent(const int LOCCTR, const SymbolTable& table)
{
    auto it = table.find(LOCCTR);
    if (it != table.end()) {
        return true;
    }
    return false;
}
