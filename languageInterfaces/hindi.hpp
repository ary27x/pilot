#ifndef __LANGUAGE_HINDI_H
#define __LANGUAGE_HINDI_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <regex>

std::vector <std::string> stringBufferVector;
std::unordered_map <std::string , std::string> hindiDoubleKeywordToEnglish = 
{
    {"जब तक" , "till"},
    {"नहीं तोह" , "else"}
};

std::unordered_map <std::string , std::string> hindiKeywordToEnglish = 
{
    {"है"     ,   "is"},
    {"प्रदर्शन" ,   "display"},
    {"डिस्प्ले"  ,   "display"},
    {"प्राप्त"  ,    "get"},
    {"कार्य"   ,   "function"},
    {"कॉल"   ,    "call"},
    {"क्रम"   ,   "range"},
    {"से"     ,   "to"},
    {"जैसे"    ,   "as"},
    {"अगर"   ,    "if"},
    {"लाल"  ,  "red"}, 
    {"हरा"   ,  "green"} ,
    {"पीला"  ,  "yellow"},
    {"नीला"  ,  "blue"},
    {"सफ़ेद"  ,  "white"},
    {"काला" , "black"},
    {"एक_साथ" , "inline"},
    {"पीछे_लाल" , "background_red"},
    {"पीछे_हरा" , "background_green"},
    {"पीछे_पीला" , "background_yellow"},
    {"पीछे_नीला" , "background_blue"},
    {"पीछे_सफ़ेद" , "background_white"},   
};

std::unordered_map <std::string , std::string> hindiNumeralToEnglishNumeral = 
{
    {"१" , "1"},
    {"२" , "2"},
    {"३" , "3"},
    {"४" , "4"},
    {"५" , "5"},
    {"६" , "6"},
    {"७" , "7"},
    {"८" , "8"},
    {"९" , "9"},
    {"०" , "0"}
};

void checkQuotesBalance(std::string& sourceCode)
{
    int quotesCount = 0;
    for (char itr : sourceCode)
    {
        if (itr == '\"')
            quotesCount++;
    }
    if (quotesCount % 2 != 0)
    {
        std::cout << "[!] Error , Unterminated quotes found in the source code !";
        exit(0);
    }
}


void hindiToPilot(std::string& sourceCode)
{
    checkQuotesBalance(sourceCode);
    int i;
    char s;
    int itrIndex;
    char itrChar;
    int stringKey;
    std::string stringBuffer;

    for (itrIndex = 0 ; itrIndex < sourceCode.length() ; itrIndex++)
    {
    	itrChar = sourceCode[itrIndex];
    	if (itrChar == '\"')
    	{
            itrIndex++;
            itrChar = sourceCode[itrIndex];
            stringKey = itrIndex;
            stringBuffer = "";
    	    while (true)
    	    {
    	    if (itrIndex == sourceCode.length() || sourceCode[itrIndex] == '\"')
    	    	break;
            
            stringBuffer += itrChar;
    	    sourceCode[itrIndex] = ' ';
            itrIndex++;
    	    itrChar = sourceCode[itrIndex];
    	    }
            stringBufferVector.push_back(stringBuffer);
    	}
    }
   
    for (auto& hindiNumeral : hindiNumeralToEnglishNumeral) 
        sourceCode = std::regex_replace(sourceCode, std::regex(hindiNumeral.first),  hindiNumeral.second);

    for (auto& hindiDoubleKeyword : hindiDoubleKeywordToEnglish)  
        sourceCode = std::regex_replace(sourceCode, std::regex(hindiDoubleKeyword.first),  hindiDoubleKeyword.second); 

    for (auto& hindiKeyword : hindiKeywordToEnglish)  
        sourceCode = std::regex_replace(sourceCode, std::regex(hindiKeyword.first),  hindiKeyword.second); 
    
    int stringCounter = 0;
    int supplementaryIndex;
    for (itrIndex = 0 ; itrIndex < sourceCode.length() ; itrIndex++)
    {
        if (sourceCode[itrIndex] == '\"')
        {
            itrIndex++;
            supplementaryIndex = 0;
            while (supplementaryIndex < stringBufferVector[stringCounter].length())
            {
                sourceCode[itrIndex] = stringBufferVector[stringCounter][supplementaryIndex];
                itrIndex++;
                supplementaryIndex++;
            }
            stringCounter++;
        }

    }

}

#endif
