#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "headers\lexer.hpp"
#include "headers\parser.hpp"

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
    std::cout << "This is the source code  : " << std::endl << std::endl << sourceCode << std::endl;

    Lexer lexer(sourceCode);
    std::vector <Token *> tokens = lexer.tokenize();
    int counter = 0;
    std::cout << "[*] TOKENIZED SUCCESSFULLY!" << std::endl;
    if (tokens.back()->TYPE != TOKEN_EOF)
    {
        Token * EOFNode = new Token();
        EOFNode->TYPE = TOKEN_EOF;
        tokens.push_back(EOFNode);
    }
    for(Token * temp : tokens)
    {
        counter++;
        std::cout << counter << ") " << temp->VALUE << " " << typeToString(temp->TYPE) << std::endl;
    }

    Parser parser(tokens);
    AST_NODE * ROOT = parser.parse();
    std::cout << "[*] NO SYNTAX ERROR !" << std::endl;
    std::cout << "this is the number of statements " << ROOT->SUB_STATEMENTS.size() << std::endl;
    std::cout << std::endl << "END " << std::endl;

    return 0;
}
