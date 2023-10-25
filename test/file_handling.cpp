#include <fstream>
#include <stdlib.h>
#include <iostream>

std::ifstream openFile(const char* filename)
{
    std::ifstream sourceCode(filename);
    if (!sourceCode) {
        // std::cerr << FILE_OPEN_FAILURE_MESSAGE << filename << std::endl;
        exit(EXIT_FAILURE);
    }
    return sourceCode;
}    

std::ifstream advanceXLines(const uint32_t X, const char* filename)
{
    uint32_t counter = X;
    std::string line;
    std::ifstream stream = openFile(filename);
    while (counter-- > 0)
    {
        std::getline(stream, line);
    }
    return stream;
}

int main()
{
    std::ifstream stream = advanceXLines(2, "/home/robox/school/cs530/assignment-two/test.sym");
    std::string line;
    std::getline(stream, line);
    std::cout << line;
}