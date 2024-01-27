#ifndef __PARSER_H
#define __PARSER_H

#include "lexer.hpp"
#include <vector>
#include <stack>


enum NODE_TYPE
{
    NODE_ROOT,
    NODE_VARIABLE,
    NODE_RETURN,
    NODE_PRINT,
    NODE_GET,
    NODE_INT,
    NODE_STRING,
    NODE_IF, 
    NODE_ELSE,
    NODE_CONDITION,
    NODE_REL_EQUALS,
    NODE_REL_NOTEQUALS, 
    NODE_REL_LESSTHAN, 
    NODE_REL_LESSTHANEQUALS,
    NODE_REL_GREATERTHAN,
    NODE_REL_GREATERTHANEQUALS,

};

std::string nodeToString(enum NODE_TYPE TYPE)
{
    switch (TYPE)
    {
        case NODE_ROOT : return "NODE_ROOT";
        case NODE_VARIABLE : return "NODE_VARIABLE";
        case NODE_RETURN : return "NODE_RETURN";
        case NODE_PRINT : return "NODE_PRINT";
 	case NODE_GET : return "NODE_GET";
        case NODE_INT : return "NODE_INT";
        case NODE_STRING : return "NODE_STRING";
	case NODE_IF : return "NODE_IF";
	case NODE_ELSE : return "NODE_ELSE";
	case NODE_CONDITION : return "NODE_CONDITION";		       
        case NODE_REL_EQUALS : return "NODE_REL_EQUALS";
	case NODE_REL_NOTEQUALS :  return "NODE_REL_NOTEQUALS";
        case NODE_REL_LESSTHAN : return "NODE_REL_LESSTHAN";
	case NODE_REL_LESSTHANEQUALS : return "NODE_REL_LESSTHANEQUALS";
    	case NODE_REL_GREATERTHAN : return "NODE_REL_GREATERTHAN";
	case NODE_REL_GREATERTHANEQUALS : return "NODE_REL_GREATERTHANEQUALS";
    	default : return "UNRECOGNIZED NODE";
        
    }
}

struct AST_NODE 
{
    enum NODE_TYPE TYPE;
    std::string * VALUE;
    AST_NODE * CHILD;
    std::vector <AST_NODE *> SUB_STATEMENTS; // THIS IS ONLY FOR THE ROOT NODE
    AST_NODE * SUPPLEMENT; // THIS IS FOR TEMPORARY MEASURES				     
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
    AST_NODE * parseINT()
    {
        if (current->TYPE != TOKEN_INT)
        {
            std::cout << "[!] SYNTAX ERROR " << std::endl;
                    exit(1);
        }
        AST_NODE * newNode = new AST_NODE();
        newNode->TYPE = NODE_INT;
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

        return newNode;

    }


    AST_NODE * parseID()
    {
        std::string * buffer = &current->VALUE;
        proceed(TOKEN_ID);
        proceed(TOKEN_EQUALS);

        AST_NODE * newNode = new AST_NODE();
        newNode->VALUE = buffer;
        newNode->TYPE = NODE_VARIABLE;
        
        switch (current->TYPE)
        {
            case TOKEN_INT : {newNode->CHILD = parseINT(); break;}
            case TOKEN_ID : {newNode->CHILD = parseID_RHS(); break;}

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
            std::cout << "[!] Parser Error : the print statement does not have a string linked to it";
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
	      case TOKEN_ID : {newNODE->SUB_STATEMENTS.push_back(parseID_RHS()); break;}
	      case TOKEN_INT : {newNODE->SUB_STATEMENTS.push_back(parseINT()); break;}
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
	      case TOKEN_ID : {newNODE->SUB_STATEMENTS.push_back(parseID_RHS()); break;}
	      case TOKEN_INT : {newNODE->SUB_STATEMENTS.push_back(parseINT()); break;}
	      default : { std::cout << "[!] SYNTAX ERROR : Unexpected Token : " << typeToString(current->TYPE) << std::endl; exit(1);}    	       
      }
        
      return newNODE;
      
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
	
	////////////
	
	
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
    	if (!recursiveCall) proceed (TOKEN_KEYWORD);
        
        //proceed(TOKEN_LEFT_PAREN);
        AST_NODE * newNode = new AST_NODE();
        switch (current->TYPE)
        {
            
            case TOKEN_INT :
            {              
                newNode->TYPE = NODE_PRINT;
                newNode->CHILD = parseINT();

                //proceed(TOKEN_RIGHT_PAREN);

                break;
            }
            
            case TOKEN_ID :
            {
                newNode->TYPE = NODE_PRINT;
                newNode->CHILD = parseID_RHS();

                //proceed(TOKEN_RIGHT_PAREN);

                break;
            }
            case TOKEN_QUOTES :
            {
                
                proceed(TOKEN_QUOTES);
        
                newNode->TYPE = NODE_PRINT;
                newNode->CHILD = parseSTRING();

                proceed(TOKEN_QUOTES);
                //proceed(TOKEN_RIGHT_PAREN);

                break;
            }
            default :
            {
                std::cout << "[!] Parser Error ! Unindentified token : " << typeToString(current->TYPE) << std::endl;
                exit(1); 
            }
            // if the current token is semicolon , then we would just proceed to the normal execution
            // else over here we would make the recursive call to the print parsing (maybe make a seperate subparse function)
            // which could be usefor for the case of recursion
        }
        if (current->TYPE != TOKEN_SEMICOLON)
        {
        newNode->SUB_STATEMENTS.push_back(parsePRINT(true));
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
                case TOKEN_ID : {ROOT->SUB_STATEMENTS.push_back(parseID()); break;}
                case TOKEN_KEYWORD : {ROOT->SUB_STATEMENTS.push_back(parseKEYWORD()); break;}
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
