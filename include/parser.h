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

//void check_command(string enteredCommand, string commandToCheck)
//{
//    if (enteredCommand.find(commandToCheck) != string::npos)
//    {
//        std::cout << "found!" << '\n';
//    }
//}
//
//
//void tokenize(std::string const &str, const char* delim,
//            std::vector<std::string> &out)
//{
//    char *token = strtok(const_cast<char*>(str.c_str()), delim);
//    while (token != nullptr)
//    {
//        out.push_back(std::string(token));
//        token = strtok(nullptr, delim);
//    }
//
//}
//
//string splitString(string str, string delimiter)
//{
//    int start = 0;
//    int end = str.find(delimiter);
//    while (end != -1) {
//        cout << str.substr(start, end - start) << endl;
//        start = end + delimiter.size();
//        end = str.find(delimiter, start);
//    }
//    return str.substr(start, end - start);
//}
//
//
//vector<string> split_text(string input)
//{
//    // Declare delimiters
//    string delimeters = " ,.-':;?()+*/%$#!\"@^&";
//
//    // Get string split results
//    auto results = Utils::split(input, delimeters);
//    return results;
//}

//void Parser::read_sql_file()
//{
//    fstream newfile;
//    newfile.open("../data/test.sql", ios::in); //open a file to perform read operation using file object
//    if (newfile.is_open())
//    { //checking whether the file is open
//        string tp;
//
//        // attach current database if not there
//        Database *db;
//        Table *tbl;
//        while (getline(newfile, tp))
//        {
//            cout<<"\nCurrent Line"<<tp;
//            if(tp.find("CREATE DATABASE")!=string::npos){
//                string databaseName = splitString(tp, "CREATE DATABASE");
//                db = new Database(databaseName);
//            }
//
//            if(tp.find("CREATE TABLE")!= string::npos){
//               string tableName = splitString(tp, "CREATE TABLE");
//               tbl = new Table(tableName);
//
//               db->AddTable(*tbl);
//                while (getline(newfile, tp) && tp != ");")
//                {
//                    vector<string> temp = split_text(tp);
//                    if (temp.size() == 0){
//                        continue;
//                    }
//                    cout << "\nColumn Type: "<<temp[1] << "|| ColumnName: " <<temp[0];
//                    string colType;
//
//                    if (temp[1]=="varchar" || temp[1] == "char" || temp[1]=="string"){
//                        colType = "string";
//                    }
//                    else{
//                        colType ="int";
//                    }
//
//                    tbl->columns.insert({temp[1], temp[0]});
//                }   
//            }
//
//            if(tp.find("INSERT INTO")!=string::npos){
//                string tableName = splitString(tp, "INSERT INTO");
//                vector<string> insertArrangement = split_text(tableName);
//
//                for(int i = 0; i<insertArrangement.size(); i++) {
//                    cout << insertArrangement[i] << endl;
//                }
//                while (getline(newfile, tp))
//                {
//                    string tableName = splitString(tp, "VALUES");
//                    vector<string> insertArrangement = split_text(tableName);
//                    tbl->Insert(insertArrangement);
//                }   
//            }
//
//            //print the data of the string
//        }
//        newfile.close(); //close the file object.
//    }
//}
