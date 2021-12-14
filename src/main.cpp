#include "../include/database.h"
#include <vector>
#include <windows.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <regex>
#include <iostream>
#include <cerrno>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void color(int s);
void setup_intro();
void show_help();
void print_rows(Table tbl);
std::string to_lower(std::string str);
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
std::string current_db_name;
Table *create_table(std::string table_name, std::vector<pair<std::string, std::string>> columns_info);
std::string table_name;
std::string db_name;
void insert_into(Database *db,vector<string> split_commands);
void drop_table(Database *db, Table *tbl);
void drop_database(string db_name);
bool has_special_char(std::string const &str);
void table_info(Table tbl);
Database *read_sql_file(string path);
void update_table(Database *db, std::string table_name, std::string col1, std::string toUpdate, std::string col2, std::string forVariable);

int main(int argc, char **argv)
{
	std::string cmd;

	setup_intro();

	Database *db;

	while (Parser::to_lower(cmd) != "exit")
	{
		cmd = "";

		// Setup the command to wait for input
		color(10);
		if (current_db_name.length() > 0)
		{
			std::cout << current_db_name << "@";
		}

		std::cout << "SQL>";
		color(7);
		std::getline(std::cin, cmd);
		std::string statement = Parser::to_lower(cmd);

		// Do something with cmd
		if (statement == "exit")
		{
			std::cout << "Good Bye" << std::endl;
		}
		else if (statement == "help")
		{
			show_help();

		} else if(statement.back() != ';') {
			std::cout << "SQL command not properly terminated." << std::endl;
		} else if(statement.find("open database ") == 0){
			//current_db_name = statement.substr(statement.find_last_of(' ') + 1, statement.find_last_of(';') - statement.find_last_of(' ') - 1);
			current_db_name = Utils::trim(Utils::get_string_between_two_strings(cmd, "database ", ";"));
		 	db = new Database(current_db_name);

			if(db->database_name != current_db_name){
				current_db_name = "";
			}

		} else if (statement.find("create database") == 0){
		    current_db_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			db = new Database();
			db->database_name = current_db_name;
			db->Save();

		} else if (statement == "list databases;") {
			Database::List();
		} else if (statement.find("load sqlfile ") == 0) {
			string target_file_path = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			db = read_sql_file(target_file_path);
			
			current_db_name = db->database_name;
			
			cout << "Database Created: " << current_db_name << endl;
		} else if (statement.find("drop database ") == 0) {
			string db_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			//db = new Database(current_db_name);
			drop_database(db_name);
		} else if (current_db_name.length() == 0) {
			std::cout << "Open a database first." << std::endl;
		} else if (statement == "list tables;")	{			
			db->List_Tables();

		} else if(statement == "db info;") {
			db->List_Info();
		} else if (statement.find("select ") == 0) {
			// Parses the select command
			try	{

				std::string tbl_name = cmd.substr(statement.find(" from") + 6);

				tbl_name = Utils::remove_char(tbl_name, ';');

				Table tbl = db->get_table(tbl_name);

				if (tbl.table_name.length() > 0)
				{
					std::vector<std::string> cols = Parser::get_select_columns(cmd);
					std::string conditional = Parser::get_conditional(cmd);
					std::vector<std::string> where_clause = Parser::get_where_clause(cmd, conditional);
					tbl.Print_Rows(cols, where_clause, conditional);
				}
				else
				{
					std::cout << "Table does not exist." << std::endl;
				}
			}
			catch (const std::exception &)
			{
			}
		} else if (statement.find("create table ") == 0) {
			//table_name = Utils::trim(Utils::get_string_between_two_strings(cmd, "table", "("));
			table_name = Parser::get_table_name(cmd, "table", "(");
			vector<string> cols = Parser::get_create_columns(cmd);
			Table *tbl = new Table(table_name, cols);
			db->AddTable(*tbl);
		} else if (statement.find("insert into") == 0) {
			//table_name = Utils::trim(cmd.substr(cmd.find("into ") + 5, cmd.find_first_of('(') - cmd.find("into ") + 5));
			//table_name = Utils::get_string_between_two_strings(cmd, "into ", "(");
			table_name = Parser::get_table_name(statement, "into", "(");
			db->insert_into(cmd, table_name);
			db->Save();
		} else if (statement.find("table info ") == 0) {

			table_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			Table tbl = db->get_table(table_name);
			table_info(tbl);
		} else if (statement.find("drop table ") == 0) {
			string table_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			db->DropTable(table_name);
			db->Save();
		} else if (statement.find("update ") == 0) {
            //UPDATE [table_name] set Name = "new name" WHERE ID = 1;
            //update_table(db, table_name, col1Name, newValue, col2Name, forValue);
            std::string tbl_name = table_name = Utils::split(statement, " \n")[1];
            vector<vector<string> > columns = Parser::get_update_clause(statement);

            std::cout << columns.size() << std::endl;
            for (int i = 0; i < columns.size(); i++)
            {
                for (int j = 0; j < columns[i].size(); j++)
                {
                     std::cout << columns[i][j] << " ";
            }
            std::cout << std::endl;
            }

        } else if (statement.find("delete from ") == 0) {
			string tbl_name = Utils::get_string_between_two_strings(cmd, "from ", " where");
			
			int count = 0;
			string conditional = Parser::get_conditional(statement);
			vector<string> clause = Parser::get_where_clause(statement, conditional);
			string value = clause[1];
			Table currentTable = db->get_table(tbl_name);
			int col_ndx = currentTable.get_column_index(clause[0]);
			vector<vector<string> > rows = currentTable.rows;
			for (vector<string> row : rows) {
				cout << row[col_ndx] << conditional << value << endl;
				
				if(conditional == "="){					
					if (row[col_ndx] == value)
					{
						currentTable.DeleteRow(row);
						count += 1;
					}
				}else if(conditional ==">="){
					if (row[col_ndx] >= value)
					{
						currentTable.DeleteRow(row);
						count += 1;
					}
				}
				else if(conditional =="<="){
					if (row[col_ndx] <= value)
					{
						currentTable.DeleteRow(row);
						count += 1;
					}
				}
				else if(conditional ==">"){
					if (row[col_ndx] > value)
					{
						currentTable.DeleteRow(row);
						count += 1;
					}
				}
				else if(conditional =="<"){
					if (row[col_ndx] < value)
					{
						currentTable.DeleteRow(row);
						count += 1;
					}
				}else if(conditional =="!="){
					if (row[col_ndx] != value)
					{
						currentTable.DeleteRow(row);
						count += 1;
					}
				}else{
					std::cout << "Given conditional statement is not supported!" << std::endl;
				}
			}
			
			db->SaveTable(currentTable);
			
			db->Save();
			
			std::cout << count << " rows deleted." << endl;
		}
		else
		{
			std::cout << "Invalid Command." << std::endl;
		}
	}

	return 0;
}

