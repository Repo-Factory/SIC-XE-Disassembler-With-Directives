/*
 *  @brief
 *          Organizes functions to output answer
 *
 *  Defines a series of operation overloaded functions to print all of our various fields
 */

#include "output_handler.hpp"
#include <iostream>
#include <vector>

#define COLUMN_SPACING 12

std::string XSpaces(const int X)
{
    std::string space;
    int counter = X;
    while (counter-- > 0)
    {
        space += " ";
    }
    return space;
}

std::string appendWord(const std::string& word)
{
    return word + XSpaces(COLUMN_SPACING - word.size());
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