// ============================================================================
//    Author: Kenneth Perkins
//    Date:   Jan 23, 2021
//    Taken From: http://programmingnotes.org/
//    File:  Utils.h
//    Description: Handles general utility functions
// ============================================================================
#pragma once
#include <vector>
#include <string>

namespace Utils {
	
	std::string WHITESPACE = " \n\r\t\f\v";
	
    /**
    * FUNCTION: split 
    * USE: Splits a string into tokens and saves them into a vector
    * @param source: String to be broken up into substrings (tokens)
    * @param delimiters: String containing the delimiter character(s)
    * @return: A vector containing all the found tokens in the string
    */
    std::vector<std::string> split(const std::string& source, const std::string& delimiters = " ") {
        std::size_t prev = 0;
        std::size_t currentPos = 0;
        std::vector<std::string> results;

        while ((currentPos = source.find_first_of(delimiters, prev)) != std::string::npos) {
            if (currentPos > prev) {
                results.push_back(source.substr(prev, currentPos - prev));
            }
            prev = currentPos + 1;
        }
        if (prev < source.length()) {
            results.push_back(source.substr(prev));
        }
        return results;
    }
    
    std::string remove_char(std::string str, char delim)
	{
	    str.erase(std::remove(str.begin(), str.end(), delim), str.end());
	
	    return str;
	}
    
	std::string ltrim(const std::string &s)
	{
	    size_t start = s.find_first_not_of(WHITESPACE);
	    return (start == std::string::npos) ? "" : s.substr(start);
	}
	 
	std::string rtrim(const std::string &s)
	{
	    size_t end = s.find_last_not_of(WHITESPACE);
	    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
	}
	 
	std::string trim(const std::string &s) {
	    return rtrim(ltrim(s));
	}	
	
}// http://programmingnotes.org/
