#ifndef __PARSER_H
#define __PARSER_H

#include "lexer.hpp"
#include <vector>
#include <stack>
#include <unordered_map>


enum NODE_TYPE
{
    NODE_ROOT,
    NODE_VARIABLE,
    NODE_ARRAY,
    NODE_RETURN,
    NODE_PRINT,
    NODE_GET,
    NODE_INT,
    NODE_STRING,
    NODE_FUNCTION,
    NODE_CALL,
    NODE_IF, 
    NODE_ELSE,
    NODE_CONDITION,
    NODE_REL_EQUALS,
    NODE_REL_NOTEQUALS, 
    NODE_REL_LESSTHAN, 
    NODE_REL_LESSTHANEQUALS,
    NODE_REL_GREATERTHAN,
    NODE_REL_GREATERTHANEQUALS,
    NODE_RANGE,
    NODE_TILL,
    NODE_MATH,
    NODE_OP_ADD,
    NODE_OP_SUB,
    NODE_OP_MUL,
    NODE_OP_DIV,
    NODE_OP_MOD,

};

std::string nodeToString(enum NODE_TYPE TYPE)
{
    switch (TYPE)
    {
        case NODE_ROOT : return "NODE_ROOT";
        case NODE_VARIABLE : return "NODE_VARIABLE";
	case NODE_ARRAY : return "NODE_ARRAY";
        case NODE_RETURN : return "NODE_RETURN";
        case NODE_PRINT : return "NODE_PRINT";
 	case NODE_GET : return "NODE_GET";
        case NODE_INT : return "NODE_INT";
        case NODE_STRING : return "NODE_STRING";
        case NODE_FUNCTION : return "NODE_FUNCTION";
        case NODE_CALL : return "NODE_CALL";
	case NODE_IF : return "NODE_IF";
	case NODE_ELSE : return "NODE_ELSE";
	case NODE_CONDITION : return "NODE_CONDITION";		       
        case NODE_REL_EQUALS : return "NODE_REL_EQUALS";
	case NODE_REL_NOTEQUALS :  return "NODE_REL_NOTEQUALS";
        case NODE_REL_LESSTHAN : return "NODE_REL_LESSTHAN";
	case NODE_REL_LESSTHANEQUALS : return "NODE_REL_LESSTHANEQUALS";
    	case NODE_REL_GREATERTHAN : return "NODE_REL_GREATERTHAN";
	case NODE_REL_GREATERTHANEQUALS : return "NODE_REL_GREATERTHANEQUALS";
	case NODE_RANGE : return "NODE_RANGE";
	case NODE_TILL : return "NODE_TILL";
	case NODE_MATH : return "NODE_MATH";	
    	case NODE_OP_ADD : return "NODE_OP_ADD";
    	case NODE_OP_SUB : return "NODE_OP_SUB";
    	case NODE_OP_MUL : return "NODE_OP_MUL";
    	case NODE_OP_DIV : return "NODE_OP_DIV";
    	case NODE_OP_MOD : return "NODE_OP_MOD";  		     
    	default : return "UNRECOGNIZED NODE";
        
    }
}

struct AST_NODE 
{
    enum NODE_TYPE TYPE;
    std::string * VALUE;
    AST_NODE * CHILD;
    AST_NODE * LIMIT;
    std::vector <AST_NODE *> SUB_STATEMENTS; 
    std::vector <AST_NODE *> SUB_VALUES;
    AST_NODE * SUPPLEMENT; 				     
};

class Parser{
    public:
    
    
    Parser(std::vector <Token *> tokens)
    {
        parserTokens = tokens;
        index = 0;
        limit = parserTokens.size();
        current = parserTokens.at(index);
    }

    std::unordered_map <std::string , enum NODE_TYPE> operatorMap = {
        {"+" ,     NODE_OP_ADD},
        {"-" ,     NODE_OP_SUB},
        {"*" ,     NODE_OP_MUL},
        {"/" ,     NODE_OP_DIV},
        {"%" ,     NODE_OP_MOD},
	{"plus" ,  NODE_OP_ADD},
	{"minus" , NODE_OP_SUB},
	{"into" ,  NODE_OP_MUL},
        {"mod" ,   NODE_OP_MOD},
        {"times" , NODE_OP_MUL},
        {"by" ,    NODE_OP_DIV},
        
    };

    std::vector <std::string> colorSupport = {
        "red" ,
        "green" ,
        "yellow" ,
        "blue" , 
        "purple" ,
        "cyan" ,
        "white"
          };
    std::vector <std::string> displayUtils = {
        "inline"
    };

