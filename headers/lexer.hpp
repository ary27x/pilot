#ifndef __LEXER_H
#define __LEXER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

enum type 
{
    TOKEN_ID,
    TOKEN_INT,
    TOKEN_EQUALS,
    TOKEN_SEMICOLON,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_KEYWORD,
    TOKEN_STRING,
    TOKEN_QUOTES,
    TOKEN_COMMA,
    TOKEN_EOF
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
        case TOKEN_KEYWORD : return "TOKEN_KEYWORD";
        case TOKEN_STRING : return "TOKEN_STRING";
        case TOKEN_QUOTES : return "TOKEN_QUOTES";
    	case TOKEN_COMMA : return "TOKEN_COMMA";
    	case TOKEN_EOF : return "TOKEN_EOF";
        default : return "UNRECOGNIZED TOKEN";
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

    void skipNew()
    {
    	while(current == '\n')
    	{
    		lineNumber++;
                characterNumber = 0;
    		advance();
    	}
    }
    
    void checkAndSkip()
    {
        while (current == ' ' || current == '\t' || current == '\r')
        { 
            advance();
        }
    }

    std::vector <std::string> keywords = {"return" , "print" , "get"};

    std::unordered_map <std::string , std::string> translatables = {
    {"display" , "print"}
    };

    std::unordered_map <std::string , enum type> convertibles = {
    	{"is" , TOKEN_EQUALS},
    	
    };

    Token * tokenizeID_KEYWORD()
    {
      std::stringstream buffer;
      buffer << advance(); 

      while (isalnum(current) || current == '_')
      {
        buffer << advance();
      }

      Token * newToken = new Token();
      newToken->VALUE = buffer.str();
      
      if (translatables.find(newToken->VALUE) != translatables.end())
      {
      	      newToken->VALUE = translatables[newToken->VALUE];
      }
          
      if (convertibles.find(newToken->VALUE) != convertibles.end())
      {
	      newToken->TYPE = convertibles[newToken->VALUE];  
      }
      else
      {
      newToken->TYPE = (std::find(keywords.begin() , keywords.end() , newToken->VALUE) != keywords.end()) ? TOKEN_KEYWORD : TOKEN_ID;
      }

      return newToken;

    }
    Token * tokenizeSTRING()
    {
        std::stringstream buffer;
        while (current != '"')
        {
            if (current == '\0')
            {
                std::cout << "[!] Lexer Error : Missing Quotes";
                exit(1);
            }
            buffer << advance();
        }

        Token * newToken = new Token();
        newToken->TYPE = TOKEN_STRING;
        newToken->VALUE = buffer.str();
        
        return newToken;

        }
    Token * tokenizeSPECIAL(enum type TYPE)
    {
        Token * newToken = new Token();
        newToken->TYPE = TYPE;
        newToken->VALUE = std::string(1 , advance());

	if (newToken->VALUE == "\n")
	{
		newToken->VALUE = "\\n";
	}

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
        bool newLine = true;
        
        while (cursor < size && notEOF)
        {
            
            checkAndSkip();
            if(isalpha(current) || current == '_') // this is the logic for ids
            {
                tokens.push_back(tokenizeID_KEYWORD());
                newLine = false;
                continue;
            }

            if(isdigit (current)) // this is the logic for integer literals
            {
                tokens.push_back(tokenizeINT());

                continue;
            }
            switch(current)
            {
                case '\n' :
                {
                    if (newLine)
                    {
                    	skipNew();
                    }
                    else
                    {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_SEMICOLON));
                    lineNumber++;
                    characterNumber = 0;
		    newLine = true;
		    }
                    break;
                }
		case ',' :
		{
		    tokens.push_back(tokenizeSPECIAL(TOKEN_COMMA));
		
		    break;
		}
                case '=' :
                {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_EQUALS));
                
		    break;
                }
                
                case '"' :
                {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_QUOTES));
                    tokens.push_back(tokenizeSTRING());
                    tokens.push_back(tokenizeSPECIAL(TOKEN_QUOTES));
                
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
                case '~':
                {
                    while (current != '\n' &&  current != '\0')
                    {
                        advance();
                    }
                    break;
                }
                case 0 :
                {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_EOF));
                 
                    break;
                }
                
                default:
                {
                    std::cout << "[!] LEXER ERROR : Unidentified symbol " << current <<std::endl ;
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
    bool newLine;

};

#endif
