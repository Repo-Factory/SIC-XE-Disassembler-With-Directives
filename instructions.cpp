/*
 *  @brief
 *          See hpp file for definition of various constants related to SIC/XE Instructions
 *
 *  Wraps messy instructions constants into a public API for convenient retrieval of information
 */

#include "instructions.hpp"

/* When allocated, tie together all three arrays into a hash map where the key is the opcode and the values come from the other arrays */
InstructionBindings::InstructionBindings()
{
    for (int i=0; i<NUM_INSTRUCTIONS; i++)
    {
        instructionBindings.insert({constants.ops[i], {constants.mnemonics[i], constants.format2[i]}});
    }
}

std::string InstructionBindings::getMnemonic(const std::string& opcode)
{
    return instructionBindings[opcode].mnemonics;
}

bool InstructionBindings::isFormat2(const std::string& opcode)
{
    return instructionBindings[opcode].isFormat2;
}
