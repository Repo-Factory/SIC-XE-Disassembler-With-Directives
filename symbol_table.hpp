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

std::vector<LITTAB_Entry> GET_LITERALS(const SymbolEntries& symbolEntries);

using SYMMAP = std::map<const int, SYMTAB_Entry>;
using LITMAP = std::map<const int, LITTAB_Entry>;
struct SymbolTable
{
    const SYMMAP symmap;
    const LITMAP litmap;
};

LITMAP CREATE_LITMAP(const SymbolEntries& litmap);
SYMMAP CREATE_SYMMAP(const SymbolEntries& litmap);
bool checkForSymbol(const int LOCCTR, const LITMAP& litmap);
bool checkForSymbol(const int LOCCTR, const SYMMAP& symmap);

#endif
