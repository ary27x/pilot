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
    NODE_INT
};

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
            std::cout << "[!] SYNTAX ERROR " << std::endl;
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
    AST_NODE * parseID()
    {
        std::string * buffer = &current->VALUE;
        proceed(TOKEN_ID);
        proceed(TOKEN_EQUALS);

        AST_NODE * newNode = new AST_NODE();
        newNode->TYPE = NODE_VARIABLE;
        newNode->CHILD = parseINT();

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

    AST_NODE * parsePRINT()
    {
        proceed(TOKEN_KEYWORD);
        AST_NODE * newNode = new AST_NODE();
        newNode->TYPE = NODE_PRINT;
        proceed(TOKEN_LEFT_PAREN);
        newNode->CHILD = parseINT();
        proceed(TOKEN_RIGHT_PAREN);

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