    Token * tokenSeek (int offset)
    {
	if (index + offset < limit)
		return parserTokens.at(index+offset);
	else
		return parserTokens.at(limit-1); // we would be returning an eof token 
    }

    Token * proceed (enum type TYPE) 
    {
        if (current->TYPE != TYPE)
        {
        	std::cout << "this is what was expected : " << typeToString(TYPE) << " but this is what we got : " << typeToString(current->TYPE) << std::endl;
            std::cout << "[!] SYNTAX ERROR  !!" << std::endl;
            exit(1);
        }
        else
        {
            index++;
            current = parserTokens.at(index);
            return current;
        }
    }
    AST_NODE * parseINT(bool negativeNumber = false)
    {
        if (negativeNumber)
        {
            proceed(TOKEN_MATH);
        }
        if (current->TYPE != TOKEN_INT)
        {
            std::cout << "[!] SYNTAX ERROR " << std::endl;
                    exit(1);
        }

        AST_NODE * newNode = new AST_NODE();
        newNode->TYPE = NODE_INT;

        if (negativeNumber)
	{
		int toNegate = std::stoi(current->VALUE);
            	toNegate *= -1;
		current->VALUE = std::to_string(toNegate);
	}

        newNode->VALUE = &current->VALUE;
        proceed(TOKEN_INT);
        return newNode;
    }

    AST_NODE * parseID_RHS()
    {
        std::string * buffer = &current->VALUE;
        proceed(TOKEN_ID);

        AST_NODE * newNode = new AST_NODE();
        
        newNode->VALUE = buffer;
        newNode->TYPE = NODE_VARIABLE;
        
        if (current->TYPE == TOKEN_LEFT_SQR) // HAVING A SUPPLEMENT LINKAGE IN THE ID OR RHS_IS MEANS THAT IT IS AN ARRAY SUBSCRIPT
	{
		proceed(TOKEN_LEFT_SQR);
		switch (current->TYPE)
		{
			case TOKEN_INT : {newNode->SUPPLEMENT = parseINT(); break;}
            		case TOKEN_ID : {newNode->SUPPLEMENT  = parseID_RHS(); break;}
            		default : {std::cout << "[!] Syntax Error : Expected an integer or a variable in sub script of the array "; exit(1);}
		}
		proceed(TOKEN_RIGHT_SQR);
	}
        return newNode;

    }

    AST_NODE * parseOPERATOR()
    {
        AST_NODE * newNode = new AST_NODE();
        newNode->TYPE = operatorMap[current->VALUE];
        proceed(TOKEN_MATH);
        return newNode;
    }

    AST_NODE * parseCALL()
    {
        proceed(TOKEN_CALL);
        AST_NODE * newNode = new AST_NODE();
        newNode->TYPE = NODE_CALL;
        newNode->CHILD = parseID_RHS();

	if (current->TYPE == TOKEN_ARGUMENTS)
	{
	   proceed(TOKEN_ARGUMENTS);
	   while (true)
	   {
		   switch (current->TYPE)
		   {
			   case TOKEN_INT : {newNode->SUB_VALUES.push_back(parseINT()); break;}
		           case TOKEN_ID  : {newNode->SUB_VALUES.push_back(parseID_RHS()); break;}
		           default : {
					     std::cout << "[!] Syntax Error : Expected an integer or a variable in the funtion arguments" << std::endl;
					     exit(1);
				     }
		   }
		   if (current->TYPE != TOKEN_COMMA)
			   break;
		   proceed(TOKEN_COMMA);
	   }
	}
        return newNode;
    }

