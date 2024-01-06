#ifndef __GENERATOR_H
#define __GENERATOR_H


#include "parser.hpp"
#include <fstream>
#include <vector>
#include <unordered_map>

class Generator{
    public:
    Generator(AST_NODE * ROOT , std::string filename)
    {
        AST_ROOT = ROOT;
        assemblyFile.open(filename + ".asm");
        
    }
    
    void writeSections()
    {
        assemblyFile << includes.str();
        assemblyFile << sectionData.str();
        assemblyFile << sectionText.str();
        assemblyFile << stackReset.str();
        assemblyFile << returnStream.str();

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
        sectionData << "db \"" + *NEW_ENTRY + "\" , 10 \n";
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
        switch (STATEMENT->CHILD->TYPE)
        {
            case NODE_VARIABLE:
            {
                int offset;
                int elemOffset = variableReferenceExists(STATEMENT->CHILD->VALUE);
                if (elemOffset == -1)
                {
                    std::cout << "[!] SYNTAX ERROR : Undefined variable : " << *STATEMENT->CHILD->VALUE << std::endl;
                    exit(1);
                }

                offset = (offsetCounter - elemOffset) * 4; // what to choose between dword and qword
                sectionText << "mov eax , dword [rbp - " << std::to_string(offset);
                sectionText << "]\ncall _printINTEGER\n\n";


                break;
            }
            case NODE_INT :
            {
                sectionText << "\nmov eax , " + *STATEMENT->CHILD->VALUE;
                sectionText << "\ncall _printINTEGER\n\n";
                
                break;
            }
            case NODE_STRING :
            {
                
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

                break;
            }
            default : 
            {
                std::cout << "[!] Generation Error : Unidentified linkage to print " << nodeToString(STATEMENT->CHILD->TYPE) << std::endl;
                exit(1);
            }
        }
        
    }

    int variableReferenceExists(std::string * TEST_STRING)
    {
        int counter = 0;
        for (auto iterator = variableReferences.begin() ; iterator != variableReferences.end() ; iterator++)
        {
            if (iterator->first == *TEST_STRING) return counter;
            counter++;
        }
        return -1;
    }

    void generateVARIABLE(AST_NODE * VAR_ID)
    {
        
        int offset;
        int elemOffset = variableReferenceExists(VAR_ID->VALUE);
        if (elemOffset == -1)
        {
            variableReferences[*VAR_ID->VALUE] = VAR_ID->CHILD->VALUE;
            sectionText << "sub rsp , 4\n";
            elemOffset = 0;
            offsetCounter++;
        }

        offset = (offsetCounter - elemOffset) * 4; 

        switch (VAR_ID->CHILD->TYPE)
        {
            case NODE_INT:
            {
                sectionText << "mov dword [rbp - " << std::to_string(offset);
                sectionText << "] , " + * VAR_ID->CHILD->VALUE + "\n";
                
                break;
            }
            case NODE_VARIABLE:
            {
                int offset_RHS;
                int elemOffset_RHS = variableReferenceExists(VAR_ID->CHILD->VALUE);
                if (elemOffset_RHS == -1)
                {
                    std::cout << "[!] SYNTAX ERROR : Undefined variable : " << *VAR_ID->CHILD->VALUE << std::endl;
                    exit(1);
                }

                offset_RHS = (offsetCounter - elemOffset_RHS) * 4; // what to choose between dword and qword

                sectionText << "\nmov eax , dword [rbp - " + std::to_string(offset_RHS);
                sectionText << "]\nmov dword [rbp - " + std::to_string(offset);
                sectionText << "] , eax \n\n";

                break;

            }
            default : {
                std::cout << "[!] Generation Error : unidentidfied linkage to variable assignment : " << nodeToString(VAR_ID->CHILD->TYPE) << std::endl;
                exit(1);
            }
        }
        

    }
    
    void generate()
    {
        stringReferenceCounter = 0;
        offsetCounter = 0;
        includes << "\%include \"asm/printINTEGER.asm\" \n\n" ; // inclusion has to be done ~
        // ~ in a better way , that is , the file is included only when required 
        sectionData << "section .data\n\n";
        sectionText << "section .text\n\nglobal _start\n_start:\n\npush rbp\nmov rbp , rsp\n\n";
        
        for (AST_NODE * CURRENT : AST_ROOT->SUB_STATEMENTS)
        {
            switch(CURRENT->TYPE)
            {
                case NODE_RETURN : {returnStream << generateRETURN(CURRENT); break;}
                case NODE_PRINT : {generatePRINT(CURRENT); break;}
                case NODE_VARIABLE : {generateVARIABLE(CURRENT); break;}
                default : {std::cout << "UNRECOGNIZED NODE : "  << nodeToString(CURRENT->TYPE); break;}
            }
        }
        stackReset << "\nadd rsp , " + std::to_string(offsetCounter * 4);
        stackReset << "\nmov rsp , rbp\n";
        stackReset << "pop rbp\n\n";
        writeSections();
    }
    

    private:
    std::stringstream includes;
    std::stringstream sectionData;
    std::stringstream sectionText;
    std::stringstream stackReset;
    std::stringstream returnStream;
    int stringReferenceCounter;
    std::vector <std::string *> stringReferences;
    std::unordered_map<std::string  , std::string * > variableReferences;
    int offsetCounter;
    AST_NODE * AST_ROOT;
    std::ofstream assemblyFile;
};

#endif
