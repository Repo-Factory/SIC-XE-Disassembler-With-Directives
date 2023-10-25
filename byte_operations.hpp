#ifndef BYTE_OPERATIONS_H
#define BYTE_OPERATIONS_H

#include <string>

typedef unsigned char Byte; // 8 bit positive value

Byte convertStringToHex(const std::string& byteChars);
std::string convertHexToString(const Byte byte);
Byte extractOpCode(const Byte byte);
Byte extract_ni_flags(const Byte byte);
Byte extract_x_flag(const Byte byte);
Byte extract_bp_flags(const Byte byte);
Byte extract_e_flag(const Byte byte);

#endif