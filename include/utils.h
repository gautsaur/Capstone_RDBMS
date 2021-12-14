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
    
    /// Author: Andrew Nunez
	/// 10-20-2021
	/// Removes the specified char from the string
    std::string remove_char(std::string str, char delim)
	{
	    str.erase(std::remove(str.begin(), str.end(), delim), str.end());
	
	    return str;
	}
    
    /// Author: Andrew Nunez
	/// 10-20-2021
	/// Trims whitespace from the left side of the string
	std::string ltrim(const std::string &s)
	{
	    size_t start = s.find_first_not_of(WHITESPACE);
	    return (start == std::string::npos) ? "" : s.substr(start);
	}
	 
	/// Author: Andrew Nunez
	/// 10-20-2021
	/// Trims whitespace from the right side of the string
	std::string rtrim(const std::string &s)
	{
	    size_t end = s.find_last_not_of(WHITESPACE);
	    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
	}
	 
	/// Author: Andrew Nunez
	/// 10-20-2021
	/// Trims whitespace from both sides of the string
	std::string trim(const std::string &s) {
	    return rtrim(ltrim(s));
	}	
	
	/// Author: Andrew Nunez
	/// 12-04-2021
	/// Determines if a string is a substring of another
	bool contains(std::string str, std::string subStr){
		bool ret = false;
		
		if (str.find(subStr) != std::string::npos) {
		    ret = true;
		}
		
		return ret;
		
	}
	
	/// Author: Andrew Nunez
	/// 12-08-2021
	/// Get a the first occurance of a string between the first occurence of two other strings
	std::string get_string_between_two_strings(const std::string str, const std::string start, const std::string end)
	{
		signed first_delim_pos = str.find(start);
	    unsigned end_pos_of_first_delim = first_delim_pos + start.length();
	    unsigned last_delim_pos = str.find(end);
	 
	    return trim(str.substr(end_pos_of_first_delim,
	            last_delim_pos - end_pos_of_first_delim));
	}
	
}
