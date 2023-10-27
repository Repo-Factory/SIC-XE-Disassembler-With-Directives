#include "disassembly.hpp"

const constexpr int INPUT_FILE_ARG_NUMBER = 1;
const constexpr int SYMBOL_FILE_ARG_NUMBER = 2;
const constexpr char* BYTE_DIRECTIVE = "BYTE";
const constexpr char* BASE_DIRECTIVE_CODE = "LDB";

const SymbolEntries printHeader(const char* argv[], std::ofstream& outputFile)
{
    const SymbolEntries symbolEntries = FileHandling::readSymbolTableFile(argv[SYMBOL_FILE_ARG_NUMBER]);   
    const std::string programName = FileHandling::getProgramName(argv[INPUT_FILE_ARG_NUMBER]);
    const std::string startAddress = symbolEntries.SYMTAB[0].address;
    FileHandling::print_column_names(outputFile, programName, startAddress);
    return symbolEntries;
}

void outputSymbol(const DisassemblerContext& context, const int LOCCTR, const LITTAB_Entry& entry, std::ofstream& outputFile)
{
    outputFile << 
    Output
    {
        CREATE_LOCCTR_OUTPUT(LOCCTR), 
        CREATE_SYMBOL_OUTPUT(LOCCTR, context.symbolTable), 
        BYTE_DIRECTIVE, 
        entry.lit_const, 
        entry.lit_const.substr(2, std::stoi(entry.length))
    };
}

// void handleBase(DisassemblerContext& context, const ParsingResult parseResult)
// {
//     if (parseResult.instruction.opCode == BASE_DIRECTIVE_CODE)
//     context.BASE_DIRECTIVE = parseResult.instruction.objectCode;
// }

using bytesUsed = int;
bytesUsed checkForSymbol(const int LOCCTR, const SymbolTable& symbolTable)
{
    auto it = symbolTable.find(LOCCTR);
    if (it != symbolTable.end()) {
        return true;
    }
    return false;
}