/// Shows the help menu
void show_help()
{
	std::cout << "Available Commands:" << std::endl;
	std::cout << "OPEN DATABASE [name] 	- Check if the database exists and open it." << std::endl;
	std::cout << "CREATE DATABASE 	- Creates and new database and opens a connection to it." << std::endl;
	std::cout << "DB INFO 		- Lists the current database names." << std::endl;
	std::cout << "DROP DATABASE 		- Deletes the given database." << std::endl;
	std::cout << "CREATE TABLE 		- Creates a table in the current database." << std::endl;
	std::cout << "DROP TABLE [name] 	- Creates a table in the current database." << std::endl;
	std::cout << "DROP DATABASE 		- Check if the database exists and open it." << std::endl;
	std::cout << "SELECT [] FROM [] 	- Selects the specified columns from the table." << std::endl;
	std::cout << "UPDATE TABLE 		- Updates the columns and meta for the given table." << std::endl;
	std::cout << "DELETE FROM 		- Deletes the sepcified data from the table." << std::endl;
	std::cout << "INSERT INTO 		- Inserts the data into the table. (In Testing))" << std::endl;
	std::cout << "LIST DATABASES 		- Lists the current database names." << std::endl;
	std::cout << "LIST TABLES 		- Lists the current database names." << std::endl;
	std::cout << "TABLE INFO [name] 	- Lists the current database names." << std::endl;
	
	// List Tables
	// Display Schema
}

/// Setups the intro, emulating a startup
void setup_intro()
{
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

	std::cout << std::endl
			  << std::endl
			  << "Success! Here is your shell." << std::endl
			  << "Type [help] for a list of commands. Type [exit] to quit." << std::endl
			  << std::endl;
}

/// Sets the color of the output window
void color(int s)
{
	SetConsoleTextAttribute(h, s);
}


