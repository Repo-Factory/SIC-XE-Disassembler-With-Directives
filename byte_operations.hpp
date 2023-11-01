#ifndef BYTE_OPERATIONS_H
#define BYTE_OPERATIONS_H

#include <string>

int convertStringToHex(const std::string& byteChars);
std::string convertHexToString(const int byte);
int hexStringToInt(const std::string& str);
const std::string intToHexString(const int num);

int extractOpCode(const int byte);
int extract_ni_flags(const int byte);
int extract_x_flag(const int byte);
int extract_bp_flags(const int byte);
int extract_e_flag(const int byte);

#endif