#pragma once
#include "database.h"

#include <fstream>
#include <regex>
#include <bits/stdc++.h>
#include "utils.h"
using namespace std;

class Parser {
	public:
		vector<string> static split_text(string input);
		vector<string> static get_select_columns(string cmd);
		static string* split_str(std::string str, char delim);
		vector<string> static get_create_columns(string cmd);
		vector<array<string, 3> > get_where_clause(string cmd);
        vector<string> split_text(string input, string delimeter);
        vector<string> static get_insert_columns(string cmd, string table_name);
        vector<vector<string> > static get_insert_rows(string cmd, string table_name);
        vector<vector<string> > get_update_clause(string cmd);
        std::string static to_lower(std::string s);
};

/// Converts a string to lower
std::string Parser::to_lower(std::string s)
{
    std::for_each(s.begin(), s.end(), [](char &c)
                  { c = ::tolower(c); });

    return s;
}

// IN DEVELOPMENT
vector<array<string, 3> > Parser::get_where_clause(string cmd) {
	smatch sm;
	vector<array<string, 3> > ret;

	regex str_expr("where (.*)");

	if(regex_search(cmd, sm, str_expr)){
		try
		{
			cout << sm[1] << endl;

			//ret = Utils::split(sm[1], ",");

		} catch(const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}

	} else {
		cout << "No Match!" << endl;
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

	//regex str_expr("insert into " + table_name + " \\((.*)\\)", regex::icase);
	regex str_expr(R"(insert into " + table_name + "(?:\s*\()(.*)\)(?:\s*)values)", regex::icase);

	// Check if the match was found, and add to the vector
	if(regex_search(cmd, sm, str_expr)){
		try
		{
			ret = Utils::split(sm[1], ",");

			for(int i = 0; i < sm.size(); i += 1){
				cout << "Column Group:\t" << sm[i] << endl;
			}

		} catch(const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}

	} else {
		cout << "Insert Columns: No Match!" << endl;
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
			for(int i = 0; i < sm.size(); i += 1){
				cout << "Row Group:\t" << sm[i] << endl;
			}

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

	} else {
		cout << "No Match!" << endl;
	}

	return ret;

}

/// Author: Andrew
/// Date: 10-28-2021
/// Splits the provided string on the specified delimiter
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
		cout << "Create Columns: No Match!" << endl;
	}

	return ret;

}

vector<vector<string>> Parser::get_update_clause(string cmd) {
    smatch sm;
    vector<vector<string> > ret;
    vector<string> values;

    regex str_expr("set(?:\s*)(.*)(?:\s*where)");

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

std::vector<std::string> split_text(std::string input, std::string delimeter)
{
    // Declare delimiters
    //string delimeters = " ,.-':;?()+*/%$#!\"@^&";

    // Get string split results
    auto results = Utils::split(input, delimeter);
    return results;
}