    AST_NODE * parseFUNCTION()
    {
    proceed(TOKEN_FUNCTION);
    AST_NODE * newNode = new AST_NODE();
    newNode->TYPE = NODE_FUNCTION;
    newNode->CHILD = parseID_RHS();

    if (current->TYPE == TOKEN_ARGUMENTS)
    {
	    proceed(TOKEN_ARGUMENTS);

	    while (true)
	    {
		    if (current->TYPE != TOKEN_ID)
		    {
			    std::cout << "[!] Syntax Error : Expected the name of the parameter : " << std::endl;
			    exit(1);
		    }
		    newNode->SUB_VALUES.push_back(parseID_RHS());
		    if (current->TYPE != TOKEN_COMMA)
			    break; //because this means that we have no other parameters
		    proceed(TOKEN_COMMA);

	    }
    }
    
     proceed(TOKEN_INDENT);
	    if (current->TYPE != TOKEN_SEMICOLON)
	    {
		std::cout << "[!] Syntax Error " << std::endl;
		exit(1);
	    }
	
	    bufferScope = stoi (current->VALUE);

	    if (bufferScope <= scopeLog.top())
	    {
		std::cout << "[!] Indentation Error : Expected further indent after the RANGE statement " << std::endl;
		exit(1);
	    }
	    scopeLog.push(bufferScope);
	    while (std::stoi(current->VALUE) == scopeLog.top())
	    {
		
        proceed(TOKEN_SEMICOLON);

	    switch (current->TYPE)
            {
                case TOKEN_ID : {newNode->SUB_STATEMENTS.push_back(parseID()); break;}
                case TOKEN_KEYWORD : {newNode->SUB_STATEMENTS.push_back(parseKEYWORD()); break;}
	            case TOKEN_RANGE : {newNode->SUB_STATEMENTS.push_back(parseRANGE()); break;} 
		        case TOKEN_TILL : {newNode->SUB_STATEMENTS.push_back(parseTILL()); break;}
                case TOKEN_CALL : {newNode->SUB_STATEMENTS.push_back(parseCALL()); break;}

                default : { std::cout << "[!] SYNTAX ERROR " << typeToString(current->TYPE) << std::endl; exit(1);}
            }

        if (current->TYPE != TOKEN_SEMICOLON)
            { std::cout << "[!] SYNTAX ERROR : Unexpected Token : " << typeToString(current->TYPE) << std::endl; exit(1); }
    }
	scopeLog.pop();
    

    return newNode;
    }

    AST_NODE * parseMATH()
    {
    	AST_NODE * newNode = new AST_NODE();
    	newNode->TYPE = NODE_MATH;
    	 
        switch (current->TYPE)
        {
            case TOKEN_INT : {newNode->SUB_STATEMENTS.push_back(parseINT()); break;}
            case TOKEN_ID : {newNode->SUB_STATEMENTS.push_back(parseID_RHS()); break;}
            default : {std::cout << "[!] Syntax Error In Maths  : Expected an integer or a value" << std::endl; exit(1);}
        }
          
        newNode->SUB_STATEMENTS.push_back(parseOPERATOR());
 
        switch (current->TYPE)
        {
            case TOKEN_INT : {newNode->SUB_STATEMENTS.push_back(parseINT()); break;}
            case TOKEN_ID : {newNode->SUB_STATEMENTS.push_back(parseID_RHS()); break;}
            default : {std::cout << "[!] Syntax Error In Maths  : Expected an integer or a value" << std::endl; exit(1);}
        }

        while (current->TYPE == TOKEN_MATH)
        {
        newNode->SUB_STATEMENTS.push_back(parseOPERATOR());
        switch (current->TYPE)
        {
            case TOKEN_INT : {newNode->SUB_STATEMENTS.push_back(parseINT()); break;}
            case TOKEN_ID : {newNode->SUB_STATEMENTS.push_back(parseID_RHS()); break;}
            default : {std::cout << "[!] Syntax Error In Maths  : Expected an integer or a value" << std::endl; exit(1);}
        }
        }

        
    	return newNode;
    }


   AST_NODE * parseARRAY()
   {
	   std::string * buffer = &current->VALUE;
	   proceed(TOKEN_ID);
           
	   AST_NODE * newNode = new AST_NODE();
	   newNode->TYPE = NODE_ARRAY;
	   newNode->VALUE = buffer;
		   
           proceed(TOKEN_EQUALS);
	   proceed(TOKEN_REL_LESSTHAN);

	   while (true)
	   {
		   switch (current->TYPE)
		   {
			   case TOKEN_INT: {newNode->SUB_VALUES.push_back(parseINT()); break;}
		           case TOKEN_ID:  {newNode->SUB_VALUES.push_back(parseID_RHS()); break;}
			   default : 
				   {
					   std::cout << "[!] Error while defining the dimensions of the array : only expected positive integers or variables" << std::endl;
					   exit(1);
				   }
		   }
		   if (current->VALUE != "*")
			   break;
		   else proceed(TOKEN_MATH);
	   }
	   // arr is <2x2> (1 , 2) , (3, 4)
           // this would be implemented later

	   proceed(TOKEN_REL_GREATERTHAN);
	   while (true)
	   {
		
	   switch(current->TYPE)
		   {
		    case TOKEN_INT : {newNode->SUB_STATEMENTS.push_back(parseINT()); break;}
		    case TOKEN_ID : {newNode->SUB_STATEMENTS.push_back(parseID_RHS()); break;}	  
		    case TOKEN_MATH : 
		    {
			    if (current->VALUE != "-")
			    {
				    std::cout << "[!] Unsupported mathematical operator while creating the array : " << current->VALUE; 
				    exit(1);

			    }
			    newNode->SUB_STATEMENTS.push_back(parseINT(true));
			    break;
		    } 
		    default :
		     {
		      std::cout << "[!] Syntax Error while creating the array : , expected either a number or a defined variable"; exit(1);
		     }
		   }
		   if (current->TYPE == TOKEN_SEMICOLON)
		  	break;
		   else 
			   proceed (TOKEN_COMMA);
	   }


	   return newNode;
   }

