#include <iostream>
#include "../include/database.h"
#include <windows.h>
#include <string>
#include <stdlib.h>
#include <regex>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void color(int s);
void setup_intro();
void show_help();
void print_rows(std::map<std::string, std::string> cols, std::vector<std::string, std::string> rows);
std::string to_lower(std::string str);
void remove_char(std::string str, char delim);
HANDLE h = GetStdHandle( STD_OUTPUT_HANDLE );
std::string current_db_name;
Table* create_table(Database *db, std::string table_name);
std::string table_name;
void print_rows(std::vector<std::vector<std::string> > rows);
Database* create_db(Database *db, std::string db_name);
std::string db_name;
Table insert_into(std::string table_name, std::map<std::string, std::string> columns);


int main(int argc, char** argv) {
	std::string cmd;
	
	setup_intro();
	
	Database *db;
	
	while(to_lower(cmd) != "exit") {
		cmd = "";
		
		// Setup the command to wait for input
		color(10);
		if(current_db_name.length() > 0) {
			std::cout << current_db_name << "@";
		}		
		std::cout << "SQL>";
		color(7);
		std::getline(std::cin, cmd);
		
		// SELECT [ID, TEST, ] FROM TABLE;
		
		// Do something with cmd
		if(tolower(cmd.find("open database ") == 0)){
			current_db_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
		 	db = new Database(current_db_name);
			
			if(db->database_name != current_db_name){
				current_db_name = "";
			}
			
		} else if (to_lower(cmd) == "help") {
			show_help();
		} else if (to_lower(cmd) == "list database") {
			Database::List();
		}
		  else if (tolower(cmd.find("select ") == 0)) {
			// Parses the select command
			try	{
				//std::regex rgx("[\n\r].* from \s*([^\n\r]*)");
				//std::smatch match;
				std::string stmt = to_lower(cmd);
				
				std::string tbl_name = stmt.substr(stmt.find("from ") + 5);
				
				remove_char(tbl_name, ';');
				
				auto tbl = find_if(db->tables.begin(), db->tables.end(), [&tbl_name](const Table& obj) {
					return obj.table_name == tbl_name;
				});
				
				//db->tables->
				
				//if(std::regex_search(stmt.begin(), stmt.end(), match, rgx)){
					//std::string tbl_name = match[1];
					
					std::cout << "SELECT FROM: " << tbl_name << std::endl; 
				//} else {
					//cout << "Invalid Command!"; << endl;
				//}
				
				
			} catch(const std::exception&) {
				
			}
		 	
		
		}  else if (to_lower(cmd) == "exit"){
			std::cout << "Good Bye" << std::endl;
		} else if(tolower(cmd.find("create table ") == 0)){
		    table_name = cmd.substr(cmd.find_last_of(' ' ) + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
		    //Table *tbl = new create_table(current_db_name, table_name, );

		} else if(tolower(cmd.find("create database")==0)){
            db_name = cmd.substr(cmd.find_last_of(' ' ) + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
		}else if(tolow
		er(cmd.find("insert into")==0)){
            table_name = cmd.substr(cmd.find_last_of(' ' ) + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
		}else {
			std::cout << "Invalid Command." << std::endl;
		}
	}
			
	return 0;
}

/// Converts a string to lower
std::string to_lower(std::string s){	
	std::for_each(s.begin(), s.end(), [](char & c){
    	c = ::tolower(c);
	});
	
	return s;	
}

void remove_char(std::string str, char delim){
	int j = 0;
	int len = str.length();

    for (int i = 0; i < len; i++)
    {
        if (str[i] == delim)
        {
            continue;
        }
        else
        {
            str[j] = str[i];
            j++;
        }
    }

    str[j] = '\0';
}

void print_rows(std::map<std::string, std::string> cols, std::vector<std::string, std::string> rows){
	
}

/// Shows the help menu
void show_help() {
	std::cout << "Available Commands:" << std::endl;
	std::cout << "OPEN DATABASE [name] 	- Check if the database exists and open it." << std::endl;
	std::cout << "CREATE DATABASE 	- Creates and new database and opens a connection to it." << std::endl; 
	std::cout << "DROP DATABASE 		- Deletes the given database." << std::endl; 
	std::cout << "CREATE TABLE 		- Creates a table in the current database." << std::endl;
	std::cout << "DROP DATABASE 		- Check if the database exists and open it." << std::endl; 
	std::cout << "SELECT [] FROM [] 	- Selects the specified columns from the table." << std::endl; 
	std::cout << "UPDATE TABLE 		- Updates the columns and meta for the given table." << std::endl; 
	std::cout << "DELETE FROM 		- Deletes the sepcified data from the table." << std::endl; 
	std::cout << "INSERT INTO 		- Inserts the data into the table." << std::endl; 
	std::cout << "LIST DATABASE 		- Lists the current database names." << std::endl; 
	// List Tables
	// Display Schema
}

/// Setups the intro, emulating a startup
void setup_intro() {
	std::cout << "ISU RDBMS Project" << std::endl;
	std::cout << "Opening RDBMS Shell.";
	Sleep(400);
	std::cout << ".";
	Sleep(400);
	std::cout << ".";
	Sleep(400);
	std::cout << ".";
	Sleep(400);
	std::cout << ".";
	Sleep(400);
	std::cout << ".";
	
	std::cout << std::endl << std::endl << "Success! Here is your shell." << std::endl << "Type [help] for a list of commands. Type [exit]to quit." << std::endl << std::endl;
}

/// Sets the color of the output window
void color( int s){
	SetConsoleTextAttribute( h, s );
}

void print_rows(std::vector<std::vector<std::string> > rows) {
	
}

Table* create_table(Database *db, std::string table_name, std::map<std::string, std::string> columns){
    // (create table) (test_table) (id int, name string)
    Table *tbl = new Table(table_name);

    // Set the columns
    for(std::map<std::string, std::string>::iterator it = columns.begin(); it != columns.end(); ++it) {
        std::string key = it->first;
        std::string value = it->second;
        tbl->columns.insert({key, value});
    }

    //tbl->columns.insert({"int", "id"});
	//tbl->columns.insert({"string", "name"});

	// insert into test_table (id, name) values((1, "test 1"), (2, "test 2"))
	// Insert the rows
	std::vector<std::string> v1 = {1, "test 1"};
	std::vector<std::string> v2 = {2, "test 2"};
	tbl->Insert(v1);
	tbl->Insert(v2);

	// Add the table to the database
	db->AddTable(*tbl);

	return tbl;
}

Database* create_db(Database *db, std::string db_name){
	Database *cr = new Database(db_name);
	db->Save();
	return cr;
}

Table insert_into(std::string table_name, std::map<std::string, std::string> columns){
	
    std::vector<std::string> v1
	columns.Insert(v1);
	return columns;
}