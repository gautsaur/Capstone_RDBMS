#pragma once
#include <fstream>
#include <regex>
#include <bits/stdc++.h>
#include "utils.h"
using namespace std;

class Parser {
	public:
		//void static check_command(string enteredCommand, string commandToCheck);
		//void static tokenize(std::string const &str, const char* delim, std::vector<std::string> &out);
		//string static splitString(string str, string delimiter);
		vector<string> static split_text(string input);
		//void static read_sql_file();
		vector<string> static get_select_columns(string cmd);
		static string* split_str(std::string str, char delim);
		vector<string> static get_create_columns(string cmd);
		vector<pair<string, string> > get_where_clause(string cmd);
        vector<string> split_text(string input, string delimeter);

};

/// Author: Andrew
/// Date: 09-28-2021
/// Splits the provided string on the specified delimiter
std::string* Parser::split_str(std::string str, char delim){
	int size = std::count(str.begin(), str.end(), delim) + 1;
	std::string* ret = new std::string[size];

	int i = 0;
	std::stringstream ssin(str.substr(str.find(":") + 1));

	while(ssin.good() && i < size) {
		getline(ssin, ret[i], delim);
		++i;
	}

	return ret;
}


vector<string> Parser::get_select_columns(string cmd) {
	smatch sm;
	vector<string> ret;

	regex str_expr("select (.*) from");

	// Check if the match was found, and add to the vector
	if(regex_search(cmd, sm, str_expr)){
		try
		{
			ret = Utils::split(sm[1], ",");

		} catch(const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}

	} else {
		cout << "No Match!" << endl;
	}

	return ret;

}

vector<string> Parser::get_create_columns(string cmd) {
	smatch sm;
	vector<string> ret;

	regex str_expr("\\((.*)\\)");

	// Check if the match was found, and add to the vector
	if(regex_search(cmd, sm, str_expr)){
		try
		{
			ret = Utils::split(sm[1], ",");

		} catch(const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}

	} else {
		cout << "No Match!" << endl;
	}

//	for(auto &v : ret){
//		std::cout << v << std::endl;
//	}

	return ret;

}

std::vector<std::string> split_text(std::string input, std::string delimeter)
{
    // Declare delimiters
    //string delimeters = " ,.-':;?()+*/%$#!\"@^&";

    // Get string split results
    auto results = Utils::split(input, delimeter);
    return results;
}
