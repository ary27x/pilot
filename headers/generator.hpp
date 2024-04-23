#ifndef __GENERATOR_H
#define __GENERATOR_H

#include "parser.hpp"
#include "../algorithms/infixToPostfix.hpp"
#include <fstream>
#include <vector>
#include <unordered_map>
#include <stack>

class Generator{

    public:
    Generator(AST_NODE * ROOT , std::string filename)
    {
        AST_ROOT = ROOT;
        assemblyFile.open(filename + ".asm");
        
    }

    std::unordered_map <enum NODE_TYPE , std::string> relationalReferences
    {
    	  {NODE_REL_EQUALS            , "je"},
    	  {NODE_REL_NOTEQUALS         , "jne"},
    	  {NODE_REL_LESSTHAN          , "jl"}, 
    	  {NODE_REL_LESSTHANEQUALS    , "jle"},
          {NODE_REL_GREATERTHAN       , "jg"},
          {NODE_REL_GREATERTHANEQUALS , "jge"}
    };
    void writeSections()
    {
    
    	if (exitStream.str().size() == 0)
    	exitStream << "mov rax , 60\nmov rdi , 0\nsyscall\n\n";
      
        assemblyFile << includes.str();
        assemblyFile << sectionData.str();
        assemblyFile << sectionBss.str();
	    assemblyFile << sectionText.str();
        assemblyFile << stackReset.str();
        assemblyFile << exitStream.str();
        assemblyFile << functionDefinitions.str();
        assemblyFile.close();
    }

    std::string generateEXIT(AST_NODE * STATEMENT)
    {
        std::stringstream exitBuffer;
        
        switch(STATEMENT->CHILD->TYPE)
        {
            case NODE_INT : 
            {
                exitBuffer << "mov rax , 60\n";
                exitBuffer << "mov rdi , " + *STATEMENT->CHILD->VALUE;
                exitBuffer << "\nsyscall\n\n";
                break;
            }
            case NODE_VARIABLE : 
            {
                // CANNOT EXIT WITH VARIALBE INTEGERS CODE BECAUSE OF THE DESTRUCTION OF THE STACK FRAME 
                // AND THE SUBSEQUENT READING FROM THE STACK OFFSET IS CAUSING A SEGMENATION FAULT 
                // AND CRASHING THE PROGRAM (fiix this later)

                // int offset = getIntegerVariableOffset(STATEMENT->CHILD->VALUE);
                // exitBuffer << "mov eax , dword [rbp - " << std::to_string(offset) << "]\n";
                // exitBuffer << "mov rdi , rax\n";
                // exitBuffer << "mov rax , 60\n";
                // exitBuffer << "syscall\n\n";
                
                std::cout << "[~] WARNING : Varible return types are as of now not supported , exiting with code 0" << std::endl;
                // exitBuffer << "mov rax , 60\n";
                // exitBuffer << "mov rdi , 0\n";
                // exitBuffer << "\nsyscall\n\n";
                break;

                break;
            }
        }
        return exitBuffer.str(); 
    }

