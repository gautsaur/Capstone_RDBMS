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

		// SELECT [ID, TEST, ] FROM TABLE;

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
			current_db_name = statement.substr(statement.find_last_of(' ') + 1, statement.find_last_of(';') - statement.find_last_of(' ') - 1);
		 	db = new Database(current_db_name);

			if(db->database_name != current_db_name){
				current_db_name = "";
			}

		} else if (statement.find("create database") == 0){
		    current_db_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			db = new Database();
			db->database_name = current_db_name;

		} else if (statement == "list databases;") {
			Database::List();
		}
		else if (statement == "list tables;")
		{
			if (current_db_name.length() == 0)
			{
				std::cout << "Open a database first." << std::endl;
			}
			else
			{
				db->List_Tables();
			}

		} else if (statement.find("select ") == 0) {
			// Parses the select command
			try	{

				std::string tbl_name = cmd.substr(statement.find(" from") + 6);

				tbl_name = Utils::remove_char(tbl_name, ';');

				Table tbl = db->get_table(tbl_name);

				cout << "Selecting from " << tbl_name << endl;

				if (tbl.table_name.length() > 0)
				{
					std::vector<std::string> cols = Parser::get_select_columns(cmd);
					//Parser::get_where_clause(cmd);
					tbl.Print_Rows(cols);
				}
				else
				{
					std::cout << "Table does not exist." << std::endl;
				}
			}
			catch (const std::exception &)
			{
			}
		}
		else if (statement.find("create table ") == 0)
		{
			table_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			Table *tbl = new Table(table_name);
			db->AddTable(tbl);
		}
		else if (statement.find("insert into") == 0)
		{
			db->insert_into(cmd);
			db->Save();
		}
		else if (statement.find("table info ") == 0)
		{

			table_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			Table tbl = db->get_table(table_name);
			table_info(tbl);
		}
		else if (statement.find("drop table ") == 0)
		{
			string table_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			db->DropTable(table_name);
			db->Save();
		}
		else if (statement.find("drop database ") == 0)
		{
			string db_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			//db = new Database(current_db_name);
			drop_database(db_name);
		}
		else if (statement.find("load sqlfile ") == 0)
		{
			string target_file_path = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			db = read_sql_file(target_file_path);
		}
		else if (statement.find("delete from ") == 0)
		{
			vector<string> splitTexts = split_text(statement, " (),'");

			if (splitTexts[0] != "delete" && splitTexts[1] != "from")
			{
				continue;
			}
			else
			{
				string conditional = splitTexts[4];
				string value = splitTexts[5];
				Table currentTable = db->get_table(splitTexts[2]);
				int col_ndx = currentTable.get_column_index(splitTexts[3]);
				int row_len = currentTable.rows.size();
				for (int i = 0; i < row_len; i++)
				{
					if(conditional == "="){					
						if (currentTable.rows[i][col_ndx] == value)
						{
							currentTable.rows.erase(currentTable.rows.begin() + col_ndx);
						}
						else
						{
							std::cout << "No such value for WHERE clause." << std::endl;
						}
					}else if(conditional ==">="){
						if (currentTable.rows[i][col_ndx] >= value)
						{
							currentTable.rows.erase(currentTable.rows.begin() + col_ndx);
						}
						else
						{
							std::cout << "No such value for WHERE clause." << std::endl;
						}
					}
					else if(conditional =="<="){
						if (currentTable.rows[i][col_ndx] <= value)
						{
							currentTable.rows.erase(currentTable.rows.begin() + col_ndx);
						}
						else
						{
							std::cout << "No such value for WHERE clause." << std::endl;
						}
					}
					else if(conditional ==">"){
						if (currentTable.rows[i][col_ndx] > value)
						{
							currentTable.rows.erase(currentTable.rows.begin() + col_ndx);
						}
						else
						{
							std::cout << "No such value for WHERE clause." << std::endl;
						}
					}
					else if(conditional =="<"){
						if (currentTable.rows[i][col_ndx] < value)
						{
							currentTable.rows.erase(currentTable.rows.begin() + col_ndx);
						}
						else
						{
							std::cout << "No such value for WHERE clause." << std::endl;
						}
					}else if(conditional =="!="){
						if (currentTable.rows[i][col_ndx] != value)
						{
							currentTable.rows.erase(currentTable.rows.begin() + col_ndx);
						}
						else
						{
							std::cout << "No such value for WHERE clause." << std::endl;
						}
					}else{
						std::cout << "Given conditional statement is not supported!" << std::endl;
					}
				}
				db->SaveTable(currentTable);
			}
			db->Save();
		}
		else
		{
			std::cout << "Invalid Command." << std::endl;
		}
	}

	return 0;
}

