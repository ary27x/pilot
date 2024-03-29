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
    	returnStream << "mov rax , 60\nmov rdi , 0\nsyscall\n\n";
      
        assemblyFile << includes.str();
        assemblyFile << sectionData.str();
        assemblyFile << sectionText.str();
        assemblyFile << stackReset.str();
        assemblyFile << returnStream.str();
        assemblyFile << functionDefinitions.str();
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
        codeBuffer << "\nsyscall\n\n";

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
    int stringVariableExists(std::string * TEST_STRING)
    {
        for (auto itr : stringVariableVector)
        {
            if (itr.first == *TEST_STRING)
                 return itr.second;
        }
        return -1;
    }
    
    int arrayIndexHandler (AST_NODE * SUBSCRIPT)
    {
        if (arrayTable.find(*SUBSCRIPT->VALUE) == arrayTable.end())
        {
            std::cout << "[!] Syntax Error : Trying to access the element of an undefined array : " << *SUBSCRIPT->VALUE;
            exit(1);
            
        }
        
       // todo : CHECK FOR OUT OF BOUNDS INDEX	
       
       int startOffset = arrayTable[*SUBSCRIPT->VALUE].first;
       	 
            
            
        
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
       if (stringVariableExists(VAR_ID->VALUE) != -1) {std::cout << "[!] SYNTAX ERROR : cannot use string variable in INPUT : " << *VAR_ID->VALUE; exit(1);}
       if (functionTable.find(*VAR_ID->VALUE) != functionTable.end())
       {
	       std::cout << "Cannot name the variable as same as previously defined function : " << *VAR_ID->VALUE;
	       exit(1);
       }
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
        if (!recursiveCall)
        {
            if (STATEMENT->SUPPLEMENT) // CALL FOR PRINTING COLORS
            {
                sectionText << "mov rax , 1\n";
                sectionText << "mov rdi , 1\n";
                sectionText << "mov rsi , " + *STATEMENT->SUPPLEMENT->VALUE + "\n";
                sectionText << "mov rdx , " + *STATEMENT->SUPPLEMENT->VALUE + "_L\n";
                sectionText << "syscall \n\n";
            }
        }
        switch (STATEMENT->CHILD->TYPE)
        {
            case NODE_VARIABLE: // PRITING STRING VARS
            {
                if (STATEMENT->CHILD->SUPPLEMENT)
                {
                   if (arrayTable.find(*STATEMENT->CHILD->VALUE) == arrayTable.end())
                   {
                   std::cout << "[!] Error : Undefined Array : " << *STATEMENT->CHILD->VALUE;
                   exit(1);
                   }
                   
                   int baseArrayAddress = arrayTable[*STATEMENT->CHILD->VALUE].first;
                   int elementOffset ;
                   switch (STATEMENT->CHILD->SUPPLEMENT->TYPE)
                   {
                      case NODE_INT:
                      {
                        elementOffset = baseArrayAddress + std::stoi(*STATEMENT->CHILD->SUPPLEMENT->VALUE);
			sectionText << "mov eax , dword [rbp - "<< std::to_string(elementOffset * 4) <<"]\n"; 
			sectionText << "call _printINTEGER \n\n";
                        break;
		      }
		      case NODE_VARIABLE:
		      {
		      std::cout << "Sorry , but variable index is actually not supported as of now... " << std::endl;
		      exit(1);
		      //MOV DWORD [RBP - RAX] , 5
		      break;
		      }
                   }
                break;
                }
                int offset;
                int elemOffset = variableReferenceExists(STATEMENT->CHILD->VALUE);
                if (elemOffset == -1)
                {
                    int stringReferenceNumber = stringVariableExists(STATEMENT->CHILD->VALUE);
                    if (stringReferenceNumber == -1)
                    {
                        std::cout << "[!] SYNTAX ERROR : undefined varialble : " << *STATEMENT->CHILD->VALUE;
                        exit(1);
                    }
                    sectionText << "mov rax , 1\n";
                    sectionText << "mov rdi , 1\n";
                    sectionText << "mov rsi , SRef" + std::to_string(stringReferenceNumber) + "\n";
                    sectionText << "mov rdx , SRef" + std::to_string(stringReferenceNumber);
                    sectionText << "_L \nsyscall \n\n";
                    
                }
                else 
                {
                offset = (offsetCounter - elemOffset) * 4; // what to choose between dword and qword
                sectionText << "mov eax , dword [rbp - " << std::to_string(offset);
                sectionText << "]\ncall _printINTEGER\n\n";
                }

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

                 
                std::string displayLoopSegment = "_DISPLAY_LOOP_" + std::to_string(displayLoopCounter);
                std::string displayLoopSegmentLHS = "_DISPLAY_LOOP_" + std::to_string(displayLoopCounter) + "_LHS";                 
                int lhsOffset;
                int limitOffset;  
                if (STATEMENT->LIMIT)
                {
                    
                    displayLoopCounter++;


                    switch (STATEMENT->LIMIT->TYPE)
                    {
                    case NODE_VARIABLE:
                     {
                      int elemOffset = variableReferenceExists(STATEMENT->LIMIT->VALUE);
                      if (elemOffset == -1)
                       {
                        std::cout << "[!] SYNTAX ERROR : Undefined variable : " << *STATEMENT->LIMIT->VALUE << std::endl;
                        exit(1);
                       }

                      limitOffset = (offsetCounter - elemOffset) * 4; 
                      break;
                    }
                    case NODE_INT :
                    {
                    variableVector.push_back(displayLoopSegment);
                    sectionText << "sub rsp , 4\n";
                    offsetCounter++;
                    limitOffset = offsetCounter * 4;
                    sectionText << "mov dword [ rbp - " << std::to_string(limitOffset) << " ] , " << *STATEMENT->LIMIT->VALUE << "\n";
          
                    break;
                    }
                    case NODE_MATH:
                    {
                    generateMATH(STATEMENT->LIMIT->SUB_STATEMENTS);
                    variableVector.push_back(displayLoopSegment);
                    sectionText << "sub rsp , 4\n";
                    offsetCounter++;
                    limitOffset = offsetCounter * 4;
                    sectionText << "mov dword [ rbp - " << std::to_string(limitOffset) << " ] , eax\n";
                    break;

                    }
                    }

                    variableVector.push_back(displayLoopSegmentLHS);
                    sectionText << "sub rsp , 4\n";
                    offsetCounter++;
                    lhsOffset = offsetCounter * 4;
                    sectionText << "mov dword [ rbp - " << std::to_string(lhsOffset) << " ] , 1\n";
          
                    sectionText << displayLoopSegment << ":\n";
                    sectionText << "mov eax , dword [ rbp - " << std::to_string(lhsOffset) << " ] \n";
                    sectionText << "mov ebx , dword [ rbp - " << std::to_string(limitOffset) << " ] \n";
                    sectionText << "cmp eax , ebx\n";
                    sectionText << "jg "<< displayLoopSegment << "_EXIT \n";
                    


                }
                
                sectionText << "mov rax , 1\n";
                sectionText << "mov rdi , 1\n";
                sectionText << "mov rsi , SRef" + std::to_string(referenceNumber) + "\n";
                sectionText << "mov rdx , SRef" + std::to_string(referenceNumber);
                sectionText << "_L \nsyscall \n\n";

                if (STATEMENT->LIMIT)
                {
                    sectionText << "add dword [ rbp - " << std::to_string(lhsOffset) << " ] , 1\n";
                    sectionText << "jmp " << displayLoopSegment << "\n";
                    sectionText << displayLoopSegment << "_EXIT:\n\n";
                }
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
	
	if (!recursiveCall) {

            if (STATEMENT->SUPPLEMENT) // CALL FOR PRINTING COLORS
            {
                sectionText << "mov rax , 1\n";
                sectionText << "mov rdi , 1\n";
                sectionText << "mov rsi , reset\n";
                sectionText << "mov rdx , reset_L\n";
                sectionText << "syscall \n\n";
            }
        
        sectionText << "call _newLine\n"; }
        
        
    }

 
    void generateRANGE(AST_NODE * STATEMENT)
    {
        std::string stringLeft = "@_LOOP_" + std::to_string(loopBranchCounter) + "_LEFT";
        std::string stringRight = "@_LOOP_" + std::to_string(loopBranchCounter) + "_RIGHT";

	   int supplementOffset = 0;

        if (STATEMENT->SUPPLEMENT)
        {
        if (stringVariableExists(STATEMENT->SUPPLEMENT->VALUE) != -1) {std::cout << "[!] SYNTAX ERROR : cannot use string variable in RANGE : " << *STATEMENT->SUPPLEMENT->VALUE; exit(1);}
        
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
        if (stringVariableExists(STATEMENT->CHILD->VALUE) != -1) {std::cout << "[!] SYNTAX ERROR : cannot use string variable in RANGE : " << *STATEMENT->CHILD->VALUE; exit(1);}

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
        if (stringVariableExists(STATEMENT->LIMIT->VALUE) != -1) {std::cout << "[!] SYNTAX ERROR : cannot use string variable in RANGE : " << *STATEMENT->LIMIT->VALUE; exit(1);}
                
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
        sectionText << "cmp eax , ebx\n";
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
                case NODE_TILL     : {generateTILL(CHILD_STATEMENT); break;}
                case NODE_CALL     : {generateCALL(CHILD_STATEMENT); break;}


                default : {std::cout << "UNRECOGNIZED NODE : "  << nodeToString(CHILD_STATEMENT->TYPE); break;}
            }
       }

        sectionText << "add dword [ rbp - " << std::to_string(offsetLeft) << " ] , 1\n";
        sectionText << "jmp _LOOP_" << std::to_string(loopBranchCounterCopy) << "_ENTRY \n";
        sectionText << "_LOOP_" << std::to_string(loopBranchCounterCopy) << "_EXIT: \n";

    }


   
    void generateTILL (AST_NODE * STATEMENT)
    {
        int tillCounterCopy = tillCounter;
        tillCounter++;
        sectionText << "_TILL_" << std::to_string(tillCounterCopy) << "_ENTRY:\n";

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
                if (stringVariableExists(BUFFERPOINTER->VALUE) != -1) {std::cout << "[!] SYNTAX ERROR : cannot use string variable in if : " << *BUFFERPOINTER->VALUE; exit(1);}

                    int offset;
                    int elemOffset = variableReferenceExists(BUFFERPOINTER->VALUE);
                    if (elemOffset == -1)
                    {
                        std::cout << "[!] SYNTAX ERROR : Undefined integer variable : " << *BUFFERPOINTER->VALUE << std::endl;
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
       
        sectionText << relationalReferences[BUFFERPOINTER->TYPE] << " _TILL_" << std::to_string(tillCounterCopy) <<"_MAIN\n";
        sectionText << "jmp _TILL_" << std::to_string(tillCounterCopy) << "_EXIT\n\n";
        sectionText << "_TILL_" << std::to_string(tillCounterCopy) <<"_MAIN:\n";

       int branchCounterCopy = branchCounter;
       if (STATEMENT->SUB_STATEMENTS.size() == 0)
       {
	       std::cout << "[!] Linkage Error : TILL has no statements linked to it" << std::endl; exit(1);
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
                case NODE_TILL     : {generateTILL(CHILD_STATEMENT); break;}
                case NODE_CALL     : {generateCALL(CHILD_STATEMENT); break;}


                default : {std::cout << "UNRECOGNIZED NODE : "  << nodeToString(CHILD_STATEMENT->TYPE); break;}
            }
       }

        sectionText << "jmp _TILL_"  << std::to_string(tillCounterCopy) << "_ENTRY\n"; 
        sectionText << "_TILL_" << std::to_string(tillCounterCopy) << "_EXIT:\n";


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
                if (stringVariableExists(BUFFERPOINTER->VALUE) != -1) {std::cout << "[!] SYNTAX ERROR : cannot use string variable in if : " << *BUFFERPOINTER->VALUE; exit(1);}

                    int offset;
                    int elemOffset = variableReferenceExists(BUFFERPOINTER->VALUE);
                    if (elemOffset == -1)
                    {
                        std::cout << "[!] SYNTAX ERROR : Undefined integer variable : " << *BUFFERPOINTER->VALUE << std::endl;
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
                case NODE_TILL     : {generateTILL(CHILD_STATEMENT); break;}
                case NODE_CALL     : {generateCALL(CHILD_STATEMENT); break;}



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
                case NODE_TILL     : {generateTILL(CHILD_STATEMENT); break;}
                case NODE_CALL     : {generateCALL(CHILD_STATEMENT); break;}

                default : {std::cout << "UNRECOGNIZED NODE : "  << nodeToString(CHILD_STATEMENT->TYPE); break;}
            }
       }
    }

    void generateFUNCTION (AST_NODE * STATEMENT)
    {
       if (functionTable.find(*STATEMENT->CHILD->VALUE) != functionTable.end())
       {
        std::cout << "[!] Error : Redefinition of already defined function : " << *STATEMENT->CHILD->VALUE;
        exit(1);
       }
       
       if (stringVariableExists(STATEMENT->VALUE) != -1) 
       {std::cout << "[!] Error : Cannot define a function with the name of an already defined string variable : " << *STATEMENT->VALUE; 
       exit(1);
       }
       
       if (variableReferenceExists(STATEMENT->VALUE) != -1)
       {
       std::cout << "[!] Error : Cannot define a function with the name of an already defined integer variable : " << *STATEMENT->VALUE;
       exit(1); 
       }

       // check whether we already have a defined variable with the function name 
    
       functionTable[*STATEMENT->CHILD->VALUE] = STATEMENT->SUB_VALUES.size(); // the value should be the number of arguments the function expects
       std::stringstream tempstream;
       tempstream << sectionText.str();
       int startSize = sectionText.str().size(); 
       int initOffsetCounter = offsetCounter;
       functionDefinitions << "\n" << *STATEMENT->CHILD->VALUE << ":\n";    
       functionDefinitions << "push rbp\n";
       functionDefinitions << "mov rbp , rsp\n";
      
       int argumentNumber = 0;
       for (AST_NODE * ARGUMENT : STATEMENT->SUB_VALUES)
       {
	       // the fact that there could be parameter of the name of same name defined in the 
	       // global scope , which could cause errors
	       // going over the variable Vector in the opposite direction would take care of it
               sectionText << "mov rax , " << registerArgumentHolders[argumentNumber] << "\n";
	       sectionText << "sub rsp , 4\n";
	       variableVector.push_back(*ARGUMENT->VALUE);
	       offsetCounter++;
	       int offset = offsetCounter;
	       sectionText << "mov dword [rbp - "<< std::to_string(offset * 4) << "] , eax \n";
	       argumentNumber++;
       }

        for (AST_NODE * CHILD_STATEMENT : STATEMENT->SUB_STATEMENTS)
       {
	    switch(CHILD_STATEMENT->TYPE)
            {
                case NODE_PRINT    : {generatePRINT(CHILD_STATEMENT); break;}
		case NODE_GET      : {generateGET(CHILD_STATEMENT); break;}
                case NODE_VARIABLE : {generateVARIABLE(CHILD_STATEMENT); break;}
                case NODE_IF       : {generateIF(CHILD_STATEMENT); break;}
                case NODE_RANGE    : {generateRANGE(CHILD_STATEMENT); break;}
                case NODE_TILL     : {generateTILL(CHILD_STATEMENT); break;}
                case NODE_CALL     : {generateCALL(CHILD_STATEMENT); break;}
		// add support for return types
		// put the return value in the eax register
                default : {std::cout << "UNRECOGNIZED NODE : "  << nodeToString(CHILD_STATEMENT->TYPE); break;}
            }
       }
       
       int currentChar = 1;
       for (char data : sectionText.str()) // plucking the function code from sectionText and putting it in functionDefinitions stream
       {
	       if (currentChar <= startSize)
	       {
		       currentChar++;
		       continue;
	       }
	       functionDefinitions << data;
       }

       sectionText.str(""); // re setting the sectionText
       sectionText << tempstream.str();

       functionDefinitions << "add rsp , " << std::to_string((offsetCounter - initOffsetCounter)*4) << "\n";
       functionDefinitions << "mov rsp , rbp\n";
       functionDefinitions << "pop rbp\n";
       functionDefinitions << "ret\n\n";

       for (int temp = 0 ; temp < offsetCounter - initOffsetCounter; temp++) // implementing closures
            variableVector.pop_back();
       
       offsetCounter = initOffsetCounter;
       
    }
    
    void generateCALL (AST_NODE * STATEMENT)
    {
        if (functionTable.find(*STATEMENT->CHILD->VALUE) == functionTable.end())
        {
                std::cout << "[!] Error : Undefined Function : " << *STATEMENT->CHILD->VALUE;
                exit(1);
        }
	// check for argument and parameter consistency
	if (STATEMENT->SUB_VALUES.size() != functionTable[*STATEMENT->CHILD->VALUE])
	{
		std::cout << "[!] Error : The number of arguments are not equal to the number of parameters expected by the function : " << *STATEMENT->CHILD->VALUE << std::endl;
		std::cout << "The function expected " << functionTable[*STATEMENT->CHILD->VALUE] << " arguments , but the number of arguments provided in the call were: " << STATEMENT->SUB_VALUES.size() << std::endl;
		exit(1);
	}
        int registerHolderCounter = 0;
	for (AST_NODE * ARGUMENT : STATEMENT->SUB_VALUES)
	{
		if (registerHolderCounter == 6)
		{
			std::cout << "Sorry , no more than 6 arguments are allowed in the current version of pilot..." << std::endl;
			exit(1);
		}
		switch (ARGUMENT->TYPE)
		{
			case NODE_INT :
				{
					sectionText << "mov rax , " << *ARGUMENT->VALUE << "\n";
					sectionText << "mov " << registerArgumentHolders[registerHolderCounter] << " , rax\n";
					break;
				}

		        case NODE_VARIABLE :
				{
					int offset = variableReferenceExists(ARGUMENT->VALUE);
					if (offset == -1)
					{
						std::cout << "[!] Error : undefined variable : " << *ARGUMENT->VALUE << std::endl;
						exit(1);
					}
					offset = offsetCounter - offset;
					offset *= 4;
					sectionText << "mov eax , dword [rbp - " << std::to_string(offset) << "]\n";
				        sectionText << "mov " << registerArgumentHolders[registerHolderCounter] << " , rax\n";	
					break;


				}	
		}
		registerHolderCounter++;
	}
        sectionText << "call " << *STATEMENT->CHILD->VALUE << "\n";
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
                if (stringVariableExists(CURRENT->VALUE) != -1) {std::cout << "[!] SYNTAX ERROR : cannot use string variable in math : " << *CURRENT->VALUE; exit(1);}

                int offset_RHS;
                int elemOffset_RHS = variableReferenceExists(CURRENT->VALUE);
                if (elemOffset_RHS == -1)
                {
                    std::cout << "[!] SYNTAX ERROR : Undefined integer variable : " << *CURRENT->VALUE << std::endl;
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

    void generateARRAY (AST_NODE * STATEMENT)
    {
       // CURRENTLY WE CANNOT RE DEFINE THE ARRAY , THIS IS A BEHAVIOUR WHICH WE SHOULD CHANGE 
       if (arrayTable.find(*STATEMENT->VALUE) != arrayTable.end())
       {
	       std::cout << "[!] Temporary Error : Since this being the early version , we cannot mutate the an already defined array, sorry " << *STATEMENT->VALUE;
	       exit(1);
       }

       // lets just assume that we just have integer values
       int arraySpace = 0;
       for (AST_NODE * TEMP : STATEMENT->SUB_VALUES)
       {
	       if (TEMP->TYPE != NODE_INT)
	       {
		       std::cout << "The current support is only for integer dimensions , sorry..." << std::endl;
		       exit(1);
	       }
               arraySpace = arraySpace == 0 ? arraySpace + std::stoi(*TEMP->VALUE) : arraySpace * std::stoi(*TEMP->VALUE); 

       }
       
       int initOffsetCounter = offsetCounter;
       int currentElementOffset;
       sectionText << "sub rsp , " << std::to_string(arraySpace * 4) << "\n";
       int arrayElementCounter;
       int repeatWriteValue = 0;
       for (arrayElementCounter = 0 ; arrayElementCounter < arraySpace ; arrayElementCounter++)
       {
	  currentElementOffset = ++offsetCounter * 4;
	  variableVector.push_back(*STATEMENT->VALUE);
          if (arrayElementCounter < STATEMENT->SUB_STATEMENTS.size())
	  {
	       AST_NODE * CURRENT_STATEMENT = STATEMENT->SUB_STATEMENTS[arrayElementCounter];
	       switch (CURRENT_STATEMENT->TYPE)
	       {
		       case NODE_INT:
		       {
		        sectionText << "mov dword [rbp - " << std::to_string(currentElementOffset);
                	sectionText << "] , " + *CURRENT_STATEMENT->VALUE + "\n";
			repeatWriteValue = std::stoi(*CURRENT_STATEMENT->VALUE);
                	break;
                       }
	               case NODE_VARIABLE: 
            		{

                        int offset_RHS;
                	int elemOffset_RHS = variableReferenceExists(CURRENT_STATEMENT->VALUE);
               		 if (elemOffset_RHS == -1)
                	{
                 	   std::cout << "[!] SYNTAX ERROR : Undefined variable : " << *CURRENT_STATEMENT->VALUE << std::endl;
               		     exit(1);
               		 }

              		  offset_RHS = (offsetCounter - elemOffset_RHS) * 4; // what to choose between dword and qword

                	sectionText << "\nmov eax , dword [rbp - " + std::to_string(offset_RHS);
               		 sectionText << "]\nmov dword [rbp - " + std::to_string(currentElementOffset);
                	sectionText << "] , eax \n\n";
                	
                	break;
            		}	

	       }
	    
	  }
	  else
	  {
		  sectionText << "mov dword [rbp - " << std::to_string(currentElementOffset);
		  sectionText << "] , " << std::to_string(repeatWriteValue) << "\n";
	  }
       }

       std::pair <int , int> arrayDetails = std::make_pair(initOffsetCounter + 1 , arraySpace);
       arrayTable[*STATEMENT->VALUE] = arrayDetails;
       
    }

    void generateVARIABLE(AST_NODE * VAR_ID)
    {
        
        int offset;
	if (functionTable.find(*VAR_ID->VALUE) != functionTable.end())
	{
		std::cout << "[!] Cannot same the variable same as already defined function : " << *VAR_ID->VALUE;
		exit(1);
	}
        if (VAR_ID->CHILD->TYPE != NODE_STRING)       
        {
        if (stringVariableExists(VAR_ID->VALUE) != -1) {std::cout << "[!] SYNTAX ERROR : Cannot cross link a string and an integer variable" << *VAR_ID->VALUE; exit(1);}

        int elemOffset = variableReferenceExists(VAR_ID->VALUE);
        if (elemOffset == -1)
        {
            variableVector.push_back(*VAR_ID->VALUE);
            sectionText << "sub rsp , 4\n";
            elemOffset = 0;
            offsetCounter++;
        }

        offset = (offsetCounter - elemOffset) * 4; 
        }

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
            case NODE_VARIABLE: //  we have to make some changes here so that stringvar = stringvar2 should work
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

            case NODE_STRING:
            {
                int elemOffset = variableReferenceExists(VAR_ID->VALUE);
                if (elemOffset != -1 ){std::cout << "[!] SYNTAX ERROR : Cannot cross link a string and an integer variable"; exit(1);;}
                int referenceNumber = lookup(VAR_ID->CHILD->VALUE);

                if (referenceNumber == -1)
                {
                    referenceNumber = stringReferenceCounter;
            
                    sectionDataDefine(VAR_ID->CHILD->VALUE , referenceNumber);
                    stringReferences.push_back(VAR_ID->CHILD->VALUE);
                    stringReferenceCounter++;
                }

                stringVariableVector[*VAR_ID->VALUE] = referenceNumber;

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
        displayLoopCounter = 0;
        tillCounter = 0;
        includes << "\%include \"asm/readINTEGER.asm\" \n";
	    includes << "\%include \"asm/printINTEGER.asm\" \n\n" ; 
        includes << "\%include \"asm/colors.asm\" \n\n" ; 
        sectionData << "section .data\n\n";
        sectionText << "section .text\n\nglobal _start\n_start:\n\npush rbp\nmov rbp , rsp\n\n";
        sectionText << "mov rax , 1\n";
        
        sectionText << "mov rdi , 1\n";
        sectionText << "mov rsi , white\n";
        sectionText << "mov rdx , white_L\n";
        sectionText << "syscall \n\n";

        for (AST_NODE * CURRENT : AST_ROOT->SUB_STATEMENTS)
        {
            switch(CURRENT->TYPE)
            {
                case NODE_RETURN   : {returnStream << generateRETURN(CURRENT); break;}
                case NODE_PRINT    : {generatePRINT(CURRENT); break;}
	        case NODE_GET      : {generateGET(CURRENT); break;}
                case NODE_VARIABLE : {generateVARIABLE(CURRENT); break;}
                case NODE_IF       : {generateIF(CURRENT); break;}
                case NODE_RANGE    : {generateRANGE(CURRENT); break;}
                case NODE_TILL     : {generateTILL(CURRENT); break;}
                case NODE_FUNCTION : {generateFUNCTION(CURRENT); break;}
                case NODE_CALL     : {generateCALL(CURRENT); break;}
		case NODE_ARRAY    : {generateARRAY(CURRENT); break;}

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
    std::stringstream functionDefinitions;
    std::stringstream stackReset;
    std::stringstream returnStream;
    int stringReferenceCounter;
    int branchCounter; 
    int loopBranchCounter;
    int displayLoopCounter;
    int tillCounter;
    std::vector <std::string *> stringReferences;
    std::vector <std::string> variableVector;
    std::unordered_map <std::string , int> stringVariableVector;
    std::unordered_map <std::string , int> functionTable;
    std::unordered_map <std::string , std::pair <int , int>> arrayTable; //string:name ,pair : first : STARTING OFFSET , second : NUMBER OF ELEMENTS
    std::vector <std::string> registerArgumentHolders = {"rdi" , "rsi" , "rdx" , "rcx" , "r8" , "r9"};
    int offsetCounter;
    AST_NODE * AST_ROOT;
    std::ofstream assemblyFile;
};

#endif
