#ifndef __PARSER_H
#define __PARSER_H

#include "lexer.hpp"
#include <vector>


enum NODE_TYPE
{
    NODE_ROOT,
    NODE_VARIABLE,
    NODE_RETURN,
    NODE_PRINT,
    NODE_GET,
    NODE_INT,
    NODE_STRING
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
        default : return "UNRECOGNIZED NODE";
        
    }
}

struct AST_NODE 
{
    enum NODE_TYPE TYPE;
    std::string * VALUE;
    AST_NODE * CHILD;
    std::vector <AST_NODE *> SUB_STATEMENTS; // THIS IS ONLY FOR THE ROOT NODE
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
        else
        {
             std::cout << "[!] SYNTAX ERROR , UNDEFINED KEYWORD " << std::endl;
                    exit(1);
        }
    }
    AST_NODE * parse()
    {
        AST_NODE * ROOT = new AST_NODE();
        ROOT->TYPE = NODE_ROOT;

        while(current->TYPE != TOKEN_EOF)
        {
            switch (current->TYPE)
            {
                case TOKEN_ID : {ROOT->SUB_STATEMENTS.push_back(parseID()); break;}
                case TOKEN_KEYWORD : {ROOT->SUB_STATEMENTS.push_back(parseKEYWORD()); break;}
                default : {
                    std::cout << "[!] SYNTAX ERROR " << std::endl;
                    exit(1);
                }
            }
            proceed(TOKEN_SEMICOLON);
        }

        return ROOT;

    }
    private:
    int limit;
    int index;
    Token * current;
    std::vector <Token *> parserTokens;


};



#endif
