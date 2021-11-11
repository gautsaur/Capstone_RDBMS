#include <iostream>
#include "database.h"
#include <windows.h>
#include <string>
#include <stdlib.h>
#include <regex>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void color(int s);
void setup_intro();
void show_help();
//void print_rows(Table tbl);
std::string to_lower(std::string str);
std::string remove_char(std::string str, char delim);
HANDLE h = GetStdHandle( STD_OUTPUT_HANDLE );
std::string current_db_name;
Table* create_table(Database *db, std::string table_name);
std::string table_name;


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

		std::string statement = to_lower(cmd);

		// SELECT [ID, TEST, ] FROM TABLE;


		// Do something with cmd
		if(statement.find("open database ") == 0){
			current_db_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
		 	db = new Database(current_db_name);

			if(db->database_name != current_db_name){
				current_db_name = "";
			}

		}else if (statement.find("create database") == 0){
		    current_db_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
            db = new Database(current_db_name);
		}
		else if (statement == "help") {
			show_help();
		} else if (statement == "list database") {
			Database::List();
		} else if (statement == "list table") {
			db->ListTables();
		}  else if (statement.find("select ") == 0) {
			// Parses the select command
			try	{

				std::string tbl_name = statement.substr(statement.find(" from") + 6);

				tbl_name = remove_char(tbl_name, ';');

				Table tbl = db->get_table(tbl_name);

				std::cout << tbl.table_name << std::endl;

				//print_rows(tbl);

			} catch(const std::exception&) {

			}

		}  else if (statement == "exit"){
			std::cout << "Good Bye" << std::endl;
		} else if(statement.find("create table ") == 0){
		    table_name = statement.substr(cmd.find_last_of(' ' ) + 1, statement.find_last_of(';') - statement.find_last_of(' ') - 1);
		    Table *tbl = new Table(table_name);
		    db->AddTable(*tbl);
		}
		else if(statement.find("drop table ") == 0){
		    table_name = statement.substr(cmd.find_last_of(' ' ) + 1, statement.find_last_of(';') - statement.find_last_of(' ') - 1);
            db->DropTable(table_name);
		}
		else {
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

std::string remove_char(std::string str, char delim){
	str.erase(std::remove(str.begin(), str.end(), delim), str.end());

	return str;

}

//void print_rows(Table *tbl){
	//for(std::map<std::string, std::string> column : tbl->columns){
      //  std::cout << column->first << " | " << std::endl;
	//}
//}

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

