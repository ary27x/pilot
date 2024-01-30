#ifndef __GENERATOR_H
#define __GENERATOR_H


#include "parser.hpp"
#include "../algorithms/infixToPostfix.hpp"
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
    std::unordered_map <enum NODE_TYPE , std::string> relationalReferences
    {
    	  {NODE_REL_EQUALS , "je"},
    	  {NODE_REL_NOTEQUALS , "jne"},
    	  {NODE_REL_LESSTHAN, "jl"}, 
    	  {NODE_REL_LESSTHANEQUALS, "jle"},
          {NODE_REL_GREATERTHAN, "jg"},
          {NODE_REL_GREATERTHANEQUALS, "jge"}
    };
    void writeSections()
    {
    
    	if (returnStream.str().size() == 0)
    	returnStream << "mov rax , 60\nmov rdi , 0\nsyscall";
      
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
    int variableReferenceExists(std::string * TEST_STRING)
    {
        int counter = 0;
        for (int i = variableVector.size() - 1 ; i >= 0  ; i--)
        {
            std::string currentValue = variableVector[i];
            if (currentValue == *TEST_STRING) return counter;
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




    void generateGET(AST_NODE * STATEMENT)
    {
       if (!STATEMENT->CHILD->VALUE || STATEMENT->CHILD->TYPE != NODE_VARIABLE)
       {
	       std::cout << "[!] Generation Error : the get statement has a missing/broken linkage ! " << std::endl;
	       exit(1);
       }

       AST_NODE * VAR_ID = STATEMENT->CHILD;
       int offset;
       int elemOffset = variableReferenceExists(VAR_ID->VALUE);

       if (elemOffset == -1)
       {
	       std::string _INPUT = "INPUT";
           variableVector.push_back(*VAR_ID->VALUE);

           sectionText << "sub rsp , 4\n";
           elemOffset = 0;
           offsetCounter++;
       }

       offset = (offsetCounter - elemOffset) * 4;
       sectionText << "call _readINTEGER";
       sectionText << "\nmov dword [rbp - " << std::to_string(offset) << " ] , eax\n";

       if (STATEMENT->SUB_STATEMENTS.size() != 0)
		generateGET(STATEMENT->SUB_STATEMENTS[0]);
    }

    void generatePRINT(AST_NODE * STATEMENT , bool recursiveCall = false)
    {
        
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
            case NODE_MATH:
            {
                generateMATH(STATEMENT->CHILD->SUB_STATEMENTS);
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
        if (STATEMENT->SUB_STATEMENTS.size() != 0)
        {
        	generatePRINT(STATEMENT->SUB_STATEMENTS[0] , true);
        }
        // we could check for a new line operator from the source code over here
        // instead of explicitly creating the newline
	
	if (!recursiveCall) sectionText << "call _newLine\n"; 
        
        
    }

    
    
    
    void generateRANGE(AST_NODE * STATEMENT)
    {
        std::string stringLeft = "@_LOOP_" + std::to_string(loopBranchCounter) + "_LEFT";
        std::string stringRight = "@_LOOP_" + std::to_string(loopBranchCounter) + "_RIGHT";

	int supplementOffset = 0;

        if (STATEMENT->SUPPLEMENT)
        {
        int offset;
        int elemOffset = variableReferenceExists(STATEMENT->SUPPLEMENT->VALUE);
        if (elemOffset == -1)
        {
            variableVector.push_back(*STATEMENT->SUPPLEMENT->VALUE);
            sectionText << "sub rsp , 4\n";
            elemOffset = 0;
            offsetCounter++;
        }
        supplementOffset = (offsetCounter - elemOffset) * 4; 
        }
        
        int offsetLeft ;
        int offsetRight;

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

                offset = (offsetCounter - elemOffset) * 4; 
                offsetLeft = offset;
                break;
            }
            case NODE_INT :
            {
                variableVector.push_back(stringLeft);
                sectionText << "sub rsp , 4\n";
                offsetCounter++;
                offsetLeft = offsetCounter * 4;
                sectionText << "mov dword [ rbp - " << std::to_string(offsetLeft) << " ] , " << *STATEMENT->CHILD->VALUE << "\n";
          
                break;
            }
            case NODE_MATH:
            {
                generateMATH(STATEMENT->CHILD->SUB_STATEMENTS);
                variableVector.push_back(stringLeft);
                sectionText << "sub rsp , 4\n";
                offsetCounter++;
                offsetLeft = offsetCounter * 4;
                sectionText << "mov dword [ rbp - " << std::to_string(offsetLeft) << " ] , eax\n";
                break;

            }
        }

        switch (STATEMENT->LIMIT->TYPE)
        {
            case NODE_VARIABLE:
            {
                
                int offset;
                int elemOffset = variableReferenceExists(STATEMENT->LIMIT->VALUE);
                if (elemOffset == -1)
                {
                    std::cout << "[!] SYNTAX ERROR : Undefined variable : " << *STATEMENT->LIMIT->VALUE << std::endl;
                    exit(1);
                }

                offset = (offsetCounter - elemOffset) * 4; 
                offsetRight = offset;
                break;
            }
            case NODE_INT :
            {

                variableVector.push_back(stringRight);

                sectionText << "sub rsp , 4\n";
                offsetCounter++;
                offsetRight = offsetCounter * 4;
                sectionText << "mov dword [ rbp - " << std::to_string(offsetRight) << " ] , " << *STATEMENT->LIMIT->VALUE << "\n";

                break;
            }
            case NODE_MATH:
            {
                generateMATH(STATEMENT->LIMIT->SUB_STATEMENTS);
                variableVector.push_back(stringRight);

                sectionText << "sub rsp , 4\n";
                offsetCounter++;
                offsetRight = offsetCounter * 4;
                sectionText << "mov dword [ rbp - " << std::to_string(offsetRight) << " ] , eax\n";

                break;

            }
        }

      
        int loopBranchCounterCopy = loopBranchCounter;
        loopBranchCounter++;

        sectionText << "jmp _LOOP_" << std::to_string(loopBranchCounterCopy) << "_ENTRY \n";
        sectionText << "_LOOP_" << std::to_string(loopBranchCounterCopy) << "_ENTRY: \n";
        
        if (supplementOffset) 
        {
        sectionText << "mov eax , dword [ rbp - " << std::to_string(offsetLeft) << " ]\n";
        sectionText << "mov dword [rbp - " << std::to_string(supplementOffset) << "] , eax\n";
        }

        sectionText << "mov eax , dword [ rbp - " << std::to_string(offsetLeft) << " ] \n";
        sectionText << "mov ebx , dword [ rbp - " << std::to_string(offsetRight) << " ] \n";
        sectionText << "cmp rax , rbx\n";
        sectionText << "jg _LOOP_" << std::to_string(loopBranchCounterCopy) << "_EXIT \n";


         for (AST_NODE * CHILD_STATEMENT : STATEMENT->SUB_STATEMENTS)
       {
	    switch(CHILD_STATEMENT->TYPE)
            {
                case NODE_PRINT : {generatePRINT(CHILD_STATEMENT); break;}
		        case NODE_GET : {generateGET(CHILD_STATEMENT); break;}
                case NODE_VARIABLE : {generateVARIABLE(CHILD_STATEMENT); break;}
                case NODE_IF : {generateIF(CHILD_STATEMENT); break;}
                case NODE_RANGE : {generateRANGE(CHILD_STATEMENT); break;}
                default : {std::cout << "UNRECOGNIZED NODE : "  << nodeToString(CHILD_STATEMENT->TYPE); break;}
            }
       }

        sectionText << "add dword [ rbp - " << std::to_string(offsetLeft) << " ] , 1\n";
        sectionText << "jmp _LOOP_" << std::to_string(loopBranchCounterCopy) << "_ENTRY \n";
        sectionText << "_LOOP_" << std::to_string(loopBranchCounterCopy) << "_EXIT: \n";

    }

    void generateIF(AST_NODE * STATEMENT)
    {
        AST_NODE * BUFFERPOINTER;
        std::string registerPlaceHolder;
        sectionText << "\n";
        for (int itr = 0 ; itr <= 2 ; itr++)
        {
            if (itr == 1) continue;

            BUFFERPOINTER = STATEMENT->CHILD->SUB_STATEMENTS[itr];
            switch (BUFFERPOINTER->TYPE)
            {
                case NODE_INT  : {sectionText << "push " << *BUFFERPOINTER->VALUE << "\n"; break;}
                case NODE_MATH : 
                {
                    generateMATH(BUFFERPOINTER->SUB_STATEMENTS); 
                    sectionText << "push rax\n"; 
                    break;
                }
                case NODE_VARIABLE : 
                {
                    int offset;
                    int elemOffset = variableReferenceExists(BUFFERPOINTER->VALUE);
                    if (elemOffset == -1)
                    {
                        std::cout << "[!] SYNTAX ERROR : Undefined variable : " << *BUFFERPOINTER->VALUE << std::endl;
                        exit(1);
                    }

                    offset = (offsetCounter - elemOffset) * 4; 
                    sectionText << "mov eax , dword [rbp - " << std::to_string(offset);
                    sectionText << "]\npush rax\n";    
                }
            }
        }

        sectionText << "pop rbx\npop rax\n";
        sectionText << "cmp eax , ebx \n";
       
        BUFFERPOINTER = STATEMENT->CHILD->SUB_STATEMENTS[1];
       
        sectionText << relationalReferences[BUFFERPOINTER->TYPE] << " _BRANCH_" << std::to_string(branchCounter) <<"_IF\n";
        sectionText << "jmp _BRANCH_" << std::to_string(branchCounter) <<"_ELSE\n\n";
        sectionText << "_BRANCH_" << std::to_string(branchCounter) <<"_IF:\n";

       int branchCounterCopy = branchCounter;
       if (STATEMENT->SUB_STATEMENTS.size() == 0)
       {
	       std::cout << "[!] Linkage Error : IF has no statements linked to it" << std::endl; exit(1);
       }
       branchCounter++; 
      
       for (AST_NODE * CHILD_STATEMENT : STATEMENT->SUB_STATEMENTS)
       {
	    switch(CHILD_STATEMENT->TYPE)
            {
                case NODE_PRINT : {generatePRINT(CHILD_STATEMENT); break;}
		        case NODE_GET : {generateGET(CHILD_STATEMENT); break;}
                case NODE_VARIABLE : {generateVARIABLE(CHILD_STATEMENT); break;}
                case NODE_IF : {generateIF(CHILD_STATEMENT); break;}
                case NODE_RANGE : {generateRANGE(CHILD_STATEMENT); break;}

                default : {std::cout << "UNRECOGNIZED NODE : "  << nodeToString(CHILD_STATEMENT->TYPE); break;}
            }
       }
       
     
       sectionText << "jmp _BRANCH_" << std::to_string(branchCounterCopy) <<"_EXIT\n";     
       sectionText << "_BRANCH_" << std::to_string(branchCounterCopy) <<"_ELSE:\n";
       
       if (STATEMENT->SUPPLEMENT)
         generateELSE(STATEMENT->SUPPLEMENT);
       
       sectionText << "jmp _BRANCH_" << std::to_string(branchCounterCopy) <<"_EXIT\n\n";     
       sectionText << "_BRANCH_" << std::to_string(branchCounterCopy) <<"_EXIT:\n";
       
       
    }

    void generateELSE (AST_NODE * STATEMENT)
    {
       if (STATEMENT->SUB_STATEMENTS.size() == 0)
       { std::cout << "[!] Linkage Error : ELSE has no statements linked to it" << std::endl; exit(1); }
       
      
       for (AST_NODE * CHILD_STATEMENT : STATEMENT->SUB_STATEMENTS)
       {
	    switch(CHILD_STATEMENT->TYPE)
            {
                case NODE_PRINT : {generatePRINT(CHILD_STATEMENT); break;}
		        case NODE_GET : {generateGET(CHILD_STATEMENT); break;}
                case NODE_VARIABLE : {generateVARIABLE(CHILD_STATEMENT); break;}
                case NODE_IF : {generateIF(CHILD_STATEMENT); break;}
                case NODE_RANGE : {generateRANGE(CHILD_STATEMENT); break;}
                default : {std::cout << "UNRECOGNIZED NODE : "  << nodeToString(CHILD_STATEMENT->TYPE); break;}
            }
       }
    }
    
    void generateMATH(std::vector <AST_NODE * >& operations )
    {
        std::vector <AST_NODE * > postfixOperations = infixToPostfix(operations);

        // IMPLEMENTING REVERSE POLISH ALGORITHM 

        for (AST_NODE * CURRENT : postfixOperations)
        {
        switch (CURRENT->TYPE)
        {
            case NODE_INT : {sectionText << "push " << *CURRENT->VALUE  << "\n"; break;}
            case NODE_VARIABLE : 
            {
                int offset_RHS;
                int elemOffset_RHS = variableReferenceExists(CURRENT->VALUE);
                if (elemOffset_RHS == -1)
                {
                    std::cout << "[!] SYNTAX ERROR : Undefined variable : " << *CURRENT->VALUE << std::endl;
                    exit(1);
                }

                offset_RHS = (offsetCounter - elemOffset_RHS) * 4; 
                sectionText << "mov eax , dword [rbp - " + std::to_string(offset_RHS);
                sectionText << "]\npush rax\n";;
                break;
            }
            case NODE_OP_ADD : {sectionText << "pop rbx \npop rax \nadd rax , rbx \npush rax\n"; break; }
            case NODE_OP_SUB : {sectionText << "pop rbx \npop rax \nsub rax , rbx \npush rax\n"; break; }
            case NODE_OP_MUL : {sectionText << "pop rbx \npop rax \nimul rax , rbx \npush rax\n"; break; }
            case NODE_OP_DIV : {sectionText << "pop rbx \npop rax \ncall _div \n push rcx\n";break;}
            case NODE_OP_MOD : {sectionText << "pop rbx \npop rax \ncall _mod \n push rcx\n";break;}
       
        }
        }

        sectionText << "pop rax\n";
    }

    void generateVARIABLE(AST_NODE * VAR_ID)
    {
        
        int offset;
        int elemOffset = variableReferenceExists(VAR_ID->VALUE);
        if (elemOffset == -1)
        {
            variableVector.push_back(*VAR_ID->VALUE);
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
            case NODE_MATH: 
            {
                generateMATH(VAR_ID->CHILD->SUB_STATEMENTS);
                sectionText << "mov dword [rbp - " << std::to_string(offset);
                sectionText << "] , eax \n";
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
        branchCounter = 0;
        loopBranchCounter = 0;
        includes << "\%include \"asm/readINTEGER.asm\" \n";
	    includes << "\%include \"asm/printINTEGER.asm\" \n\n" ; 
        sectionData << "section .data\n\n";
        sectionText << "section .text\n\nglobal _start\n_start:\n\npush rbp\nmov rbp , rsp\n\n";
        
        for (AST_NODE * CURRENT : AST_ROOT->SUB_STATEMENTS)
        {
            switch(CURRENT->TYPE)
            {
                case NODE_RETURN : {returnStream << generateRETURN(CURRENT); break;}
                case NODE_PRINT : {generatePRINT(CURRENT); break;}
		        case NODE_GET : {generateGET(CURRENT); break;}
                case NODE_VARIABLE : {generateVARIABLE(CURRENT); break;}
                case NODE_IF : {generateIF(CURRENT); break;}
                case NODE_RANGE : {generateRANGE(CURRENT); break;}
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
    int branchCounter; 
    int loopBranchCounter;
    std::vector <std::string *> stringReferences;
    std::vector <std::string> variableVector;
    int offsetCounter;
    AST_NODE * AST_ROOT;
    std::ofstream assemblyFile;
};

#endif