   AST_NODE * parse1D_ARRAY()
   {
	   std::string * buffer = &current->VALUE;
	   proceed(TOKEN_ID);
	   proceed(TOKEN_ARE);

	   AST_NODE * newNode = new AST_NODE();
	   newNode->TYPE = NODE_ARRAY;
	   newNode->VALUE = buffer;
           
	   int elementCounter = 0;
	   while (true)
	   {
		
	   switch(current->TYPE)
		   {
		    case TOKEN_INT : {newNode->SUB_STATEMENTS.push_back(parseINT()); break;}
		    case TOKEN_ID : {newNode->SUB_STATEMENTS.push_back(parseID_RHS()); break;}	  
		    case TOKEN_MATH : 
		    {
			    if (current->VALUE != "-")
			    {
				    std::cout << "[!] Unsupported mathematical operator while creating the array : " << current->VALUE; 
				    exit(1);

			    }
			    newNode->SUB_STATEMENTS.push_back(parseINT(true));
			    break;
		    } 
		    default :
		     {
		      std::cout << "[!] Syntax Error while creating the array : , expected either a number or a defined variable"; exit(1);
		     }
		   }
	           elementCounter++;
		   if (current->TYPE == TOKEN_SEMICOLON)
		  	break;
		   else 
			   proceed (TOKEN_COMMA);
	   }
	   
	   AST_NODE * sizeNODE = new AST_NODE();
	   sizeNODE->TYPE = NODE_INT;
	   std::string arraySize = std::to_string(elementCounter);
	   sizeNODE->VALUE = &arraySize;

	   newNode->SUB_VALUES.push_back(sizeNODE);

	   
	   return newNode;
   }


    AST_NODE * parseID()
    {
        std::string * buffer = &current->VALUE;
        AST_NODE * newNode = new AST_NODE();
        newNode->VALUE = buffer;
        newNode->TYPE = NODE_VARIABLE;
        proceed(TOKEN_ID);
        if (current->TYPE == TOKEN_LEFT_SQR) // HAVING A SUPPLEMENT LINKAGE IN THE ID OR RHS_IS MEANS THAT IT IS AN ARRAY SUBSCRIPT
	{
		proceed(TOKEN_LEFT_SQR);
		switch (current->TYPE)
		{
			case TOKEN_INT : {newNode->SUPPLEMENT = parseINT(); break;}
            		case TOKEN_ID : {newNode->SUPPLEMENT  = parseID_RHS(); break;}
            		default : {std::cout << "[!] Syntax Error : Expected an integer or a variable in sub script of the array "; exit(1);}
		}
		proceed(TOKEN_RIGHT_SQR);
	}
        proceed(TOKEN_EQUALS);

        
    
        
        switch (current->TYPE)
        {
            case TOKEN_INT : {newNode->CHILD = (tokenSeek(1)->TYPE == TOKEN_MATH) ? parseMATH() : parseINT(); break;}
            case TOKEN_ID : {newNode->CHILD =  (tokenSeek(1)->TYPE == TOKEN_MATH) ? parseMATH() : parseID_RHS(); break;}
            case TOKEN_MATH: 
			    {
				    if (current->VALUE != "-")
				    {
					    std::cout << "[!} Syntax Error : Unindentified Token : " << typeToString(current->TYPE) << std::endl;
					    exit(1);
				    }
				    newNode->CHILD = parseINT(true); break;
			    }
            case TOKEN_QUOTES :
            {
                proceed(TOKEN_QUOTES);
                newNode->CHILD = parseSTRING();
                proceed(TOKEN_QUOTES);
                break;
            }

            default : {
                std::cout << "[!] SYNTAX ERROR : Unidentified Token : " << typeToString(current->TYPE) << std::endl;
                exit(1);
            }
        }

        return newNode;

    }

    AST_NODE * parseRETURN()
    {
        proceed(TOKEN_KEYWORD);
        AST_NODE * newNode = new AST_NODE();
        newNode->TYPE = NODE_RETURN;
        newNode->CHILD = parseINT();

        return newNode;
    }