///Author: Janita Aamir
///This function updates an existing value with a new one given the column names and specific row.
//UPDATE [table_name] set Name = "new name" WHERE ID = 1;
void update_table(Database *db, std::string table_name, std::string col1, std::string toUpdate, std::string col2, std::string forVariable){
    Table tbl = db->get_table(table_name);

    int col1Index = tbl.get_column_index(col1);
    int col2Index = tbl.get_column_index(col2);

    std::vector<std::string>::const_iterator col;
    std::vector<std::vector<std::string> > rows = tbl.rows;


    for(std::vector<std::string> row : rows) {
        if (row[col2Index] ==  forVariable){
                row[col1Index] = toUpdate;
        }

    }


}

/// Author: Janita Aamir
/// Creates a table with given column info
Table *create_table(std::string table_name, std::vector<pair<std::string, std::string>> columns_info)
{
	Table *tbl = new Table(table_name);
	for (int i = 0; i < columns_info.size(); i++)
	{
		tbl->columns.insert({columns_info[i].first, columns_info[i].second});
	}
	return tbl;
}

///Author: Janita Aamir
///This function drops the given table from the current database.
void drop_table(Database *db, Table* tbl){
    tbl->Delete();

    for (std::vector<Table>::iterator it = db->tables.begin(); it != db->tables.end(); ++it)
    {
        if (it->table_name == tbl->table_name)
        {
            db->tables.erase(it);
            break;
        }
    }
    db->Save();
}

///Author: Janita Aamir
///This function drops the given database from the dbms.
void drop_database(string db_name)
{

	std::string s = "data/" + db_name + ".db";

	if (std::remove(s.c_str()) != 0)
	{

		perror("Error deleting file");
	}
	else
	{
		puts("File successfully deleted");
		current_db_name = "";
	}
}

/// Author: Janita Aamir, Saurav Gautam
/// Shows the given table information
/// Table Name, Column Names, Number of Rows
void table_info(Table tbl)
{
	std::cout << "Table name: " << tbl.table_name << std::endl;
	std::cout << "----------------------------- " << std::endl;
	std::cout << "Column names: " << std::endl;
	std::vector<std::string> it = tbl.get_column_names();

	for (int i = 0; i < it.size(); i++)
	{
		cout << "\t"
			 << "-" << it.at(i) << '\n';
	}

	std::cout << "----------------------------- " << std::endl;
	std::cout << "Number of Rows: " << tbl.rows.size() << std::endl;
}

///Janita Aamir
///This function is used within create table. It checks to see if the
///database selected has any special characters that aren't allowed.
bool has_special_char(std::string const &s)
{
	for (int i = 0; i < s.length(); i++)
	{
		if (!std::isalpha(s[i]) && !std::isdigit(s[i]) && s[i] != '_')
			return false;
	}
}

/// Author: Saurav Gautam
/// Loads the sql file
/// Parses the data
/// Runs the queries
/// Saves the database
Database *read_sql_file(string path)
{
	ifstream infile("data/" + path, std::ios::in | std::ios::binary);
	string file_contents{istreambuf_iterator<char>(infile), istreambuf_iterator<char>()};
	file_contents.erase(std::remove(file_contents.begin(), file_contents.end(), '\n'), file_contents.end());

	vector<string> commands = split_text(file_contents, ";");
	string table_name;
	string current_db_name;
	Database *db = new Database();

	for (string statement : commands)
	{
		string statement_lowercase = Parser::to_lower(Utils::trim(statement));

		if (statement_lowercase.find("create database") == 0)
		{
			current_db_name = statement.substr(statement.find_last_of(' ') + 1, statement.find_last_of(';') - statement.find_last_of(' ') - 1);
			db->database_name = current_db_name;
		}
		else if (statement_lowercase.find("create table ") == 0)
		{
			table_name = Parser::get_table_name(statement, "table", "(");
			cout << "Table Created: " << table_name << endl;
			vector<string> cols = Parser::get_create_columns(statement);
			Table *tbl = new Table(table_name, cols);
			db->AddTable(*tbl);
		}
		else if (statement_lowercase.find("insert into") == 0)
		{
			table_name = Parser::get_table_name(statement, "into", "(");
			db->insert_into(statement, table_name);
			db->Save();
		}
	}
	db->Save();
	return db;
}
