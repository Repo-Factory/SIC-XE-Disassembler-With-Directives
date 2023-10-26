#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>
#include <map>
#include <cstdint>

struct SYMTAB_Entry
{
    const std::string symbol;
    const std::string address;
    const std::string flags;
};

struct LITTAB_Entry
{
    const std::string name;
    const std::string lit_const;
    const std::string length;
    const std::string address;
};

struct SymbolEntries
{
    SymbolEntries(const std::vector<SYMTAB_Entry> SYMTAB, const std::vector<LITTAB_Entry> LITTAB) 
        : SYMTAB(SYMTAB), 
        LITTAB(LITTAB) 
    {}
    const std::vector<SYMTAB_Entry> SYMTAB;
    const std::vector<LITTAB_Entry> LITTAB;
};

using SymbolTable = std::map<const int, LITTAB_Entry>;
SymbolTable createTable(const SymbolEntries& symbolTable);
const bool addressPresent(const int LOCCTR, const SymbolTable& table);

#endif