    AST_NODE * parseSTRING()
    {
        if (current->TYPE != TOKEN_STRING)
        {
            std::cout << "[!] Parser Error : Expected string , but did not found one";
            exit(1);
        }

        AST_NODE * newNode = new AST_NODE();

        newNode->TYPE = NODE_STRING;
        newNode->VALUE = &current->VALUE;
        
        proceed(TOKEN_STRING);
        return newNode;
    }

    AST_NODE * parseGET(bool recursiveCall = false)
    {

	if (!recursiveCall) proceed(TOKEN_KEYWORD);

	AST_NODE * newNode = new AST_NODE();

	newNode->TYPE = NODE_GET;
	newNode->CHILD = parseID_RHS();


	if (current->TYPE == TOKEN_COMMA)
	{
		  proceed(TOKEN_COMMA);
		  newNode->SUB_STATEMENTS.push_back(parseGET(true));
	}

	return newNode;

    }

    AST_NODE * parseCONDITION()
    {
	    AST_NODE * newNODE = new AST_NODE();
	    newNODE->TYPE = NODE_CONDITION;
	    

	    
      switch(current->TYPE)
      {
	      case TOKEN_ID : {newNODE->SUB_STATEMENTS.push_back((tokenSeek(1)->TYPE == TOKEN_MATH) ? parseMATH() :parseID_RHS()); break;}
	      case TOKEN_INT : {newNODE->SUB_STATEMENTS.push_back((tokenSeek(1)->TYPE == TOKEN_MATH) ? parseMATH() :parseINT()); break;}
	      default : { std::cout << "[!] SYNTAX ERROR : Unexpected Token : " << typeToString(current->TYPE) << std::endl; exit(1);}
			       
			       
      }
      
      AST_NODE * operatorNODE = new AST_NODE();
      
      switch(current->TYPE)
      {
      	case TOKEN_REL_EQUALS : {operatorNODE->TYPE = NODE_REL_EQUALS; proceed(TOKEN_REL_EQUALS); break;}
	    case TOKEN_REL_NOTEQUALS : {operatorNODE->TYPE= NODE_REL_NOTEQUALS ; proceed(TOKEN_REL_NOTEQUALS);  break;}				   
	    case TOKEN_REL_LESSTHAN : {operatorNODE->TYPE = NODE_REL_LESSTHAN; proceed(TOKEN_REL_LESSTHAN); break;}
	    case TOKEN_REL_LESSTHANEQUALS : {operatorNODE->TYPE = NODE_REL_LESSTHANEQUALS; proceed(TOKEN_REL_LESSTHANEQUALS); break;}
	    case TOKEN_REL_GREATERTHAN : {operatorNODE->TYPE = NODE_REL_GREATERTHAN; proceed(TOKEN_REL_GREATERTHAN); break;}
        case TOKEN_REL_GREATERTHANEQUALS : {operatorNODE->TYPE = NODE_REL_GREATERTHANEQUALS; proceed(TOKEN_REL_GREATERTHANEQUALS); break;}
      	default : {std::cout << "[!] SYNTAX ERROR : Unexpected Token : " << typeToString(current->TYPE) << " Expected a relational operator " << std::endl; exit(1);}
      }

      newNODE->SUB_STATEMENTS.push_back(operatorNODE);

      switch(current->TYPE)
      {
	      case TOKEN_ID : {newNODE->SUB_STATEMENTS.push_back((tokenSeek(1)->TYPE == TOKEN_MATH) ? parseMATH() :parseID_RHS()); break;}
	      case TOKEN_INT : {newNODE->SUB_STATEMENTS.push_back((tokenSeek(1)->TYPE == TOKEN_MATH) ? parseMATH() :parseINT()); break;}
	      default : { std::cout << "[!] SYNTAX ERROR : Unexpected Token : " << typeToString(current->TYPE) << std::endl; exit(1);}
	  }
        
      return newNODE;
      
    }

