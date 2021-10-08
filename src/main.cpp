<<<<<<< Updated upstream
#include <iostream>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	
	std::cout << "Hello World!";
	
	return 0;
}
=======
#include "../include/database.h"
#include <windows.h>
#include <string>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void example_write();
void example_read(std::string db_name);
Table* create_table(Database *db, std::string table_name);
void color(int s);
void setup_intro();
std::string to_lower(std::string str);
HANDLE h = GetStdHandle( STD_OUTPUT_HANDLE );
std::string current_db_name;
std::string table_name;

int main(int argc, char** argv) {
	std::string cmd;

	setup_intro();

	while(to_lower(cmd) != "exit") {
		cmd = "";

<<<<<<< Updated upstream
=======
		// Setup the command to wait for input
>>>>>>> Stashed changes
		color(10);

		if(current_db_name.length() > 0) {
			std::cout << current_db_name << "@";
		}
<<<<<<< Updated upstream

=======
>>>>>>> Stashed changes
		std::cout << "SQL>";
		color(7);
		std::getline(std::cin, cmd);

<<<<<<< Updated upstream
		// Do something with cmd
		if(tolower(cmd.find("open database ") == 0)){
			current_db_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);

=======
		// SELECT [ID, TEST, ] FROM TABLE;

		// Do something with cmd
		if(tolower(cmd.find("open database ") == 0)){
			current_db_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			Database *db = new Database(current_db_name);

			if(db->database_name != current_db_name){
				current_db_name = "";
			}

		} else if (to_lower(cmd) == "help") {
			show_help();
		} else if (to_lower(cmd) == "list database") {
			Database::List();
>>>>>>> Stashed changes
		} else if(tolower(cmd.find("create table ") == 0)){
		    table_name = cmd.substr(cmd.find_last_of(' ' ) + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
		    //Table *tbl = new create_table(current_db_name, table_name, );

		} else if (to_lower(cmd) == "exit"){
			std::cout << "Good Bye" << std::endl;
		} else {
			std::cout << "Invalid Command." << std::endl;
		}

<<<<<<< Updated upstream
		example_write();

=======
>>>>>>> Stashed changes
	}

	//Database *db = new Database("test");

	//db->Save();

	return 0;
}

<<<<<<< Updated upstream
=======
/// Converts a string to lower
>>>>>>> Stashed changes
std::string to_lower(std::string s){
	std::for_each(s.begin(), s.end(), [](char & c){
    	c = ::tolower(c);
	});
<<<<<<< Updated upstream

	return s;
=======

	return s;
}

/// Shows the help menu
void show_help() {
	std::cout << "Available Commands:" << std::endl;
	std::cout << "OPEN DATABASE 		- Check if the database exists and open it." << std::endl;
	std::cout << "CREATE DATABASE 	- Creates and new database and opens a connection to it." << std::endl;
	std::cout << "DROP DATABASE 		- Deletes the given database." << std::endl;
	std::cout << "CREATE TABLE 		- Creates a table in the current database." << std::endl;
	std::cout << "DROP DATABASE 		- Check if the database exists and open it." << std::endl;
	std::cout << "SELECT [] FROM [] 	- Selects the specified columns from the table." << std::endl;
	std::cout << "UPDATE TABLE 		- Updates the columns and meta for the given table." << std::endl;
	std::cout << "DELETE FROM 		- Deletes the sepcified data from the table." << std::endl;
	std::cout << "INSERT INTO 		- Inserts the data into the table." << std::endl;
	std::cout << "LIST DATABASE 		- Lists the current database names." << std::endl;

>>>>>>> Stashed changes
}

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

<<<<<<< Updated upstream
	std::cout << std::endl << std::endl << "Success! Here is your shell. Type exit to quit." << std::endl << std::endl;
=======
	std::cout << std::endl << std::endl << "Success! Here is your shell." << std::endl << "Type [help] for a list of commands. Type [exit]to quit." << std::endl << std::endl;
>>>>>>> Stashed changes
}

void color( int s){
	SetConsoleTextAttribute( h, s );
}

<<<<<<< Updated upstream
void example_write()
{
	// (create database) (test);
	Database *db = new Database("test");

	//std::string table_name = "test_Table";
	Table *tbl = create_table(db, table_name);
	std::cout << "Table Created! " << std::endl;

	// open test
	// File writing process
	std::ofstream wf("data/" + db->database_name + ".bin", std::ios::out | std::ios::binary);

	if(!wf){
		std::cout << "Cannot open file!" << std::endl;
	}

	wf.write((char *) &db, sizeof(Database));

	if(!wf.good()) {
      std::cout << "Error occurred at writing time!" << std::endl;
   	}

	// Clean up
	delete db;
	delete tbl;
}

/// FYI, This is still not functioning yet. Still in progress.
void example_read(std::string db_name) {
	std::ifstream rf("data/" + db_name + ".bin", std::ios::out | std::ios::binary);
   if(!rf) {
      std::cout << "Cannot open file!" << std::endl;
   }

   Database *db;

   rf.read((char *) &db, sizeof(Database));

   rf.close();

   if(!rf.good()) {
      std::cout << "Error occurred at reading time!" << std::endl;
   } else {
   		std::cout << "Successfully Opened: " << db->database_name << std::endl;
   }
}

Table* create_table(Database *db, std::string table_name){
=======
Table* create_table(Database *db, std::string table_name, std::map<std::string, std::string> columns){
>>>>>>> Stashed changes
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
>>>>>>> Stashed changes
