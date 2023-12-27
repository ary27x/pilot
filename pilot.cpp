#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "headers\lexer.hpp"

int main(int argc , char ** argv)
{
    if (argc < 2)
    {
        std::cout << "Please supply the source file";
        exit(1);
    }
    // pilot example.pi

    std::cout << "Reading from the file name : " << argv[1] << std::endl;
    std::ifstream sourceFileStream (argv[1]);

    std::stringstream buffer;

    char temp;

    while(sourceFileStream.get(temp))
    {
        buffer << temp;
    }

    std::string sourceCode = buffer.str();


    Lexer lexer(sourceCode);
    std::vector <Token *> tokens = lexer.tokenize();
    int counter = 0;
    std::cout << "[*] TOKENIZED SUCCESSFULLY!" << std::endl;
    for(Token * temp : tokens)
    {
        counter++;
        std::cout << counter << ") " << temp->VALUE << " " << typeToString(temp->TYPE) << std::endl;
    }
    std::cout << std::endl << "this is the end of the program " << std::endl;

    return 0;
}