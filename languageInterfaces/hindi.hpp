#ifndef __LANGUAGE_HINDI_H
#define __LANGUAGE_HINDI_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <regex>

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

// tranlsate the display sub functions

void hindiToPilot(std::string& sourceCode) // write better logic to not convert hindi words or numerals which are in quotes
{
    int i;
    char s;
    for (auto& hindiNumeral : hindiNumeralToEnglishNumeral) 
        sourceCode = std::regex_replace(sourceCode, std::regex(hindiNumeral.first),  hindiNumeral.second);

    for (auto& hindiDoubleKeyword : hindiDoubleKeywordToEnglish)  
        sourceCode = std::regex_replace(sourceCode, std::regex(hindiDoubleKeyword.first),  hindiDoubleKeyword.second); 

    for (auto& hindiKeyword : hindiKeywordToEnglish)  
        sourceCode = std::regex_replace(sourceCode, std::regex(hindiKeyword.first),  hindiKeyword.second); 

}

#endif
