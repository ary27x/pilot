#ifndef __LEXER_H
#define __LEXER_H
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

enum type 
{
    TOKEN_ID,
    TOKEN_INT,
    TOKEN_EQUALS,
    TOKEN_SEMICOLON,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN
};

struct Token
{
    enum type TYPE;
    std::string VALUE;
};


std::string typeToString(enum type TYPE)
{
    switch(TYPE)
    {
        case TOKEN_ID : return "TOKEN_ID";
        case TOKEN_INT : return "TOKEN_INT";
        case TOKEN_EQUALS : return "TOKEN_EQUALS";
        case TOKEN_SEMICOLON : return "TOKEN_SEMICOLON";
        case TOKEN_LEFT_PAREN : return "TOKEN_LEFT_PAREN";
        case TOKEN_RIGHT_PAREN : return "TOKEN_RIGHT_PAREN";
        
    }
}

class Lexer
{
    public:
    Lexer(std::string sourceCode)
    {
        source = sourceCode;
        cursor = 0;
        size = sourceCode.length();
        current = sourceCode.at(cursor);
        lineNumber = 1;
        characterNumber = 1;
    }

    char advance () 
    {

        if (cursor < size)
        {
            char temp = current;
            cursor++;
            characterNumber++;
            current = (cursor < size) ? source[cursor] : '\0';
            return temp;
        }
        else{
            return '\0';
        }
    }

    void checkAndSkip()
    {
        while (current == ' ' || current == '\n' || current == '\t' || current == '\r')
        {
            if (current == '\n')
            {
                lineNumber++;
                characterNumber = 0;
            }
            
            advance();
        }
    }

    Token * tokenizeID()
    {
      std::stringstream buffer;
      buffer << advance(); 

      while (isalnum(current) || current == '_')
      {
        buffer << advance();
      }

      Token * newToken = new Token();

      newToken->TYPE = TOKEN_ID;
      newToken->VALUE = buffer.str();

      return newToken;

    }
    Token * tokenizeSPECIAL(enum type TYPE)
    {
        Token * newToken = new Token();
        newToken->TYPE = TYPE;
        newToken->VALUE = std::string(1 , advance());

        return newToken;
    }
    Token * tokenizeINT()
    {
        std::stringstream buffer;
        while (isdigit(current))
        {
            buffer << advance();
        }

        Token * newToken = new Token();
        newToken->TYPE = TOKEN_INT;
        newToken->VALUE = buffer.str();

        return newToken;
    }
    std::vector<Token *> tokenize()
    {
        std::vector<Token *> tokens;
        
        bool notEOF = true;
        while (cursor < size && notEOF)
        {
            
            checkAndSkip();
            if(isalpha(current) || current == '_') // this is the logic for ids
            {
                tokens.push_back(tokenizeID());
                continue;
            }

            if(isdigit (current)) // this is the logic for integer literals
            {
                tokens.push_back(tokenizeINT());

                continue;
            }
            switch(current)
            {
                case ';' :
                {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_SEMICOLON));

                    break;
                }
                case '=' :
                {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_EQUALS));

                    break;
                }
                
                case '(' :
                {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_LEFT_PAREN));

                    break;
                }
                case ')' :
                {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_RIGHT_PAREN));
                 
                    break;
                }
                default:
                {
                    std::cout << "[!] PARSER ERROR : Unidentified symbol " << current <<std::endl ;
                    std::cout << "LINE NUMBER : " << lineNumber << " CHARACTER NUMBER : " << characterNumber <<std::endl; 
                    exit(1);
                }
                


            }
        }
        return tokens;
    }

    char peak (int offset = 0)
    {
        if (cursor + offset < size)
        {
            return source[cursor + offset];
        }
        else{
            return '\0';
        }
    }

    private:
    std::string source;
    int cursor;
    int size;
    char current;
    int lineNumber;
    int characterNumber;

};

#endif