    int lookup(std::string * DATA)
    {
        int counter = 0;
        for (std::string * TEMP : stringReferences)
            {
            if (*TEMP == *DATA)
	            return counter;
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

    std::vector<int> convertToRequiredDimensions (std::vector<int> dimensions)
    {
        std::vector<int> newDimensions(dimensions.size());
        int writeValue = 1;
        for (int i = dimensions.size() - 1 ; i >= 0 ; i--)
	    {
		    newDimensions.at(i) = writeValue;
		    writeValue *= dimensions.at(i);
	    }
	    return newDimensions;
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

// this is the start of the helper functions
    int functionExists(std::string * FUNCTION_NAME)
    {
        if (functionTable.find(*FUNCTION_NAME) != functionTable.end())
            return 1;
        else 
            return -1;
    }
    
    int arrayExists(std::string * ARRAY_NAME)
    {
        if (arrayTable.find(*ARRAY_NAME) != arrayTable.end()) 
            return 1;
        else
            return -1;

    }

    void arrayCheck(AST_NODE * ARRAY_NODE)
    {
        if (arrayExists(ARRAY_NODE->VALUE) == -1) // this means that the array does not exists
		{
			std::cout << "[!] Error , the given array does not exists : " << *ARRAY_NODE->VALUE << std::endl;
			exit(1);
		}
		if (ARRAY_NODE->SUB_VALUES.size() != arrayTable[*ARRAY_NODE->VALUE].size())
		{
			std::cout << "[!] Dimensional error while accessing the array : " << *ARRAY_NODE->VALUE;
		       exit(1);	
		}
    }
    
    void createNewInteger(std::string VARIABLE_NAME , int * elemOffset)
    {
        variableVector.push_back(VARIABLE_NAME);
        sectionText << "sub rsp , 4\n";
        offsetCounter++;
        *elemOffset = offsetCounter * 4;
    }

    int handleStringData(std::string * STRING_DATA)
    {
        int referenceNumber = lookup(STRING_DATA);
        if (referenceNumber == -1)
        {
            referenceNumber = stringReferenceCounter;
            sectionDataDefine(STRING_DATA , referenceNumber);
            stringReferences.push_back(STRING_DATA);
            stringReferenceCounter++;
        }
        return referenceNumber;
    }

    int getIntegerVariableOffset(std::string * VARIABLE_NAME)
    {
        int elemOffset = variableReferenceExists(VARIABLE_NAME);
        if (elemOffset == -1)
        {
            std::cout << "[!] SYNTAX ERROR : Undefined integer variable : " << *VARIABLE_NAME << std::endl;
            exit(1);
        }
        return (offsetCounter - elemOffset) * 4;
    }

    void checkUniqueID(std::string * VARIABLE_NAME , enum NODE_TYPE ACCEPTABLE_TYPE = NODE_ROOT) 
    {
        int sData = stringVariableExists(VARIABLE_NAME);
        int vData = variableReferenceExists(VARIABLE_NAME);
        int fData = functionExists(VARIABLE_NAME);
        int aData = arrayExists(VARIABLE_NAME);

        if (sData != -1 && ACCEPTABLE_TYPE != NODE_STRING)
            {std::cout << "[!] Error : Cannot redeclare the following as string : " << *VARIABLE_NAME; exit(1);}
        if (vData != -1 && ACCEPTABLE_TYPE != NODE_INT)
            {std::cout << "[!] Error : Cannot redeclare the following integer variable : " << *VARIABLE_NAME; exit(1);}
        if (fData != -1 && ACCEPTABLE_TYPE != NODE_FUNCTION)
            {std::cout << "[!] Error : Cannot redeclare the following function : " << *VARIABLE_NAME; exit(1);}
        if (aData != -1 && ACCEPTABLE_TYPE != NODE_ARRAY)
            {std::cout << "[!] Error : Cannot redeclare the following array : " << *VARIABLE_NAME; exit(1);}
    }

    void inScopeGeneration(std::vector <AST_NODE *> STATEMENTS)
    {
        for (AST_NODE * CHILD_STATEMENT : STATEMENTS)
       {
	    switch(CHILD_STATEMENT->TYPE)
            {
                case NODE_EXIT           : {exitStream << generateEXIT(CHILD_STATEMENT); break;}
                case NODE_PRINT          : {generatePRINT(CHILD_STATEMENT); break;}
		        case NODE_GET            : {generateGET(CHILD_STATEMENT); break;}
                case NODE_VARIABLE       : {generateVARIABLE(CHILD_STATEMENT); break;}
                case NODE_ARRAY_VARIABLE : {generateARRAY_VARIABLE(CHILD_STATEMENT); break;}
                case NODE_IF             : {generateIF(CHILD_STATEMENT); break;}
                case NODE_RANGE          : {generateRANGE(CHILD_STATEMENT); break;}
                case NODE_TILL           : {generateTILL(CHILD_STATEMENT); break;}
                case NODE_CALL           : {generateCALL(CHILD_STATEMENT); break;}
		        case NODE_ARRAY          : {generateARRAY(CHILD_STATEMENT); break;}
                case NODE_RETURN         : {generateRETURN(CHILD_STATEMENT); break;}
                case NODE_CLEAR          : {generateCLEAR(CHILD_STATEMENT); break;}
                case NODE_SLEEP          : {generateSLEEP(CHILD_STATEMENT); break;}




                default : 
                {
                    std::cout << "[!] ERROR : FOUND : "  << nodeToString(CHILD_STATEMENT->TYPE); 
                    std::cout << " IN INDENT SCOPE OF THE PROGRAM: " ;
                    exit(0); 
                    break;
                }
            }
       }
    }

    void functionalInScopeGeneration(std::vector <AST_NODE *> STATEMENTS)
    {
        for (AST_NODE * CHILD_STATEMENT : STATEMENTS)
       {
	    switch(CHILD_STATEMENT->TYPE)
            {
                case NODE_EXIT           : {exitStream << generateEXIT(CHILD_STATEMENT); break;}
                case NODE_PRINT          : {generatePRINT(CHILD_STATEMENT); break;}
		        case NODE_GET            : {generateGET(CHILD_STATEMENT); break;}
                case NODE_VARIABLE       : {generateVARIABLE(CHILD_STATEMENT); break;}
                case NODE_ARRAY_VARIABLE : {generateARRAY_VARIABLE(CHILD_STATEMENT); break;}
                case NODE_IF             : {generateIF(CHILD_STATEMENT); break;}
                case NODE_RANGE          : {generateRANGE(CHILD_STATEMENT); break;}
                case NODE_TILL           : {generateTILL(CHILD_STATEMENT); break;}
                case NODE_CALL           : {generateCALL(CHILD_STATEMENT); break;}
		        case NODE_ARRAY          : {generateARRAY(CHILD_STATEMENT); break;}
                case NODE_RETURN         : {generateRETURN(CHILD_STATEMENT); break;}
                case NODE_CLEAR          : {generateCLEAR(CHILD_STATEMENT); break;}
                case NODE_SLEEP          : {generateSLEEP(CHILD_STATEMENT); break;}

                default : 
                {
                    std::cout << "[!] ERROR : FOUND : "  << nodeToString(CHILD_STATEMENT->TYPE); 
                    std::cout << " IN FUNCTION SCOPE OF THE PROGRAM: " ;
                    exit(0); 
                    break;
                }
            }
       }
    }
    

    void calculateArrayOffset(AST_NODE * ARRAY_NODE) // THE OFFSET VALUE WOULD BE AT THE TOP OF THE STACK
    {
        arrayCheck(ARRAY_NODE);
        std::vector <int> requiredDimensions = arrayTable[*ARRAY_NODE->VALUE];
        sectionText << "mov ecx , 0\n";
		for (int dimensionalCounter = 0 ; dimensionalCounter < ARRAY_NODE->SUB_VALUES.size() ; dimensionalCounter++)
		{
			AST_NODE * INDEX_NODE = ARRAY_NODE->SUB_VALUES[dimensionalCounter];	
			switch(INDEX_NODE->TYPE) // this value would go in the eax register
			{
			    case NODE_INT: 
			    {
				    sectionText << "mov eax , " << *INDEX_NODE->VALUE << "\n";
				    break;
			    }
			    case NODE_VARIABLE :
			    {
				    int indexOffset = variableReferenceExists(INDEX_NODE->VALUE);
				    if (indexOffset == -1)
				    {
					    std::cout << "[!] Undefined Variable " << *INDEX_NODE->VALUE;
					    exit(1);
				    }
				    indexOffset = (offsetCounter - indexOffset) * 4;
			        sectionText << "mov eax , dword [rbp - " << std::to_string(indexOffset) << "]\n";
				    break;
			    }
			}

			sectionText << "mov ebx , " << std::to_string(requiredDimensions[dimensionalCounter]) << "\n";
			sectionText << "imul rax , rbx \n";
			sectionText << "add ecx , eax \n";
		}
        
        sectionText << "mov eax , ecx \n";
        sectionText << "push rax \n";
    }
// this is the end of the helper functions

    void sectionDataDefine(std::string * NEW_ENTRY , int referenceNumber)
    {
        sectionData << "@Sref" + std::to_string(referenceNumber);
        sectionData << " :\n";
        sectionData << "db \"" + *NEW_ENTRY + "\"\n";
        sectionData << "@Sref" + std::to_string(referenceNumber);
        sectionData << "_L : equ $-@Sref" + std::to_string(referenceNumber);
        sectionData << "\n\n"; 

    }
  
    void generateGET(AST_NODE * STATEMENT) // OP0 DONE 
    {
        AST_NODE * VAR_ID = STATEMENT->CHILD;
        // could also replace this
        // BY YUSING HE CHECK UQNIE ID FUNTIONCAL CALL
        if (stringVariableExists(VAR_ID->VALUE) != -1) {std::cout << "[!] SYNTAX ERROR : cannot use string variable in INPUT : " << *VAR_ID->VALUE; exit(1);}
        if (functionExists(VAR_ID->VALUE) != -1) {std::cout << "Cannot name the variable as same as previously defined function : " << *VAR_ID->VALUE; exit(1);}
        

        switch (VAR_ID->TYPE)
        {
            case NODE_VARIABLE:
            {
                int offset;
                if (variableReferenceExists(VAR_ID->VALUE) == -1)
                    createNewInteger(*VAR_ID->VALUE , &offset);
                else
                    offset = getIntegerVariableOffset(VAR_ID->VALUE);
              
                sectionText << "call _readINTEGER";
                sectionText << "\nmov dword [rbp - " << std::to_string(offset) << " ] , eax\n";
                break;
            }
            case NODE_ARRAY_VARIABLE:
            {
                calculateArrayOffset(VAR_ID);
                sectionText << "pop rax\n";
                sectionText << "mov ecx , eax\n";
                sectionText << "call _readINTEGER \n";
			    sectionText << "mov [" << *VAR_ID->VALUE << " + ecx * 4] , eax \n";
                break;
            }
        }
        if (STATEMENT->SUB_STATEMENTS.size() != 0)
		    generateGET(STATEMENT->SUB_STATEMENTS[0]);
    }

    void generatePRINT(AST_NODE * STATEMENT , bool recursiveCall = false)
    {
        bool inlineFlag = false;
        if (!recursiveCall)
        {
            if (STATEMENT->SUB_VALUES.size() != 0) // CALL FOR PRINTING COLORS
            {
                for (AST_NODE * SUB_FUNCTION : STATEMENT->SUB_VALUES)
                {
                    if (*SUB_FUNCTION->VALUE == "inline")
                        inlineFlag = true;
                    else
                        sectionText << "call  _" + *SUB_FUNCTION->VALUE + "\n";

                }
            }
        }
        switch (STATEMENT->CHILD->TYPE)
        {
            case NODE_VARIABLE: 
            {
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
                    sectionText << "mov rsi , @Sref" + std::to_string(stringReferenceNumber) + "\n";
                    sectionText << "mov rdx , @Sref" + std::to_string(stringReferenceNumber);
                    sectionText << "_L \nsyscall \n\n";
                    
                }
                else 
                {
                    offset = getIntegerVariableOffset(STATEMENT->CHILD->VALUE);
                    sectionText << "mov eax , dword [rbp - " << std::to_string(offset);
                    sectionText << "]\ncall _printINTEGER\n\n";
                }
                break;
            }
            case NODE_ARRAY_VARIABLE :
            {
                calculateArrayOffset(STATEMENT->CHILD);
                sectionText << "pop rax \n";
                sectionText << "mov rcx , rax\n";
                sectionText << "mov eax , [" << *STATEMENT->CHILD->VALUE << " + ecx * 4]\n";
			    sectionText << "call _printINTEGER\n";
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
                            limitOffset = variableReferenceExists(STATEMENT->LIMIT->VALUE);
                            limitOffset = (offsetCounter - limitOffset) * 4;
                            break;
                        }
                        case NODE_INT :
                        {
                            createNewInteger(displayLoopSegment , &limitOffset);
                            sectionText << "mov dword [ rbp - " << std::to_string(limitOffset) << " ] , " << *STATEMENT->LIMIT->VALUE << "\n";
                            break;
                        }
                        case NODE_MATH:
                        {  
                            generateMATH(STATEMENT->LIMIT->SUB_STATEMENTS);
                            createNewInteger(displayLoopSegment , &limitOffset);
                            sectionText << "mov dword [ rbp - " << std::to_string(limitOffset) << " ] , eax\n";
                            break;
                        }
                    }
                    createNewInteger(displayLoopSegmentLHS , &lhsOffset);
                    sectionText << "mov dword [ rbp - " << std::to_string(lhsOffset) << " ] , 1\n";
                    sectionText << displayLoopSegment << ":\n";
                    sectionText << "mov eax , dword [ rbp - " << std::to_string(lhsOffset) << " ] \n";
                    sectionText << "mov ebx , dword [ rbp - " << std::to_string(limitOffset) << " ] \n";
                    sectionText << "cmp eax , ebx\n";
                    sectionText << "jg "<< displayLoopSegment << "_EXIT \n";
                }
                
                sectionText << "mov rax , 1\n";
                sectionText << "mov rdi , 1\n";
                sectionText << "mov rsi , @Sref" + std::to_string(referenceNumber) + "\n";
                sectionText << "mov rdx , @Sref" + std::to_string(referenceNumber);
                sectionText << "_L \nsyscall \n\n";

                if (STATEMENT->LIMIT)
                {
                    sectionText << "add dword [ rbp - " << std::to_string(lhsOffset) << " ] , 1\n";
                    sectionText << "jmp " << displayLoopSegment << "\n";
                    sectionText << displayLoopSegment << "_EXIT:\n\n";
                }
                break;
            }
        }
        if (STATEMENT->SUB_STATEMENTS.size() != 0)
        {
        	generatePRINT(STATEMENT->SUB_STATEMENTS[0] , true);
        }
        // we could check for a new line operator from the source code over here
        // instead of explicitly creating the newline
	
