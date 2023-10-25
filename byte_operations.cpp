/*
 *  @brief
 *          Defines helpful operations for byte stream manipulations
 *
 *  We will use various masks to extract info from our instructions
 *  We will also need to perform conversions between characters and hex and vice versa
 */


#include "byte_operations.hpp"
#include <stdint.h>
#include <bitset>
#include <sstream>
#include <iostream>
#include <cstring>

#define INIT_BIT_STREAM 0
#define STRING_END_CHAR '\0'
#define ZERO_BITS 0
#define ONE_BIT 1
#define THREE_BITS 3
#define FOUR_BITS 4
#define EIGHT_BITS 8
#define OPCODE_MASK 0xFC
#define FIRST_4_BITS_MASK 0xF0
#define SECOND_4_BITS_MASK 0x0F
#define X_MASK  0x08
#define NI_MASK 0x03
#define BP_MASK 0x06
#define E_FLAG_MASK 0x01
#define HEX_CHARS "0123456789ABCDEF"

int convertFromCharToHex(char c)
{
    return (int)(strchr(HEX_CHARS, c) - HEX_CHARS); // Get pointer to hex char, then cast pointer subtractiion to get index
}

char convertFromHexToChar(int hex)
{
    return HEX_CHARS[hex];
}

/* This will take some characters read in and prepare them for masking. */
Byte convertStringToHex(const std::string& byteChars) // Accepts two chars
{
    const char* c_word = byteChars.c_str();
    Byte bitStream = INIT_BIT_STREAM;
    while (*c_word != STRING_END_CHAR) {    // Overlay four bits of each char into our 8 bit bitstream
        bitStream = (bitStream << FOUR_BITS) | convertFromCharToHex(*c_word++);
    }
    return bitStream;
}

/* Split byte into two chars and concatanate them to make our string */
std::string convertHexToString(const Byte byte)
{
    return std::string() + 
    convertFromHexToChar((byte & FIRST_4_BITS_MASK) >> FOUR_BITS) + 
    convertFromHexToChar( byte & SECOND_4_BITS_MASK >> ZERO_BITS);
}

/* opcode | nixbpe | address */

Byte extractOpCode(const Byte byte) // Pass in first two hex digits 
{
    return byte & OPCODE_MASK; // Get first six bits of first 8 bits
}

Byte extract_ni_flags(const Byte byte) // Pass in first two hex digits 
{
    return byte & NI_MASK;      // Get last two bits of first 8 bits
}

Byte extract_x_flag(const Byte byte) // Pass in first two hex digits
{
    return (byte & X_MASK) >> THREE_BITS;   // Get first bit of second hex digit, then shift it to return 0 or 1
}

Byte extract_bp_flags(const Byte byte) // Pass in last two hex digits
{
    return (byte & BP_MASK) >> ONE_BIT; // Get middle two bits of last hex digit then shift to shave off e bit
}

Byte extract_e_flag(const Byte byte) // Pass in last two hex digits
{
    return byte & E_FLAG_MASK;  // get last bit of second hex digit, returns 0 or 1
}