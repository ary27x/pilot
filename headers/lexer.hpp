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
    TOKEN_ARE,
    TOKEN_SEMICOLON,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_SQR,
    TOKEN_RIGHT_SQR,
    TOKEN_KEYWORD,
    TOKEN_STRING,
    TOKEN_QUOTES,
    TOKEN_COMMA,
    TOKEN_FUNCTION,
    TOKEN_CALL,
    TOKEN_ARGUMENTS,
    TOKEN_REL_EQUALS,
    TOKEN_REL_NOTEQUALS,
    TOKEN_REL_LESSTHAN,
    TOKEN_REL_LESSTHANEQUALS,
    TOKEN_REL_GREATERTHAN,
    TOKEN_REL_GREATERTHANEQUALS,
    TOKEN_MATH,
    TOKEN_RANGE,
    TOKEN_TILL, 
    TOKEN_TO,
    TOKEN_AS,
    TOKEN_REFERENCE,
    TOKEN_INDENT,
    TOKEN_DOT,
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
	    case TOKEN_ARE : return "TOKEN_ARE";
	    case TOKEN_SEMICOLON : return "TOKEN_SEMICOLON";
        case TOKEN_LEFT_PAREN : return "TOKEN_LEFT_PAREN";
        case TOKEN_RIGHT_PAREN : return "TOKEN_RIGHT_PAREN";
	    case TOKEN_LEFT_SQR : return "TOKEN_LEFT_SQR";
	    case TOKEN_RIGHT_SQR : return "TOKEN_RIGHT_SQR";
        case TOKEN_KEYWORD : return "TOKEN_KEYWORD";
        case TOKEN_STRING : return "TOKEN_STRING";
        case TOKEN_QUOTES : return "TOKEN_QUOTES";
    	case TOKEN_COMMA : return "TOKEN_COMMA";
	    case TOKEN_FUNCTION : return "TOKEN_FUNCTION";
        case TOKEN_CALL : return "TOKEN_CALL";
	    case TOKEN_ARGUMENTS : return "TOKEN_ARGUMENTS";
	    case TOKEN_REL_EQUALS : return "TOKEN_REL_EQUALS";
	    case TOKEN_REL_NOTEQUALS : return "TOKEN_REL_NOTEQUALS";				   
	    case TOKEN_REL_LESSTHAN : return "TOKEN_REL_LESSTHAN";
	    case TOKEN_REL_LESSTHANEQUALS : return "TOKEN_REL_LESSTHANEQUALS";
	    case TOKEN_REL_GREATERTHAN : return "TOKEN_REL_GREATERTHAN";
        case TOKEN_REL_GREATERTHANEQUALS : return "TOKEN_REL_GREATERTHANEQUALS";	
	    case TOKEN_MATH : return "TOKEN_MATH";
	    case TOKEN_RANGE : return "TOKEN_RANGE";
        case TOKEN_TILL : return "TOKEN_TILL";
        case TOKEN_TO : return "TOKEN_TO";
	    case TOKEN_AS : return "TOKEN_AS";
	    case TOKEN_REFERENCE : return "TOKEN_REFERENCE";
        case TOKEN_INDENT : return "TOKEN_INDENT";
    	case TOKEN_EOF : return "TOKEN_EOF";
        case TOKEN_DOT : return "TOKEN_DOT";
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
    
    char seek (int offset)
    {
	if (cursor + offset >= size)
		return 0;
	else
	   return source[cursor + offset];
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
        else
            return '\0';
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
            advance();
    }
    bool isAlphaWrapper()
    {
        if (isalpha(current))
            return true;
        else if ((int) current < 0)
            return true;
        else return false;
    }

    bool isAlnumWrapper()
    {
        if (isalnum(current))
            return true;
        else if ((int) current < 0)
            return true;
        else return false;
    }


    std::vector <std::string> keywords = {"return" , "print" , "get","if" , "else" , "exit" , "clear" , "sleep"};

    std::unordered_map <std::string , std::string> translatables = {
    {"display" , "print"}
    };

    std::unordered_map <std::string , enum type> convertibles = {
    {"is" , TOKEN_EQUALS},
	{"plus" , TOKEN_MATH},
	{"minus" , TOKEN_MATH},
	{"into" , TOKEN_MATH},
	{"mod" , TOKEN_MATH},
	{"times" , TOKEN_MATH},
	{"by" , TOKEN_MATH},
	{"range" , TOKEN_RANGE},
	{"till" , TOKEN_TILL},
	{"to" , TOKEN_TO},
	{"as" , TOKEN_AS},
	{"function" , TOKEN_FUNCTION},
	{"call" , TOKEN_CALL},
	{"are"  , TOKEN_ARE}
    };

    Token * tokenizeID_KEYWORD()
    {
        std::stringstream buffer;
        buffer << advance(); 

        while (isAlnumWrapper() || current == '_')
            buffer << advance();

        Token * newToken = new Token();
        newToken->VALUE = buffer.str();
      
        if (translatables.find(newToken->VALUE) != translatables.end())
      	    newToken->VALUE = translatables[newToken->VALUE];
        if (convertibles.find(newToken->VALUE) != convertibles.end())
	        newToken->TYPE = convertibles[newToken->VALUE];  
        else
            newToken->TYPE = (std::find(keywords.begin() , keywords.end() , newToken->VALUE) != keywords.end()) ? TOKEN_KEYWORD : TOKEN_ID;

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
		    newToken->VALUE = "\\n";

        return newToken;
    }
    
    Token * tokenizeSEMICOLON (int scope)
    {
	    Token * newToken = new Token();
	    newToken->TYPE = TOKEN_SEMICOLON;
	    newToken->VALUE = std::to_string(scope);
	    return newToken;
    }
    
    Token * tokenizeINT()
    {
        std::stringstream buffer;

        while (isdigit(current))
            buffer << advance();

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
	    int scopeCounter;
        
        while (cursor < size && notEOF)
        {
            checkAndSkip();
            if(isAlphaWrapper() || current == '_') 
            {
                tokens.push_back(tokenizeID_KEYWORD());
                newLine = false;
                continue;
            }
            if(isdigit (current)) 
            {
                tokens.push_back(tokenizeINT());
                continue;
            }
            switch(current)
            {
                case '\n' :
                {
                    if (newLine)
                    	skipNew();
                    else
                    {
		                advance();
		                scopeCounter = 0;
		                // MAKE CHANGES TO ADD SUPPORT FOR TAB INDENTATION
		                while (current == '\n' || current == ' ')
		                {
			                if (current == '\n')
				                scopeCounter = 0;
		                    else 
				                scopeCounter++;
			                advance();
		                }
			    
                        tokens.push_back(tokenizeSEMICOLON(scopeCounter));
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
		            if (seek(1) == '=')
                    {
			            advance();
			            tokens.push_back(tokenizeSPECIAL(TOKEN_REL_EQUALS));
			        }
		            else
                        tokens.push_back(tokenizeSPECIAL(TOKEN_EQUALS));
		            break;
                }
		        case '!' :
		        {
			        if (seek(1) == '=')
			        {
			        advance();
			        tokens.push_back(tokenizeSPECIAL(TOKEN_REL_NOTEQUALS));
			        break;
			        }
			        std::cout << "[!] Syntax Error : unexpected symbol : " << seek(1) << " expected  : = " << std::endl;
			        exit(1);
		        }
			    case '[':
			    {
				    tokens.push_back(tokenizeSPECIAL(TOKEN_LEFT_SQR));
				    break;
			    }
			    case ']':
			    {
				    tokens.push_back(tokenizeSPECIAL(TOKEN_RIGHT_SQR));
				    break;
			    }
			    case '@' :
			    {
				    tokens.push_back(tokenizeSPECIAL(TOKEN_REFERENCE));
				    break;
			    }   
		        case '<':
		        {
			        if (seek(1) == '=')
			        {
				        advance();
				        tokens.push_back(tokenizeSPECIAL(TOKEN_REL_LESSTHANEQUALS));
				        break;
			        }
			        else 
			        {
				        tokens.push_back(tokenizeSPECIAL(TOKEN_REL_LESSTHAN));
				        break;
			        }
		        }
		        case '>':
		        {
			        if (seek(1) == '=')
			        {
				        advance();
				        tokens.push_back(tokenizeSPECIAL(TOKEN_REL_GREATERTHANEQUALS));
				        break;
			        }
			        else
			        {
				        tokens.push_back(tokenizeSPECIAL(TOKEN_REL_GREATERTHAN));
				        break;
			        }
		        } 
		        case '.':
			    {
				    tokens.push_back(tokenizeSPECIAL(TOKEN_DOT));
				    break;
			    }	
		        case ':' :
		        {
                    if (seek(1) == ':')
				    {
					    advance();
					    tokens.push_back(tokenizeSPECIAL(TOKEN_ARGUMENTS));
					    break;
				    }
				    else
				    {
			            tokens.push_back(tokenizeSPECIAL(TOKEN_INDENT));
        			    break;
				    }
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
                case '+' :
                {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_MATH));
                    break;
                }
                case '-' :
                {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_MATH));
                    break;
                }
                case '*' :
                {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_MATH));
                    break;
                }
                case '/' :
                {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_MATH));
                    break;
                }
                case '%' :
                {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_MATH));
                    break;
                }
                case '~':
                {
                    while (current != '\n' &&  current != '\0')
                        advance();
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
            return source[cursor + offset];
        else
            return '\0';
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