    AST_NODE * parseTILL()
    {
    proceed (TOKEN_TILL);

    AST_NODE * newNode = new AST_NODE();
    newNode->TYPE = NODE_TILL;
    newNode->CHILD = parseCONDITION();

     proceed(TOKEN_INDENT);
	    if (current->TYPE != TOKEN_SEMICOLON)
	    {
		std::cout << "[!] Syntax Error " << std::endl;
		exit(1);
	    }
	
	    bufferScope = stoi (current->VALUE);

	    if (bufferScope <= scopeLog.top())
	    {
		std::cout << "[!] Indentation Error : Expected further indent after the RANGE statement " << std::endl;
		exit(1);
	    }
	    scopeLog.push(bufferScope);
	    while (std::stoi(current->VALUE) == scopeLog.top())
	    {
		
        proceed(TOKEN_SEMICOLON);

	    switch (current->TYPE)
            {
                case TOKEN_ID : {newNode->SUB_STATEMENTS.push_back(parseID()); break;}
                case TOKEN_KEYWORD : {newNode->SUB_STATEMENTS.push_back(parseKEYWORD()); break;}
	            case TOKEN_RANGE : {newNode->SUB_STATEMENTS.push_back(parseRANGE()); break;} 
		        case TOKEN_TILL : {newNode->SUB_STATEMENTS.push_back(parseTILL()); break;}
                case TOKEN_CALL : {newNode->SUB_STATEMENTS.push_back(parseCALL()); break;}

                default : { std::cout << "[!] SYNTAX ERROR " << typeToString(current->TYPE) << std::endl; exit(1);}
            }

        if (current->TYPE != TOKEN_SEMICOLON)
            { std::cout << "[!] SYNTAX ERROR : Unexpected Token : " << typeToString(current->TYPE) << std::endl; exit(1); }
    }
	scopeLog.pop();
    

    return newNode;
    }


    AST_NODE * parseRANGE()
    {
       proceed(TOKEN_RANGE);
       AST_NODE * newNode = new AST_NODE();
       newNode->TYPE = NODE_RANGE;
       
        switch (current->TYPE)
        {
            case TOKEN_INT : {newNode->CHILD = (tokenSeek(1)->TYPE == TOKEN_MATH) ? parseMATH() : parseINT(); break; }           
            case TOKEN_ID : {newNode->CHILD = (tokenSeek(1)->TYPE == TOKEN_MATH) ? parseMATH() : parseID_RHS(); break; }
            default : {std::cout << "[!] Parser Error ! Unindentified token : " << typeToString(current->TYPE) << std::endl; exit(1);}
        }
        
        proceed(TOKEN_TO);

        switch (current->TYPE)
        {
            case TOKEN_INT : {newNode->LIMIT = (tokenSeek(1)->TYPE == TOKEN_MATH) ? parseMATH() : parseINT(); break; }           
            case TOKEN_ID : {newNode->LIMIT = (tokenSeek(1)->TYPE == TOKEN_MATH) ? parseMATH() : parseID_RHS(); break; }
            default : {std::cout << "[!] Parser Error ! Unindentified token : " << typeToString(current->TYPE) << std::endl; exit(1);}
        }

        if (current->TYPE == TOKEN_AS)
        {
            proceed(TOKEN_AS);
            if (current->TYPE != TOKEN_ID) 
            {
                std::cout << "[!] Parser Error : Give a variable name after AS" << std::endl;
                exit(1);
            }
            newNode->SUPPLEMENT = parseID_RHS();
        }

        proceed(TOKEN_INDENT);
	    if (current->TYPE != TOKEN_SEMICOLON)
	    {
		std::cout << "[!] Syntax Error " << std::endl;
		exit(1);
	    }
	
	    bufferScope = stoi (current->VALUE);

	    if (bufferScope <= scopeLog.top())
	    {
		std::cout << "[!] Indentation Error : Expected further indent after the RANGE statement " << std::endl;
		exit(1);
	    }
	    scopeLog.push(bufferScope);
	    while (std::stoi(current->VALUE) == scopeLog.top())
	    {
		
        proceed(TOKEN_SEMICOLON);

	    switch (current->TYPE)
            {
                case TOKEN_ID : {newNode->SUB_STATEMENTS.push_back(parseID()); break;}
                case TOKEN_KEYWORD : {newNode->SUB_STATEMENTS.push_back(parseKEYWORD()); break;}
	            case TOKEN_RANGE : {newNode->SUB_STATEMENTS.push_back(parseRANGE()); break;} 
		        case TOKEN_TILL : {newNode->SUB_STATEMENTS.push_back(parseTILL()); break;}
                case TOKEN_CALL : {newNode->SUB_STATEMENTS.push_back(parseCALL()); break;}
                
                default : { std::cout << "[!] SYNTAX ERROR " << typeToString(current->TYPE) << std::endl; exit(1);}
            }

        if (current->TYPE != TOKEN_SEMICOLON)
            { std::cout << "[!] SYNTAX ERROR : Unexpected Token : " << typeToString(current->TYPE) << std::endl; exit(1); }
    }
	scopeLog.pop();
    

    return newNode;
    }
    AST_NODE * parseIF()
    {	
	proceed(TOKEN_KEYWORD);
	
	AST_NODE * newNODE = new AST_NODE();
	newNODE->TYPE = NODE_IF;
	newNODE->CHILD = parseCONDITION();

	proceed(TOKEN_INDENT);
	if (current->TYPE != TOKEN_SEMICOLON)
	{
		std::cout << "[!] Syntax Error " << std::endl;
		exit(1);
	}
	
	bufferScope = stoi (current->VALUE);
	if (bufferScope <= scopeLog.top())
	{
		std::cout << "[!] Indentation Error : Expected further indent after the if statement " << std::endl;
		exit(1);
	}
	scopeLog.push(bufferScope);
	while (std::stoi(current->VALUE) == scopeLog.top())
	{
		
        proceed(TOKEN_SEMICOLON);
	    switch (current->TYPE)
            {
                case TOKEN_ID : {newNODE->SUB_STATEMENTS.push_back(parseID()); break;}
                case TOKEN_KEYWORD : {newNODE->SUB_STATEMENTS.push_back(parseKEYWORD()); break;}
                 case TOKEN_RANGE : {newNODE->SUB_STATEMENTS.push_back(parseRANGE()); break;} 
		        case TOKEN_TILL : {newNODE->SUB_STATEMENTS.push_back(parseTILL()); break;}
                case TOKEN_CALL : {newNODE->SUB_STATEMENTS.push_back(parseCALL()); break;}


                default : { std::cout << "[!] SYNTAX ERROR " << typeToString(current->TYPE) << std::endl; exit(1);}
            }

        if (current->TYPE != TOKEN_SEMICOLON)
            { std::cout << "[!] SYNTAX ERROR : Unexpected Token : " << typeToString(current->TYPE) << std::endl; exit(1); }
    }
	scopeLog.pop();
	if (std::stoi(current->VALUE) == scopeLog.top() && tokenSeek(1)->VALUE == "else")
	{
		proceed(TOKEN_SEMICOLON);
		newNODE->SUPPLEMENT = parseELSE();

	}	
	
	return newNODE;
    }