	if (!recursiveCall) 
        {
            int subFunctionCountWithoutInline = 0;
            for (AST_NODE * CALC_NODE : STATEMENT->SUB_VALUES)
            {
                if (*CALC_NODE->VALUE == "inline")
                    continue;
                subFunctionCountWithoutInline++;
            }
            if (subFunctionCountWithoutInline != 0) 
                sectionText << "call  _reset\n";
            if (!inlineFlag)
                sectionText << "call _newLine\n"; 
        }
        
        
    }

    void generateCLEAR(AST_NODE * STATEMENT)
    {
        sectionText << "mov rax , 1\n";
        sectionText << "mov rdi , 1\n";
        sectionText << "mov rsi , @pilot_clear\n";
        sectionText << "mov rdx , @pilot_clear_L\n";
        sectionText << "syscall \n\n";
    }

    void generateRANGE(AST_NODE * STATEMENT) // OP0 DONE
    {
        std::string stringLeft = "@_LOOP_" + std::to_string(loopBranchCounter) + "_LEFT";
        std::string stringRight = "@_LOOP_" + std::to_string(loopBranchCounter) + "_RIGHT";
	    int supplementOffset = 0;

        if (STATEMENT->SUPPLEMENT)
        {
            checkUniqueID(STATEMENT->SUPPLEMENT->VALUE , NODE_INT);
            int offset;
            int elemOffset = variableReferenceExists(STATEMENT->SUPPLEMENT->VALUE);
            if (elemOffset == -1)
                createNewInteger(*STATEMENT->SUPPLEMENT->VALUE , &supplementOffset);
            else 
                supplementOffset = (offsetCounter - elemOffset) * 4;
        }
        int offsetLeft;
        int offsetRight;
        switch (STATEMENT->CHILD->TYPE)
        {
            case NODE_VARIABLE:
            {
                checkUniqueID(STATEMENT->CHILD->VALUE , NODE_INT);
                offsetLeft = getIntegerVariableOffset(STATEMENT->CHILD->VALUE);
                break;
            }
            case NODE_INT :
            {
                createNewInteger(stringLeft , &offsetLeft);
                sectionText << "mov dword [ rbp - " << std::to_string(offsetLeft) << " ] , " << *STATEMENT->CHILD->VALUE << "\n";
                break;
            }
            case NODE_MATH:
            {
                generateMATH(STATEMENT->CHILD->SUB_STATEMENTS);
                createNewInteger(stringLeft , &offsetLeft);
                sectionText << "mov dword [ rbp - " << std::to_string(offsetLeft) << " ] , eax\n";
                break;
            }
        }
        switch (STATEMENT->LIMIT->TYPE)
        {
            case NODE_VARIABLE:
            {
                checkUniqueID(STATEMENT->LIMIT->VALUE , NODE_INT);
                offsetRight = getIntegerVariableOffset(STATEMENT->LIMIT->VALUE);
                break;
            }
            case NODE_INT :
            {
                createNewInteger(stringRight , &offsetRight);
                sectionText << "mov dword [ rbp - " << std::to_string(offsetRight) << " ] , " << *STATEMENT->LIMIT->VALUE << "\n";
                break;
            }
            case NODE_MATH:
            {
                generateMATH(STATEMENT->LIMIT->SUB_STATEMENTS);
                createNewInteger(stringRight , &offsetRight);
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

        inScopeGeneration(STATEMENT->SUB_STATEMENTS);

        sectionText << "add dword [ rbp - " << std::to_string(offsetLeft) << " ] , 1\n";
        sectionText << "jmp _LOOP_" << std::to_string(loopBranchCounterCopy) << "_ENTRY \n";
        sectionText << "_LOOP_" << std::to_string(loopBranchCounterCopy) << "_EXIT: \n";
    }

    void generateTILL (AST_NODE * STATEMENT) // OP0
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
                    if (stringVariableExists(BUFFERPOINTER->VALUE) != -1) {std::cout << "[!] SYNTAX ERROR : Cannot use the following string variable inside TILL : " << *BUFFERPOINTER->VALUE; exit(1);}

                    int offset = getIntegerVariableOffset(BUFFERPOINTER->VALUE);
                    sectionText << "mov eax , dword [rbp - " << std::to_string(offset);
                    sectionText << "]\npush rax\n";    
                }
            }
        }
        
        // THIS IS UNIQUE TILL CODE
        sectionText << "pop rbx\npop rax\n";
        sectionText << "cmp eax , ebx \n";
       
        BUFFERPOINTER = STATEMENT->CHILD->SUB_STATEMENTS[1];
       
        sectionText << relationalReferences[BUFFERPOINTER->TYPE] << " _TILL_" << std::to_string(tillCounterCopy) <<"_MAIN\n";
        sectionText << "jmp _TILL_" << std::to_string(tillCounterCopy) << "_EXIT\n\n";
        sectionText << "_TILL_" << std::to_string(tillCounterCopy) <<"_MAIN:\n";

        int branchCounterCopy = branchCounter;
        branchCounter++; 

        inScopeGeneration(STATEMENT->SUB_STATEMENTS);

        sectionText << "jmp _TILL_"  << std::to_string(tillCounterCopy) << "_ENTRY\n"; 
        sectionText << "_TILL_" << std::to_string(tillCounterCopy) << "_EXIT:\n";


    }

    void generateIF(AST_NODE * STATEMENT) // OP0
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

                    int offset = getIntegerVariableOffset(BUFFERPOINTER->VALUE);
                    sectionText << "mov eax , dword [rbp - " << std::to_string(offset);
                    sectionText << "]\npush rax\n";  
                    break;  
                }
                case NODE_ARRAY_VARIABLE : 
                {
                    calculateArrayOffset(BUFFERPOINTER);
                    sectionText << "pop rax \n";
                    sectionText << "mov rcx , rax \n";
                    sectionText << "mov eax , [" << *BUFFERPOINTER->VALUE << " + ecx * 4]\n";
                    sectionText << "push rax\n"; 
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
      
       branchCounter++; 
       inScopeGeneration(STATEMENT->SUB_STATEMENTS);
      
       sectionText << "jmp _BRANCH_" << std::to_string(branchCounterCopy) <<"_EXIT\n";     
       sectionText << "_BRANCH_" << std::to_string(branchCounterCopy) <<"_ELSE:\n";
       
       if (STATEMENT->SUPPLEMENT)
         generateELSE(STATEMENT->SUPPLEMENT);
       
       sectionText << "jmp _BRANCH_" << std::to_string(branchCounterCopy) <<"_EXIT\n\n";     
       sectionText << "_BRANCH_" << std::to_string(branchCounterCopy) <<"_EXIT:\n";
       
    }

    void generateELSE (AST_NODE * STATEMENT) // OP0 DONE
    {
       inScopeGeneration(STATEMENT->SUB_STATEMENTS);
    }

    void generateSLEEP (AST_NODE * STATEMENT)
    {
        switch(STATEMENT->CHILD->TYPE)
        {
            case NODE_INT :
            {
                sectionText << "mov rax , " << *STATEMENT->CHILD->VALUE << "\n";
                sectionText << "call _sleep\n";
                break;
            }
            case NODE_VARIABLE :
            {
                int offset = getIntegerVariableOffset(STATEMENT->CHILD->VALUE);
                sectionText << "mov eax , dword [rbp - " << std::to_string(offset) << "]\n";
                sectionText << "call _sleep\n";
                break;
            }
            case NODE_ARRAY_VARIABLE :
            {
                calculateArrayOffset(STATEMENT->CHILD);
                sectionText << "pop rax\n";
                sectionText << "mov ecx , eax\n";
                sectionText << "mov eax , [" << *STATEMENT->CHILD->VALUE << " + ecx * 4]\n";
                sectionText << "call _sleep\n";
                break;
            }
        }
    }

    void generateFUNCTION (AST_NODE * STATEMENT) // OP0 DONE
    {
        
       checkUniqueID(STATEMENT->CHILD->VALUE);
       functionTable[*STATEMENT->CHILD->VALUE] = STATEMENT->SUB_VALUES.size(); // the value should be the number of arguments the function expects
       std::stringstream tempstream;
       tempstream << sectionText.str();
       int startSize = sectionText.str().size(); 
       initOffsetCounter = offsetCounter;
       offsetCounter = 0;
       inFunctionalScope = true;

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

       functionalInScopeGeneration(STATEMENT->SUB_STATEMENTS); 

    //    sectionText << "add rsp , " << std::to_string((offsetCounter - initOffsetCounter)*4) << "\n";
    //    sectionText << "mov rsp , rbp\n";
    //    sectionText << "pop rbp\n";
    //    sectionText << "ret ; this is the final return and hsould be at the last \n\n";

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
       functionDefinitions << "ret \n\n";
       for (int temp = 0 ; temp < offsetCounter ; temp++) // implementing closures
            variableVector.pop_back();

       offsetCounter = initOffsetCounter;
       inFunctionalScope = false;

    }
    
    void generateCALL (AST_NODE * STATEMENT) // THE RETURN VALUE WOULD BE IN THE RAX REGISTER
    {
        if (functionExists(STATEMENT->CHILD->VALUE) == -1)
        {
            std::cout << "[!] Error : Undefined Function : " << *STATEMENT->CHILD->VALUE;
            exit(1);
        }
	    if (STATEMENT->SUB_VALUES.size() != functionTable[*STATEMENT->CHILD->VALUE])
	    {
		    std::cout << "[!] Error : The function : " << *STATEMENT->CHILD->VALUE << " expected " << functionTable[*STATEMENT->CHILD->VALUE] << " arguments , but the number of arguments provided in the call were: " << STATEMENT->SUB_VALUES.size() << std::endl;
		    exit(1);
	    }
        int registerHolderCounter = 0;
        if (STATEMENT->SUB_VALUES.size() > 6)
        {
			std::cout << "Sorry , no more than 6 arguments are allowed in the current version of pilot..." << std::endl;
			exit(1);
		}
	    for (AST_NODE * ARGUMENT : STATEMENT->SUB_VALUES) // SETTING UP THE ARGUMENTS IN THE REGISTER
	    {
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
                    int offset = getIntegerVariableOffset(ARGUMENT->VALUE);
					sectionText << "mov eax , dword [rbp - " << std::to_string(offset) << "]\n";
				    sectionText << "mov " << registerArgumentHolders[registerHolderCounter] << " , rax\n";	
					break;
				}	
		    }
		    registerHolderCounter++;
	    }
        sectionText << "call " << *STATEMENT->CHILD->VALUE << "\n";
    }

    void generateRETURN (AST_NODE * STATEMENT)
    {
        if (!inFunctionalScope)
        {
            std::cout << "[!] Syntax Error : Cannot have return statement outside of functional block";
            exit(0);
        }
        switch(STATEMENT->CHILD->TYPE)
        {
            case NODE_INT:
            {
                sectionText << "mov eax , " << *STATEMENT->CHILD->VALUE << "\n";
                break;
            }
            case NODE_VARIABLE:
            {
                int offset = getIntegerVariableOffset(STATEMENT->CHILD->VALUE);
                sectionText << "mov eax , dword [ rbp - " << std::to_string(offset) << "]\n";
                break;
            }
        }
        sectionText << "add rsp , " << std::to_string((offsetCounter - initOffsetCounter)*4) << "\n";
        sectionText << "mov rsp , rbp\n";
        sectionText << "pop rbp\n";
        sectionText << "ret; this is th error return \n\n";

    //     for (int temp = 0 ; temp < offsetCounter - initOffsetCounter; temp++) // implementing closures
    //         variableVector.pop_back();

    //    offsetCounter = initOffsetCounter;
    }

    void generateMATH(std::vector <AST_NODE * >& operations ) // THE CALCULATED VALUE IS STORED IN THE RAX REGISTER
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
                    checkUniqueID(CURRENT->VALUE , NODE_INT);
                    int offset_RHS = getIntegerVariableOffset(CURRENT->VALUE);
                    sectionText << "mov eax , dword [rbp - " + std::to_string(offset_RHS);
                    sectionText << "]\npush rax\n";
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

    void generateARRAY (AST_NODE * STATEMENT) // OP0 DONE
    {
        checkUniqueID(STATEMENT->VALUE);
        int arraySpace = 0;
        std::vector<int> dimensions; 
        for (AST_NODE * TEMP : STATEMENT->SUB_VALUES)
        {
	        if (TEMP->TYPE != NODE_INT)
	        {
		        std::cout << "The current support is only for integer dimensions , sorry..." << std::endl;
		        exit(1);
    	    }
	        dimensions.push_back(std::stoi(*TEMP->VALUE));
            arraySpace = arraySpace == 0 ? arraySpace + std::stoi(*TEMP->VALUE) : arraySpace * std::stoi(*TEMP->VALUE); 
        }
        sectionBss << *STATEMENT->VALUE << " : resd "<< std::to_string(arraySpace) << "\n\n";
        sectionText << "mov ebx , " << *STATEMENT->VALUE << "\n"; // loading the address into the ebx register
        for (AST_NODE * CURRENT_STATEMENT : STATEMENT->SUB_STATEMENTS)
        {
	        switch (CURRENT_STATEMENT->TYPE)
	        {
		        case NODE_INT:
		        {
		            sectionText << "mov eax , " <<  *CURRENT_STATEMENT->VALUE + "\n";
                	sectionText << "mov [ebx] , eax\n";
			        break;
                }
	            case NODE_VARIABLE: 
            	{
                    int offset_RHS = getIntegerVariableOffset(CURRENT_STATEMENT->VALUE);
                	sectionText << "\nmov eax , dword [rbp - " + std::to_string(offset_RHS);
             		sectionText << "]\nmov [ebx] , eax\n";
                	break;
            	}	
	       }
	       sectionText << "add ebx , 4\n";
       }
       arrayTable[*STATEMENT->VALUE] = convertToRequiredDimensions(dimensions);
    }

    void generateARRAY_VARIABLE (AST_NODE * ARRAY_ID)
    {
        calculateArrayOffset(ARRAY_ID); // THE VALUE OF THE ARRAY OFFSET WOULD BE ON THE TOP OF THE STACK
        switch (ARRAY_ID->CHILD->TYPE) 
        {
            case NODE_INT:
            {
                sectionText << "pop rax \n";
                sectionText << "mov rcx , rax\n";
			    sectionText << "mov eax , " << *ARRAY_ID->CHILD->VALUE << "\n";
			    sectionText << "mov [" << *ARRAY_ID->VALUE << " + ecx * 4] , eax\n";
		        break;
            }
            case NODE_MATH: 
            {
                generateMATH(ARRAY_ID->CHILD->SUB_STATEMENTS);
                sectionText << "mov rbx , rax \n";
                sectionText << "pop rax \n";
                sectionText << "mov rcx , rax\n";
			    sectionText << "mov [" << *ARRAY_ID->VALUE << " + ecx * 4] , ebx\n";
                break;
            }
            case NODE_ARRAY_VARIABLE:
            {
                calculateArrayOffset(ARRAY_ID->CHILD);
                sectionText << "pop rax \n";
                sectionText << "mov rbx , rax\n";
                sectionText << "pop rax \n";
                sectionText << "mov rcx , rax\n";
                sectionText << "mov eax , [" << *ARRAY_ID->CHILD->VALUE << " + ebx * 4] \n";
			    sectionText << "mov [" << *ARRAY_ID->VALUE << " + ecx * 4] , eax \n";
                break;
            }
            case NODE_VARIABLE:  // THERE SEEMS TO BE AN ERROR HERE 
            {
                int offset = getIntegerVariableOffset(ARRAY_ID->VALUE);
                sectionText << "pop rax\n";
                sectionText << "mov rbx , rax\n";
                sectionText << "mov eax, dword [rbp - " + std::to_string(offset);
                sectionText << "]\n";
                sectionText << "mov [" << *ARRAY_ID->CHILD->VALUE << " + ebx * 4] , eax\n";
                break;
            }
            case NODE_CALL: 
            {
                generateCALL(ARRAY_ID->CHILD); // the return value would be in the rax register
                sectionText << "mov rbx , rax \n";
                sectionText << "pop rax \n";
                sectionText << "mov rcx , rax\n";
			    sectionText << "mov [" << *ARRAY_ID->VALUE << " + ecx * 4] , ebx\n";
                break;
            }
            
            default : {
                std::cout << "[!] Generation Error : Error while switch the following child of ARRAY_VARIABLE : " << nodeToString(ARRAY_ID->CHILD->TYPE) << std::endl;
                exit(1);
            }
        }
    }
   
    void generateVARIABLE(AST_NODE * VAR_ID)
    {
        // FOR HADDLING NAME COLLISION , COULD MAKE USE OF
        // CHECK UNIQUE ID FUNCTION

	    if (functionTable.find(*VAR_ID->VALUE) != functionTable.end()) // name collision with function
	    {
		    std::cout << "[!] Cannot same the variable same as already defined function : " << *VAR_ID->VALUE;
		    exit(1);
	    }
        if (VAR_ID->CHILD->TYPE != NODE_STRING)  // cross linkage error     
        {
            if (stringVariableExists(VAR_ID->VALUE) != -1) 
            {
                std::cout << "[!] SYNTAX ERROR : Cannot cross link a string and an integer variable" << *VAR_ID->VALUE; 
                exit(1);
            }
    	}

        int offset;

        if (variableReferenceExists(VAR_ID->VALUE) == -1)
        {
            if (VAR_ID->CHILD->TYPE != NODE_STRING)
                createNewInteger(*VAR_ID->VALUE , &offset);
        }
        else
        {
            offset = getIntegerVariableOffset(VAR_ID->VALUE);
        }
    	

        switch (VAR_ID->CHILD->TYPE) // MAJOR THING : ADD SUPPORT FOR RHS ARRAY VALUES 
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
            case NODE_CALL: 
            {
                generateCALL(VAR_ID->CHILD); // the return value would be in the rax register
                sectionText << "mov dword [rbp - " << std::to_string(offset);
                sectionText << "] , eax \n";
                break;
            }
            case NODE_VARIABLE: 
            {
                int offset_RHS = getIntegerVariableOffset(VAR_ID->CHILD->VALUE); 
                sectionText << "\nmov eax , dword [rbp - " + std::to_string(offset_RHS);
                sectionText << "]\nmov dword [rbp - " + std::to_string(offset);
                sectionText << "] , eax \n\n";
                break;
            }
            case NODE_ARRAY_VARIABLE:
            {
                calculateArrayOffset(VAR_ID->CHILD);
                sectionText << "pop rax \n";
                sectionText << "mov rbx , rax\n";
                sectionText << "mov eax , [" << *VAR_ID->CHILD->VALUE << " + ebx * 4] \n";
                sectionText << "mov dword [rbp - " + std::to_string(offset);
                sectionText << "] , eax \n\n";
                break;
            }
            
            case NODE_STRING:
            {
                int elemOffset = variableReferenceExists(VAR_ID->VALUE);
                if (elemOffset != -1)
                {
                    std::cout << "[!] SYNTAX ERROR : Cannot Cross link a string and an integer variable"; 
                    exit(1);
                }
                int referenceNumber = handleStringData(VAR_ID->CHILD->VALUE);
                stringVariableVector[*VAR_ID->VALUE] = referenceNumber;
                break;
            }
            default : 
            {
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
        inFunctionalScope = false;
        
        includes << "\%include \"asm/readINTEGER.asm\" \n";
	    includes << "\%include \"asm/printINTEGER.asm\" \n\n" ; 
        includes << "\%include \"asm/colorWrapper.asm\" \n\n" ; 
        includes << "\%include \"asm/sleep.asm\" \n\n" ; 
        sectionData << "section .data\n\n";
        sectionData << "@pilot_clear :\n";
        sectionData << "db `\\033[2J\\033[H`\n";
        sectionData << "@pilot_clear_L : equ $-@pilot_clear\n\n";
	    sectionBss << "section .bss \n\n";
        sectionText << "section .text\n\nglobal _start\n_start:\n\npush rbp\nmov rbp , rsp\n\n";
        
        sectionText << "call _white\n\n";
        
        for (AST_NODE * CURRENT : AST_ROOT->SUB_STATEMENTS)
        {
            
            switch(CURRENT->TYPE)
            {
                case NODE_EXIT           : {exitStream << generateEXIT(CURRENT); break;}
                case NODE_PRINT          : {generatePRINT(CURRENT); break;}
	            case NODE_GET            : {generateGET(CURRENT); break;}
                case NODE_VARIABLE       : {generateVARIABLE(CURRENT); break;}
                case NODE_ARRAY_VARIABLE : {generateARRAY_VARIABLE(CURRENT); break;}
                case NODE_IF             : {generateIF(CURRENT); break;}
                case NODE_RANGE          : {generateRANGE(CURRENT); break;}
                case NODE_TILL           : {generateTILL(CURRENT); break;}
                case NODE_FUNCTION       : {generateFUNCTION(CURRENT); break;}
                case NODE_CALL           : {generateCALL(CURRENT); break;}
		        case NODE_ARRAY          : {generateARRAY(CURRENT); break;}
                case NODE_CLEAR          : {generateCLEAR(CURRENT); break;}
                case NODE_SLEEP          : {generateSLEEP(CURRENT); break;}


                default : 
                {
                    std::cout << "[!] ERROR : FOUND : "  << nodeToString(CURRENT->TYPE); 
                    std::cout << " IN GLOBAL SCOPE OF THE PROGRAM" ;
                    exit(0); 
                    break;
                }
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
    std::stringstream sectionBss;
    std::stringstream functionDefinitions;
    std::stringstream stackReset;
    std::stringstream exitStream;
    int stringReferenceCounter;
    int branchCounter; 
    int loopBranchCounter;
    int displayLoopCounter;
    int tillCounter;
    bool inFunctionalScope;
    std::vector <std::string *> stringReferences;
    std::vector <std::string> variableVector;
    std::unordered_map <std::string , int> stringVariableVector;
    std::unordered_map <std::string , int> functionTable;
    std::unordered_map <std::string , std::vector<int>> arrayTable; // string : name , std::vector<int> : dimensions
    std::vector <std::string> registerArgumentHolders = {"rdi" , "rsi" , "rdx" , "rcx" , "r8" , "r9"};
    int initOffsetCounter;
    int offsetCounter;
    AST_NODE * AST_ROOT;
    std::ofstream assemblyFile;
};

#endif
