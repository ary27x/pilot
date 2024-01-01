#ifndef __GENERATOR_H
#define __GENERATOR_H


#include "parser.hpp"
#include <fstream>
#include <vector>

class Generator{
    public:
    Generator(AST_NODE * ROOT , std::string filename)
    {
        AST_ROOT = ROOT;
        assemblyFile.open(filename + ".asm");
        
    }
    
    void writeSections()
    {
        assemblyFile << sectionData.str();
        assemblyFile << sectionText.str();

        assemblyFile.close();
    }

    std::string generateRETURN(AST_NODE * STATEMENT)
    {
        if (!STATEMENT->CHILD->VALUE)
        {
            std::cout << "[!] Generation Error : the return statement has value linked to it";
            exit(1);
        }

        std::stringstream codeBuffer;
        codeBuffer << "mov rax , 60\n";
        codeBuffer << "mov rdi , " + *STATEMENT->CHILD->VALUE;
        codeBuffer << "\nsyscall";

        return codeBuffer.str(); 
    }

    int lookup(std::string * DATA)
    {
        int counter = 0;
        for (std::string * TEMP : stringReferences)
        {
            if (*TEMP == *DATA)
            {
                return counter;
            }
            counter++;
        }
        return -1;
    }

    void sectionDataDefine(std::string * NEW_ENTRY , int referenceNumber)
    {
        sectionData << "SRef" + std::to_string(referenceNumber);
        sectionData << " :\n";
        sectionData << "db \"" + *NEW_ENTRY + "\"\n";
        sectionData << "SRef" + std::to_string(referenceNumber);
        sectionData << "_L : equ $-SRef" + std::to_string(referenceNumber);
        sectionData << "\n\n"; 

    }

    void generatePRINT(AST_NODE * STATEMENT)
    {
        if (!STATEMENT->CHILD->VALUE)
        {
            std::cout << "[!] Generation Error : the print statement has no string linked to it";
            exit(1);
        }

        int referenceNumber = lookup(STATEMENT->CHILD->VALUE);

        if (referenceNumber == -1)
        {
            referenceNumber = stringReferenceCounter;
            
            sectionDataDefine(STATEMENT->CHILD->VALUE , referenceNumber);
            stringReferences.push_back(STATEMENT->CHILD->VALUE);
            stringReferenceCounter++;
        }

        sectionText << "mov rax , 1\n";
        sectionText << "mov rdi , 1\n";
        sectionText << "mov rsi , SRef" + std::to_string(referenceNumber);
        sectionText << "\nmov rdx , SRef" + std::to_string(referenceNumber);
        sectionText << "_L \nsyscall \n\n";
    }


    void generate()
    {
        stringReferenceCounter = 0;
        sectionData << "section .data\n\n";
        sectionText << "section .text\n\nglobal _start\n_start:\n\n";
        for (AST_NODE * CURRENT : AST_ROOT->SUB_STATEMENTS)
        {
            switch(CURRENT->TYPE)
            {
                case NODE_RETURN : {sectionText << generateRETURN(CURRENT); break;}
                case NODE_PRINT : {generatePRINT(CURRENT); break;}
                default : {std::cout << "UNRECOGNIZED NODE : "  << nodeToString(CURRENT->TYPE); break;}
            }
        }
        writeSections();
    }
    

    private:
    std::stringstream sectionData;
    std::stringstream sectionText;
    int stringReferenceCounter;
    std::vector <std::string *> stringReferences;
    AST_NODE * AST_ROOT;
    std::ofstream assemblyFile;
};

#endif