    AST_NODE * parseELSE()
    {
    	proceed(TOKEN_KEYWORD);
	
	AST_NODE * newNODE = new AST_NODE();
	newNODE->TYPE = NODE_ELSE;
	

	proceed(TOKEN_INDENT);

	if (current->TYPE != TOKEN_SEMICOLON)
	{
		std::cout << "[!] Syntax Error " << std::endl;
		exit(1);
	}
	
	bufferScope = stoi (current->VALUE);
	if (bufferScope <= scopeLog.top())
	{
		std::cout << "[!] Indentation Error : Expected further indent after the else statement " << std::endl;
		exit(1);
	}
	scopeLog.push(bufferScope);
	while (std::stoi(current->VALUE) == scopeLog.top())
	{
		
        proceed(TOKEN_SEMICOLON);
	    switch (current->TYPE)
            {
                case TOKEN_ID : {newNODE->SUB_STATEMENTS.push_back(parseID()); break;}
                case TOKEN_KEYWORD : {newNODE->SUB_STATEMENTS.push_back(parseKEYWORD()); break;}
                case TOKEN_RANGE : {newNODE->SUB_STATEMENTS.push_back(parseRANGE()); break;} 
		        case TOKEN_TILL : {newNODE->SUB_STATEMENTS.push_back(parseTILL()); break;}
                case TOKEN_CALL : {newNODE->SUB_STATEMENTS.push_back(parseCALL()); break;}
                default : { std::cout << "[!] SYNTAX ERROR " << typeToString(current->TYPE) << std::endl; exit(1);}
            }

        if (current->TYPE != TOKEN_SEMICOLON)
            { std::cout << "[!] SYNTAX ERROR : Unexpected Token : " << typeToString(current->TYPE) << std::endl; exit(1); }
    	}
	scopeLog.pop();
		
	
	return newNODE;
    }


