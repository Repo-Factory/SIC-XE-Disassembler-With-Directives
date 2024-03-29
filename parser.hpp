#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "instructions.hpp"
#include "symbol_table.hpp"

#include <memory>

enum class AddressingFormat
{
    Format2 =       2,
    Format3 =       3,
    Format4 =       4
};

enum class AddressingMode
{                         // ni
    Immediate =     0x01, // 01
    Indirect =      0x02, // 10
    Simple =        0x03, // 11
};

enum class TargetAddressMode
{                         // bp
    Absolute =      0x00, // 00
    PC =            0x01, // 01
    Base =          0x02, // 10
};

class Parser
{
    public:
        Parser();
        std::string determineOpCode(const std::string& instruction) const;
        AddressingFormat determineFormat(const std::string& instruction) const;
        AddressingMode determineAddressingMode(const std::string& instruction) const;
        TargetAddressMode determineTargetAddressMode(const std::string& instruction) const;
        bool isIndexed(const std::string& firstThreeHexDigits) const;
        std::string readInFullInstruction(std::ifstream& stream, const std::string& firstTwelveBits, const AddressingFormat format) const;
    private:
        std::unique_ptr<InstructionBindings> instructionBindings;
};

std::map<int32_t, std::string> REGISTERS();

#endif