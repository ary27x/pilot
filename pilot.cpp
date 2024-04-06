#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "headers/lexer.hpp"
#include "headers/parser.hpp"
#include "headers/generator.hpp"

#include "languageInterfaces/hindi.hpp"

int main(int argc , char ** argv)
{
    if (argc < 2)
    {
        std::cout << "Please supply the source file";
        exit(1);
    }
    // pilot example.pi

    std::cout << "[*] COMPILING : " << argv[1] << std::endl;
    std::ifstream sourceFileStream (argv[1]);

    std::stringstream buffer;

    char temp;

    while(sourceFileStream.get(temp))
    {
        buffer << temp;
    }

    std::string sourceCode = buffer.str();
    sourceCode.append("\0");

    hindiToPilot(sourceCode);
    
    Lexer lexer(sourceCode);
    std::vector <Token *> tokens = lexer.tokenize();
    int counter = 0;
    if (tokens.back()->TYPE != TOKEN_EOF)
    {
        Token * EOFNode = new Token();
        EOFNode->TYPE = TOKEN_EOF;
        tokens.push_back(EOFNode);
    }

    // for (Token * temp : tokens)
    // {
    //     counter++;
    // 	    std::cout << counter << ") " << temp->VALUE << " " << typeToString(temp->TYPE) << std::endl;
    // }
    // exit(0);

    Parser parser(tokens);
    AST_NODE * ROOT = parser.parse();
   
    Generator generator(ROOT , argv[1]);
    generator.generate();
    
    std::string filenameWithoutExtension = argv[1];
    filenameWithoutExtension.pop_back();
    filenameWithoutExtension.pop_back();
    filenameWithoutExtension.pop_back();
    
    std::cout << "[!] COMPILATION SUCCESSFUL " << std::endl;
    std::cout << "[+] FILE READY : ./" << filenameWithoutExtension << std::endl; 

    std::stringstream assemblerInstruction;
    assemblerInstruction << "nasm -f elf64 " + filenameWithoutExtension + ".pi.asm";
    
    system(assemblerInstruction.str().c_str());

    std::stringstream linkerInstruction;
    linkerInstruction << "ld -o " + filenameWithoutExtension + " " + filenameWithoutExtension + ".pi.o";
    
    system(linkerInstruction.str().c_str());

    return 0;
}
