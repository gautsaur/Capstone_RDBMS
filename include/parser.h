#pragma once
#include "database.h"

#include <fstream>
#include <regex>
#include <bits/stdc++.h>
#include "utils.h"
#include <iomanip>
using namespace std;

class Parser {
	public:
		vector<string> static split_text(string input);
		vector<string> static get_select_columns(string cmd);
		static string* split_str(std::string str, char delim);
		vector<string> static get_create_columns(string cmd);
		vector<string> static get_where_clause(string cmd, string op);
        vector<string> split_text(string input, string delimeter);
        vector<string> static get_insert_columns(string cmd, string table_name);
        vector<vector<string> > static get_insert_rows(string cmd, string table_name);
        std::string static to_lower(std::string s);
        vector<vector<string> > static get_update_clause(string cmd);
        string static get_conditional(string stm);
        string static get_table_name(string cmd, string first_delim, string second_delim);
};

/// Converts a string to lower
std::string Parser::to_lower(std::string s)
{
    std::for_each(s.begin(), s.end(), [](char &c)
                  { c = ::tolower(c); });

    return s;
}

/// Author: Andrew
/// Date: 11-28-2021
/// Parses an update command
vector<vector<string> > Parser::get_update_clause(string cmd) {
	smatch sm;
	vector<vector<string> > ret;
	vector<string> values;
		
	regex str_expr("set(?:\\s*)(.*)(?:\\s*where)");
	
	if(regex_search(cmd, sm, str_expr)){
		try
		{			
			values = Utils::split(sm[1], ",");
			
			for(string value : values) {
				vector<string> temp = Utils::split("=");
				
				ret.push_back(temp);
				
			}
			
		} catch(const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}
		
	} else {
		cout << "No Match!" << endl;
	}
			
	return ret;
	
}

/// Author: Andrew
/// Date: 11-28-2021
/// Get's the where clause parameters for a select or update statement
vector<string> Parser::get_where_clause(string cmd, string op) {
	smatch sm;
	vector<string> ret;
	vector<string> tmp;
		
	regex str_expr("where (.*)");
	
	if(regex_search(cmd, sm, str_expr)){
		try
		{		
			tmp = Utils::split(Utils::remove_char(sm[1], ';'), op);
			
			for(string str : tmp) {
				ret.push_back(Utils::trim(str));
			}
			
		} catch(const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}
		
	}
			
	return ret;
}

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

/// Author: Andrew
/// Date: 11-15-2021
/// Splits the provided string on the specified delimiter
vector<string> Parser::get_insert_columns(string cmd, string table_name) {
	smatch sm;
	vector<string> ret;
	vector<string> tmp;

	//regex str_expr("insert into " + table_name + " \\((.*)\\)", regex::icase);
	regex str_expr(table_name + "(?:\\s*\\()(.*)\\)(?:\\s*)values", regex::icase);

	// Check if the match was found, and add to the vector
	if(regex_search(cmd, sm, str_expr)){
		try
		{
			tmp = Utils::split(sm[sm.size() - 1], ",");
			
			for(string str : tmp) {
				ret.push_back(Utils::trim(str));
			}
					
		} catch(const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}

	} else {
		cout << "Insert Columns: No Match!" << endl;
	}
	
	return ret;
}

/// Author: Andrew
/// Date: 12-12-2021
/// Gets the conditional statement from a command
string Parser::get_conditional(string stm) {
	string ret = "";
	
	if(Utils::contains(stm, "<=")) {
		ret = "<=";
	} else if(Utils::contains(stm, ">=")) {
		ret = ">=";
	} else if(Utils::contains(stm, "=")) {
		ret = "=";
	} else if(Utils::contains(stm, "<")) {
		ret = "<";
	} else if(Utils::contains(stm, ">")) {
		ret = ">";
	}
	
	return ret;
}

/// Author: Andrew Nunez
/// Date: 11-15-2021
/// Splits the provided string on the specified delimiter
vector<vector<string> > Parser::get_insert_rows(string cmd, string table_name) {
	smatch sm;
	vector<vector<string> > ret;
	cmd.erase(std::remove(cmd.begin(), cmd.end(), '\n'), cmd.end());

	regex str_expr(R"(values(?:\s*)\(([^()]+)\))", regex_constants::icase);

	// Check if the match was found, and add to the vector
	if(regex_search(cmd, sm, str_expr)){
		try
		{		
			vector<string> rows = Utils::split(sm[1], ")(");
			
			for(string row : rows){
				row = Utils::remove_char(row, '\'');
				row = Utils::remove_char(row, '"');
				
				vector<string> values = Utils::split(row, ",");
				
				if(values.size() > 0){					
					ret.push_back(values);
				}
				
			}
			
		} catch(const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}

	} else {
		cout << "Insert Rows: No Match!" << endl;
	}

	return ret;
}

/// Author: Andrew
/// Date: 10-28-2021
/// Splits the provided string on the specified delimiter
vector<string> Parser::get_select_columns(string cmd) {
	smatch sm;
	vector<string> ret;

	regex str_expr("select (.*) from", regex_constants::icase);

	// Check if the match was found, and add to the vector
	if(regex_search(cmd, sm, str_expr)){
		try
		{
			ret = Utils::split(sm[1], ",");

		} catch(const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}

	}

	return ret;

}

/// Author: Andrew
/// Date: 10-28-2021
/// Gets the columns for create table command
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
		cout << "Invalid Command. Can't find Columns." << endl;
	}

	return ret;

}

std::string Parser::get_table_name(string cmd, string first_delim, string second_delim) {
	smatch sm;
	std::string ret;
	string exp = "";
	
	if(second_delim == "(") {
		exp = first_delim + "(?:\\s*)([A-Za-z\\-_]*)(?:\\s*)\\(";
	} else {
		exp = first_delim + "(?:\\s*)([A-Za-z\\-_]*)(?:\\s*)" + second_delim;
	}
	
	regex str_expr(exp, regex_constants::icase);

	// Check if the match was found, and add to the vector
	if(regex_search(cmd, sm, str_expr)){
		try
		{			
			ret = Utils::trim(sm[1]);

		} catch(const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}

	} else {
		cout << "Invalid Command. Can't find table name." << endl;
	}

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