std::string remove_char(std::string str, char delim)
{
	str.erase(std::remove(str.begin(), str.end(), delim), str.end());

	return str;
}

/// Shows the help menu
void show_help()
{
	std::cout << "Available Commands:" << std::endl;
	std::cout << "OPEN DATABASE [name] 	- Check if the database exists and open it." << std::endl;
	std::cout << "CREATE DATABASE 	- Creates and new database and opens a connection to it." << std::endl;
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

//Janita Aamir
//Date: mm-dd-yy
//This function creates a new table within the current database and accepts a set of column names with types.
// Table* create_table(Database *db, std::string table_name, std::vector<std::string> columns){

//     if (!current_db_name.empty() && has_special_char(current_db_name)!= true){
//             if (db->find_table(table_name) != true ){

//             Table *tbl = new Table(table_name);

//             for (string i: columns){

// 				vector<string> pr = Utils::split(i, " ");

// 				if(pr.size() > 1){
// 					tbl->columns.insert({pr[0], pr[1]});
// 				}

//             }

//             return tbl;
//         }
//     }
// }

/// Author: Saurav Gautam
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

//Janita Aamir
//Date: mm-dd-yy
//This function drops the given table from the current database.
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

/// Author: ?????, Saurav Gautam
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

//Janita Aamir
//Date: mm-dd-yy
//This function is used within create table. It checks to see if the
//database selected has any special characters that aren't allowed.
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

	for (auto &statement : commands)
	{
		string statement_lowercase = statement;
		std::for_each(statement_lowercase.begin(), statement_lowercase.end(), [](char &c)
					  { c = ::tolower(c); });

		if (statement_lowercase.find("create database") == 0)
		{
			current_db_name = statement.substr(statement.find_last_of(' ') + 1, statement.find_last_of(';') - statement.find_last_of(' ') - 1);
			db->database_name = current_db_name;
			cout << "Created datbase: " << current_db_name << "\n";
		}
		else if (statement_lowercase.find("create table") == 0)
		{
			vector<string> splitTexts = split_text(statement, " (),'");
			vector<pair<string, string>> read_columns;
			string datatype_key = "";
			string columnName_value = "";
			int count = 0;
			for (auto &it : splitTexts)
			{
				string it_lowercase = it;
				std::for_each(it_lowercase.begin(), it_lowercase.end(), [](char &c)
							  { c = ::tolower(c); });

				if (count == 2)
				{
					table_name = it;
				}
				if (count > 2)
				{
					if (count % 3 == 0)
					{
						columnName_value = it;
					}
					if (count % 3 == 1)
					{
						if (it_lowercase == "int")
						{
							datatype_key = "int";
						}
						else
						{
							datatype_key = "string";
						}

						read_columns.push_back(make_pair(columnName_value, datatype_key));
						count = 2;
					}
				}
				count = count + 1;
			}

			Table *tbl = create_table(table_name, read_columns);
			db->AddTable(*tbl);
			cout << "Created table: " << table_name << "\n";
		}
		else if (statement_lowercase.find("insert into") == 0)
		{
			db->insert_into(statement);
		}
	}
	db->Save();
	return db;
}
