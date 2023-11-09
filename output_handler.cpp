/*
 *  @brief
 *          Organizes functions to output answer
 *
 *  Defines a series of operation overloaded functions to print all of our various fields
 */

#include "output_handler.hpp"
#include "byte_operations.hpp"
#include <iostream>
#include <vector>
#include <sstream>

const constexpr int COLUMN_SPACING =  10;
const constexpr char* IMMEDIATE_INDICATOR =  "#";
const constexpr char* INDIRECT_INDICATOR =  "@";
const constexpr char* FORMAT_4_INDICATOR =  "+";
const constexpr int NUMBER_PRINTOUT_SIZE =  4;
const constexpr char* LDB_INSTRUCTION =  "LDB";
const constexpr char* NUMBER_PADDING =  "0";
const constexpr char* EMPTY_STRING =  "";
const constexpr char* SPACE =  " ";
const constexpr char* START_DIRECTIVE =  "START";
const constexpr char* END_DIRECTIVE =  "END";
const constexpr char* BASE_DIRECTIVE =  "BASE";
const constexpr char* FIRST_DIRECTIVE =  "FIRST";
const constexpr int INPUT_FILE_ARG_NUMBER = 1;
const constexpr int SYMBOL_FILE_ARG_NUMBER = 2;
const constexpr char* BYTE_DIRECTIVE = "BYTE";
const constexpr char* BASE_DIRECTIVE_CODE = "LDB";

/********************************************************* 
 *                  FORMATTING HELPERS                   *
 *********************************************************/
const std::string XSpaces(const int X)
{
    std::string space;
    int counter = X;
    while (counter-- > 0)
        space += SPACE;
    return space;
}

const std::string pad(const std::string& word)
{
    if (word.size() == NUMBER_PRINTOUT_SIZE)
        return word;
    else if (word.size() > NUMBER_PRINTOUT_SIZE)
        return word.substr(word.size()-NUMBER_PRINTOUT_SIZE, word.size());
    std::string paddedString = word;
    while (paddedString.size() < NUMBER_PRINTOUT_SIZE)
        paddedString = prependString(NUMBER_PADDING, paddedString);
    return paddedString;
}

const std::string appendWord(const std::string& word)
{
    return word + XSpaces(COLUMN_SPACING - word.size());
}

const std::string prependString(const std::string& prependStr, const std::string& str)
{
    return prependStr + str;
}
/*********************************************************/

/********************************************************* 
 *                        OUTPUT                         *
 *********************************************************/
// Simple helper function that will retrieve the appropriate information to print the column names
const SymbolEntries printHeader(const char* argv[], std::ofstream& outputFile)
{
    const SymbolEntries symbolEntries = FileHandling::readSymbolTableFile(argv[SYMBOL_FILE_ARG_NUMBER]);   
    const std::string programName = FileHandling::getProgramName(argv[INPUT_FILE_ARG_NUMBER]);
    const std::string startAddress = symbolEntries.SYMTAB[0].address;
    FileHandling::print_column_names(outputFile, programName, startAddress);
    return symbolEntries;
}

// This will be create the appropriate output for a symbol
void outputSymbol(const DisassemblerContext& context, const int LOCCTR, const LITTAB_Entry& entry, std::ofstream& outputFile)
{
    outputFile << 
    Output
    {
        CREATE_LOCCTR_OUTPUT(LOCCTR), 
        CREATE_SYMBOL_OUTPUT(LOCCTR, context.litmap), 
        BYTE_DIRECTIVE, 
        entry.lit_const, 
        entry.lit_const.substr(2, std::stoi(entry.length))
    };
}

// First row to print
std::ostream& FileHandling::print_column_names(std::ofstream& stream, const std::string& programName, const std::string& startAddress)
{
    return stream   
    << appendWord(pad(startAddress)) 
    << appendWord(programName)  
    << appendWord(START_DIRECTIVE) 
    << appendWord(NUMBER_PADDING) 
    << std::endl; 
}

