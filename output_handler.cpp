/*
 *  @brief
 *          Organizes functions to output answer
 *
 *  Defines a series of operation overloaded functions to print all of our various fields
 */

#include "output_handler.hpp"
#include <iostream>
#include <vector>
#include <sstream>

#define COLUMN_SPACING 12

const std::string XSpaces(const int X)
{
    std::string space;
    int counter = X;
    while (counter-- > 0)
    {
        space += " ";
    }
    return space;
}

const std::string appendWord(const std::string& word)
{
    return word + XSpaces(COLUMN_SPACING - word.size());
}

const std::string prependString(const std::string& prependStr, const std::string& str)
{
    return prependStr + str;
}

// First row to print
std::ostream& FileHandling::print_column_names(std::ofstream& stream, const std::string& programName, const std::string& startAddress)
{
    return stream   
    << appendWord(startAddress) 
    << appendWord(programName)  
    << appendWord("START") 
    << appendWord("0") 
    << std::endl; 
}

/* Function called from main to print the answer for the output */
std::ostream& operator<<(std::ostream& stream, const Output output)
{
    return stream  
    << appendWord(output.LOCCTR)  
    << appendWord(output.symbol)  
    << appendWord(output.opcode)  
    << appendWord(output.value) 
    << appendWord(output.objectCode)  
    << std::endl; 
}

const std::string CREATE_LOCCTR_OUTPUT(const int LOCCTR)
{
    std::stringstream hexConverter;
    hexConverter << std::hex << LOCCTR;
    return hexConverter.str();
}

const std::string CREATE_SYMBOL_OUTPUT(const int LOCCTR, const SymbolTable& table)
{
    auto it = table.find(LOCCTR);
    if (it != table.end()) {
        return it->second.name;
    }
    return "";
}

const std::string CREATE_OPCODE_OUTPUT(const std::string& opcode, const AddressingFormat format)
{
    if (format == AddressingFormat::Format4) 
        return prependString("+", opcode);
    return opcode;
}

const std::string CREATE_ADDRESS_OUTPUT(const AddressingMode addressingMode, const TargetAddressMode targetAddressMode)
{
    if (addressingMode == AddressingMode::Immediate)
        return prependString("#", "");
    if (addressingMode == AddressingMode::Indirect)
        return prependString("@", "");
    
    // if (targetAddressMode == TargetAddressMode::Base)
    // if (targetAddressMode == TargetAddressMode::PC)
    // if (targetAddressMode == TargetAddressMode::Absolute)
    
    return "";
}

const std::string CREATE_OBJECT_OUTPUT(const std::string& objectCode)
{
    return objectCode;
}