    AST_NODE * parsePRINT(bool recursiveCall = false) // current support is only for 32 bits numbers
    {
    	AST_NODE * newNode = new AST_NODE();
	
	if (!recursiveCall)
	{
	       	proceed (TOKEN_KEYWORD);
		if (current->TYPE == TOKEN_DOT)
		{
			proceed(TOKEN_DOT);
			if (current->TYPE != TOKEN_ID)
			{
				std::cout << "[!] SYNTAX ERROR : error in the display sub function call " << std::endl;
				exit(1);
			}
             if (std::find(colorSupport.begin() , colorSupport.end() , current->VALUE) == colorSupport.end())
            {
                std::cout << "[!] SYNTAX ERROR : unknown display sub function called" << std::endl;
                exit(1);
            }
			newNode->SUPPLEMENT = parseID_RHS();
		}

	}
        

        switch (current->TYPE)
        {
            
            case TOKEN_INT :
            {              
                newNode->TYPE = NODE_PRINT;
                newNode->CHILD = (tokenSeek(1)->TYPE == TOKEN_MATH) ? parseMATH() : parseINT();


                break;
            }
            
            case TOKEN_ID :
            {
                newNode->TYPE = NODE_PRINT;
                newNode->CHILD = (tokenSeek(1)->TYPE == TOKEN_MATH) ? parseMATH() : parseID_RHS();

                break;
            }
            case TOKEN_QUOTES :
            {
                
                proceed(TOKEN_QUOTES);
        
                newNode->TYPE = NODE_PRINT;
                newNode->CHILD = parseSTRING();

                proceed(TOKEN_QUOTES);

                if (current->TYPE == TOKEN_MATH && (current->VALUE == "times" || current->VALUE == "*"))
                {
                    proceed(TOKEN_MATH);
                    switch (current->TYPE)
                    {
                    case TOKEN_INT : {newNode->LIMIT = (tokenSeek(1)->TYPE == TOKEN_MATH) ? parseMATH() : parseINT(); break;}
                    case TOKEN_ID : {newNode->LIMIT =  (tokenSeek(1)->TYPE == TOKEN_MATH) ? parseMATH() : parseID_RHS(); break;}

                    default : {
                    std::cout << "[!] SYNTAX ERROR : Unidentified Token : " << typeToString(current->TYPE) << std::endl;
                    exit(1);
                    }
                }

                }

                break;
            }
            default :
            {
                std::cout << "[!] Parser Error ! Unindentified token : " << typeToString(current->TYPE) << std::endl;
                exit(1); 
            }
        }


        if (current->TYPE == TOKEN_COMMA)
        {
        proceed(TOKEN_COMMA);
        newNode->SUB_STATEMENTS.push_back(parsePRINT(true));
        }
        
        if (current->TYPE != TOKEN_SEMICOLON)
        {
        std::cout << "[!] Syntax Error " << std::endl;
        exit(1);
        }
        
        return newNode;
        
    }

    AST_NODE * parseKEYWORD()
    {
        if (current->VALUE == "return")
        {
            return parseRETURN();
        }
        else if (current->VALUE == "print")
        {
            return parsePRINT();
        }
	    else if (current->VALUE == "get")
	    {   
	        return parseGET();
	    }
	    else if (current->VALUE == "if")
	    {
        	return parseIF();
	    }
        else { std::cout << "[!] SYNTAX ERROR , UNDEFINED KEYWORD " << std::endl; exit(1); }
    }

    AST_NODE * parse()
    {
        AST_NODE * ROOT = new AST_NODE();
        ROOT->TYPE = NODE_ROOT;
	    scopeLog.push(0);
	

        while(current->TYPE != TOKEN_EOF)
        {
            switch (current->TYPE)
            {

                case TOKEN_ID : 
		{
		if (tokenSeek(1)->TYPE == TOKEN_ARE)
			ROOT->SUB_STATEMENTS.push_back(parse1D_ARRAY());
		else if (tokenSeek(1)->TYPE == TOKEN_EQUALS && tokenSeek(2)->TYPE == TOKEN_REL_LESSTHAN)
			ROOT->SUB_STATEMENTS.push_back(parseARRAY());
		else
			ROOT->SUB_STATEMENTS.push_back(parseID()); 
		break;
		}
                case TOKEN_KEYWORD : {ROOT->SUB_STATEMENTS.push_back(parseKEYWORD()); break;}
	        case TOKEN_RANGE : {ROOT->SUB_STATEMENTS.push_back(parseRANGE()); break;} 
		case TOKEN_TILL : {ROOT->SUB_STATEMENTS.push_back(parseTILL()); break;}
		case TOKEN_FUNCTION : {ROOT->SUB_STATEMENTS.push_back(parseFUNCTION()); break;}
		case TOKEN_CALL : {ROOT->SUB_STATEMENTS.push_back(parseCALL()); break;}
                default : { std::cout << "[!] SYNTAX ERROR " << std::endl; exit(1);}
            }
            
	        bufferScope = std::stoi(current->VALUE);
            proceed(TOKEN_SEMICOLON);

	        if (bufferScope != scopeLog.top())
	        { std::cout << "[!] Indentation Error : Inconsistent indent level 1" << std::endl; exit(1);  }
        }

        return ROOT;

    }
    private:
    
    int limit;
    int index;
    int bufferScope;
    Token * current;
    std::vector <Token *> parserTokens;
    std::stack<int> scopeLog;

    
};



#endif