// Last row to print
std::ostream& FileHandling::printEnd(std::ofstream& stream, const std::string& programName)
{
    return stream   
    << appendWord(EMPTY_STRING) 
    << appendWord(EMPTY_STRING)  
    << appendWord(END_DIRECTIVE) 
    << appendWord(programName) 
    << std::endl; 
}

// This will handle the unique output for a BASE_DIRECTIVE
void FileHandling::handleBaseDirective(const std::string& opcode, const std::string& objectCode, DisassemblerContext& context)
{
    if (opcode == LDB_INSTRUCTION)
    {
        context.baseAddress = hexStringToInt(objectCode.substr(3, objectCode.size()));
        context.outputFile 
        << appendWord(EMPTY_STRING)
        << appendWord(EMPTY_STRING)
        << appendWord(BASE_DIRECTIVE) 
        << appendWord(pad(intToHexString(context.baseAddress)))  
        << std::endl;
    }          
}

/* Function called from main to print the answer for the output */
std::ostream& operator<<(std::ostream& stream, const Output output)
{
    return stream  
    << appendWord(pad(output.LOCCTR))  
    << appendWord(output.symbol)  
    << appendWord(output.opcode)  
    << appendWord(output.value) 
    << appendWord(output.objectCode)  
    << std::endl; 
}
/*********************************************************/

/************************************************************ 
 * PERFORM OPERATIONS TO DETERMINE CORRECT STRING TO OUTPUT *
 ************************************************************/

// Take current LOCCTR and convert to hex string
const std::string CREATE_LOCCTR_OUTPUT(const int LOCCTR)
{
    return intToHexString(LOCCTR);
}

// Search Our Symbol table
const std::string CREATE_SYMBOL_OUTPUT(const int LOCCTR, const LITMAP& table)
{
    if (LOCCTR == 0) 
        return FIRST_DIRECTIVE;
    auto it = table.find(LOCCTR);
    if (it != table.end()) {
        return it->second.name;
    }
    return EMPTY_STRING;
}

// Will determine if any assembler information needs to be appended/prepended to opcode
const std::string CREATE_OPCODE_OUTPUT(const std::string& opcode, const AddressingFormat format)
{
    if (format == AddressingFormat::Format4) 
        return prependString(FORMAT_4_INDICATOR, opcode);
    return opcode;
}

namespace //Helpers to construct address
{
    // Will add BC or Base to displacement for our address
    const std::string getAddress(const TargetAddressMode targetAddressMode, const std::string& objectCode, const OffsetInfo& offsetInfo)
    {
        const std::string address = objectCode.substr(3, objectCode.size()-1); // Last 3 digits of object code
        if (targetAddressMode == TargetAddressMode::Base)
            return pad(intToHexString(offsetInfo.BASE + hexStringToInt(address)));
        else if (targetAddressMode == TargetAddressMode::PC)
            return pad(intToHexString(offsetInfo.PC + hexStringToInt(address)));
        else if (targetAddressMode == TargetAddressMode::Absolute)
            return pad(address);
        return EMPTY_STRING;
    }

    // Will prepend symbols for addressing mode if necessary
    const std::string prependAddressMode(const AddressingMode addressingMode, const std::string& address)
    {
        if (addressingMode == AddressingMode::Immediate)
            return prependString(IMMEDIATE_INDICATOR, address);
        if (addressingMode == AddressingMode::Indirect)
            return prependString(INDIRECT_INDICATOR, address);
        return address;
    }
}

// Use helpers to create full address string to output
const std::string CREATE_ADDRESS_OUTPUT(const AddressingInfo& addressingInfo, const OffsetInfo& offsetInfo, const std::string& objectCode)
{
    return prependAddressMode(addressingInfo.addressingMode, getAddress(addressingInfo.targetAddressMode, objectCode, offsetInfo));
}

// No manipulations but keep our main function consistent
const std::string CREATE_OBJECT_OUTPUT(const std::string& objectCode)
{
    return objectCode;
}