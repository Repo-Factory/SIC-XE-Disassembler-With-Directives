/*
 *  @brief
 *          Organizes functions to output answer
 *
 *  Defines a series of operation overloaded functions to print all of our various fields
 */

#include "output_handler.hpp"
#include <iostream>
#include <vector>

#define FORMAT_ENUM_DISPLACEMENT 2
#define ADDRESSING_MODE_ENUM_DISPLACEMENT 1

std::ostream& FileHandling::print_column_names(std::ofstream& stream, const std::string programName)
{
    return stream << ("INSTR   FORMAT   OAT   TAAM   OBJ\n"); 
}

/* Static casting our enums will give us a value we can corresponding to array index to print appropriate string  */
std::ostream& operator<<(std::ostream& stream, const AddressingMode addressingMode)
{
    switch (addressingMode)
    {
        case AddressingMode::Simple:    return stream << "Simple";    break;
        case AddressingMode::Immediate: return stream << "Immediate"; break;
        case AddressingMode::Indirect:  return stream << "Indirect";  break;
    }
    return stream << "Simple";
}

std::ostream& operator<<(std::ostream& stream, const TargetAddressMode targetAddressMode)
{
    switch (targetAddressMode)
    {
        case TargetAddressMode::Absolute: return stream << "Absolute"; break;
        case TargetAddressMode::Base:     return stream << "Base";     break;
        case TargetAddressMode::PC:       return stream << "PC";       break;
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const AddressingFormat instructionFormat)
{
    switch (instructionFormat)
    {
        case AddressingFormat::Format2: return stream << "2"; break;
        case AddressingFormat::Format3: return stream << "3"; break;
        case AddressingFormat::Format4: return stream << "4"; break;
    }
    return stream;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* If indexed is true, print out _Indexed (this will be appended to the end of target address mode) */
std::ostream& operator<<(std::ostream& stream, const Indexed wrapper)
{
    return wrapper.indexed ? stream << "_Indexed" : stream;
}

/* Function called from main to print the answer for the output */
std::ostream& operator<<(std::ostream& stream, const Output output)
{
    if (output.format == AddressingFormat::Format2) {
        return stream << output.opCode << "      " << output.format << "              " << output.objectCode << std::endl;
    } 
    else {
        return stream << output.opCode << "      " << output.format << "      " << output.addresingMode << "      " << 
        output.targetAddressMode << Indexed{output.isIndexed} << "      " << output.objectCode << std::endl;
    }
}