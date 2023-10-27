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

#define COLUMN_SPACING 10
#define IMMEDIATE_INDICATOR "#"
#define INDIRECT_INDICATOR "@"
#define FORMAT_4_INDICATOR "+"
#define EMPTY_STRING ""
#define SPACE " "

const std::string XSpaces(const int X)
{
    std::string space;
    int counter = X;
    while (counter-- > 0)
    {
        space += SPACE;
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
    return intToHexString(LOCCTR);
}

const std::string CREATE_SYMBOL_OUTPUT(const int LOCCTR, const SymbolTable& table)
{
    auto it = table.find(LOCCTR);
    if (it != table.end()) {
        return it->second.name;
    }
    return EMPTY_STRING;
}

const std::string CREATE_OPCODE_OUTPUT(const std::string& opcode, const AddressingFormat format)
{
    if (format == AddressingFormat::Format4) 
        return prependString(FORMAT_4_INDICATOR, opcode);
    return opcode;
}

namespace
{
    const std::string getAddress(const TargetAddressMode targetAddressMode, const std::string& objectCode, const OffsetInfo& offsetInfo)
    {
        const std::string address = objectCode.substr(3, objectCode.size()-1);
        if (targetAddressMode == TargetAddressMode::Base)
            return intToHexString(offsetInfo.BASE + hexStringToInt(address));
        else if (targetAddressMode == TargetAddressMode::PC)
            return intToHexString(offsetInfo.PC + hexStringToInt(address));
        else if (targetAddressMode == TargetAddressMode::Absolute)
            return address;
        return EMPTY_STRING;
    }

    const std::string prependAddressMode(const AddressingMode addressingMode, const std::string& address)
    {
        if (addressingMode == AddressingMode::Immediate)
            return prependString(IMMEDIATE_INDICATOR, address);
        if (addressingMode == AddressingMode::Indirect)
            return prependString(INDIRECT_INDICATOR, address);
        return address;
    }
}

const std::string CREATE_ADDRESS_OUTPUT(const AddressingInfo& addressingInfo, const OffsetInfo& offsetInfo, const std::string& objectCode)
{
    return prependAddressMode(addressingInfo.addressingMode, getAddress(addressingInfo.targetAddressMode, objectCode, offsetInfo));
}

const std::string CREATE_OBJECT_OUTPUT(const std::string& objectCode)
{
    return objectCode;
}