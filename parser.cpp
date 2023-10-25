/*
 *  @brief
 *          Defines parsing functions which will help us determine relevant information from instruction bits
 *
 *  The byte_operations helpers hide the ugly math details, here we apply logic to answers given from those functions
 *  Depends on instructions and byte_operations files
 */

#include "parser.hpp"
#include "input_handler.hpp"
#include "byte_operations.hpp"

#define PLUS_HALF_BYTE 1
#define TWO_BYTES 2
#define FIRST_DIGIT 0
#define SECOND_DIGIT 1
#define LENGTH_OF_BYTE_IN_CHARS 2

namespace // Helper methods to take the first three hex digits, and get a substring of either the first two or last two digits
{
    std::string getFirstTwoHexDigits(const std::string& firstThreeHexDigits)
    {
        return firstThreeHexDigits.substr(FIRST_DIGIT, LENGTH_OF_BYTE_IN_CHARS);
    }

    std::string getSecondTwoHexDigits(const std::string& firstThreeHexDigits)
    {
        return firstThreeHexDigits.substr(SECOND_DIGIT, LENGTH_OF_BYTE_IN_CHARS);
    }
}

/* On allocation, will load all instruction constants into memory to be used by parser for format determination*/
Parser::Parser()
{
    this->instructionBindings = std::unique_ptr<InstructionBindings>(new InstructionBindings());   
}

/* Get OpCode mnemonic from instructionBindings hash map by passing in extracted opcode */
std::string Parser::determineOpCode(const std::string& firstThreeHexDigits)
{
    return this->instructionBindings->getMnemonic(convertHexToString(extractOpCode(convertStringToHex(getFirstTwoHexDigits(firstThreeHexDigits)))));
}

/* Can use clues from first three hex digits to determine format. */
AddressingFormat Parser::determineFormat(const std::string& firstThreeHexDigits)
{
    if (this->instructionBindings->isFormat2(convertHexToString(extractOpCode(convertStringToHex(getFirstTwoHexDigits(firstThreeHexDigits)))))) {
        return AddressingFormat::Format2; // We can use our knowledge of which OpCodes are format 2 to determine if format2
    }
    else {
        if (extract_e_flag(convertStringToHex(getSecondTwoHexDigits(firstThreeHexDigits)))) // Return 0 or 1
            return AddressingFormat::Format4;
        return AddressingFormat::Format3;  
    }  // E flag will tell us if its format 3 or 4
}

/* Extracting NI flags will give us a value between 1 and 3 which corresponds to addressing mode enum*/
AddressingMode Parser::determineAddressingMode(const std::string& firstThreeHexDigits)
{
    return static_cast<AddressingMode>(extract_ni_flags(convertStringToHex(getFirstTwoHexDigits(firstThreeHexDigits))));
}

/* Extract BP Flags will give us a value between 0 and 2 which corresponds to TargetAddressMode enum*/
TargetAddressMode Parser::determineTargetAddressMode(const std::string& firstThreeHexDigits)
{
    return static_cast<TargetAddressMode>(extract_bp_flags(convertStringToHex(getSecondTwoHexDigits(firstThreeHexDigits))));
}

/* Uses the format determination to figure out how many more bytes need to be read in after the first twelve bits */
std::string Parser::readInFullInstruction(std::ifstream& stream, const std::string& firstTwelveBits, const AddressingFormat format)
{
    return firstTwelveBits + FileHandling::readInBytes(stream, static_cast<int>(format)-TWO_BYTES, PLUS_HALF_BYTE); // static cast format gives value between 2 and 4
}

/* extracting x flag will tell us if the addressing mode is indexed */
bool Parser::isIndexed(const std::string& firstThreeHexDigits)
{
    return extract_x_flag(convertStringToHex(getSecondTwoHexDigits(firstThreeHexDigits)));
}