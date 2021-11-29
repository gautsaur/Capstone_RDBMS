#include "../include/database.h"
#include <vector>
#include <windows.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <streambuf>
#include <regex>
#include <cerrno>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void color(int s);
void setup_intro();
void show_help();
void print_rows(Table tbl);
std::string to_lower(std::string str);
std::string remove_char(std::string str, char delim);
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
std::string current_db_name;
Table* create_table(Database *db, std::string table_name, std::vector<std::string> columns);
std::string table_name;
Database *create_db(Database *db, std::string db_name);
std::string db_name;
void insert_into(Database *db,vector<string> split_commands);
void drop_table(Database *db, Table *tbl);
void drop_database(string db_name);
bool has_special_char(std::string const &str);
void table_info(Table tbl);
Database *read_sql_file(string path);
void update_table(Database *db, std::string table_name, std::string col1, std::string toUpdate, std::string col2, std::string forVariable);

int main(int argc, char** argv) {
	std::string cmd;

	setup_intro();

	Database *db;

	while(Parser::to_lower(cmd) != "exit") {
		cmd = "";

		// Setup the command to wait for input
		color(10);
		if(current_db_name.length() > 0) {
			std::cout << current_db_name << "@";
		}

		std::cout << "SQL>";
		color(7);
		std::getline(std::cin, cmd);
		std::string statement = Parser::to_lower(cmd);


		// SELECT [ID, TEST, ] FROM TABLE;

		// Do something with cmd
		if (statement == "exit"){
			std::cout << "Good Bye" << std::endl;
		} else if (statement == "help") {
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
		    current_db_name = statement.substr(statement.find_last_of(' ') + 1, statement.find_last_of(';') - statement.find_last_of(' ') - 1);
            db = new Database(current_db_name);

		} else if (statement == "list databases;") {
			Database::List();
		} else if (statement == "list tables;") {
			if(current_db_name.length() == 0) {
				std::cout << "Open a database first." << std::endl;
			} else {
				db->List_Tables();
			}

		} else if (statement.find("select ") == 0) {
			// Parses the select command
			try	{

				std::string tbl_name = cmd.substr(statement.find(" from") + 6);

				tbl_name = remove_char(tbl_name, ';');

				Table tbl = db->get_table(tbl_name);

				cout << "Selecting from " << tbl_name << endl;

				if(tbl.table_name.length() > 0){
					std::vector<std::string> cols = Parser::get_select_columns(cmd);
					//Parser::get_where_clause(cmd);
					tbl.Print_Rows(cols);

				} else {
					std::cout << "Table does not exist." << std::endl;
				}

			} catch(const std::exception&) {

			}

		}  else if(statement.find("create table ") == 0){
		    vector<string> result;
		    stringstream ss (cmd);
		    string item;
		    while (std::getline (ss, item, ' ' )) {
                    result.push_back (item);
            }
            int k = 0;
            for (auto i : result){
                if (k == 2){
                    table_name = i;
                }
                k++;
            }

		    vector<string> columns = Parser::get_create_columns(cmd);

		    Table *tbl =  create_table(db, table_name, columns);

		    db->AddTable(*tbl);
		    db->Save();

		} else if (statement.find("insert into") == 0) {
            table_name = Utils::split(statement, " \n")[2];
        	vector<vector<string> > rows = Parser::get_insert_rows(statement, table_name);
            vector<string> columns = Parser::get_insert_columns(statement, table_name);

            db->insert_into_table(table_name, columns, rows);
        } else if(statement.find("table info ") == 0) {

            table_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
		    Table tbl =  db->get_table(table_name);
		    table_info(tbl);

		} else if (statement.find("drop table ") == 0){
			string table_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			db->DropTable(table_name);
		} else if(statement.find("drop database ") == 0) {
		    string db_name = statement.substr(statement.find_last_of(' ') + 1, statement.find_last_of(';') - statement.find_last_of(' ') - 1);
		 	//db = new Database(current_db_name);
		 	drop_database(db_name);
		} else if (statement.find("load sqlfile ") == 0) {
            string target_file_path = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
            db = read_sql_file(target_file_path);
        } else if (statement.find("update ") == 0) {
            vector<string> result;
		    stringstream ss (cmd);
		    string item;
		    string col1Name;
		    string forValue;
		    string col2Name;
		    string newValue;
		    while (std::getline (ss, item, ' ' )) {
                    result.push_back (item);
            }
            int k = 0;
            for (auto i : result){
                if (k == 1){
                    table_name = i;
                }
                if (k == 3){
                    col1Name = i;
                }
                if (k == 5){
                    newValue = i;
                }
                if (k == 7){
                    col2Name = i;
                }
                if (k == 9){
                    forValue = i;
                }
                k++;
            }
            //std::cout << table_name << std::endl;
            //std::cout << col1Name << std::endl;
            //std::cout << forValue << std::endl;
            //std::cout << col2Name << std::endl;
            //std::cout << newValue << std::endl;
            update_table(db, table_name, col1Name, newValue, col2Name, forValue);

        }else {
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
//mm-dd-yy
//This function updates an existing value with a new one given the column names and specific row.
//UPDATE [table_name] set Name = "new name" WHERE ID = 1;
void update_table(Database *db, std::string table_name, std::string col1, std::string toUpdate, std::string col2, std::string forVariable){
    cout << "here" << endl;
    cout << table_name << endl;
    Table tbl = db->get_table(table_name);
    cout << "found Table" << endl;

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

//Janita Aamir
//Date: mm-dd-yy
//This function creates a new table within the current database and accepts a set of column names with types.
Table* create_table(Database *db, std::string table_name, std::vector<std::string> columns){

    if (!current_db_name.empty() && has_special_char(current_db_name)!= true){
            if (db->find_table(table_name) != true ){

            Table *tbl = new Table(table_name);

            for (string i: columns){

				vector<string> pr = Utils::split(i, " ");

				if(pr.size() > 1){
					tbl->columns.insert({pr[0], pr[1]});
				}

            }

            return tbl;
        }
    }
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

Database *create_db(Database *db, std::string db_name)
{
    Database *cr = new Database(db_name);
    db->Save();
    return cr;
}

//Janita Aamir
//Date: mm-dd-yy
//This function removes the given database from the data folder.
void drop_database(string db_name){

    std::string s = "data/" + db_name + ".db";

    if (std::remove(s.c_str()) != 0){

        perror("Error deleting file");

	} else {
		puts("File successfully deleted");
		current_db_name = "";

	}

}

//Janita Aamir
//Date: mm-dd-yy
//This function prints out the table information (table name, column names, number of rows etc).
void table_info(Table tbl){
    std::cout << "Table name: " + tbl.table_name << std::endl;
    std::cout << "Column names: " << std::endl;
    std::vector<std::string> it = tbl.get_column_names();

    std::cout << "Number of Rows: " + tbl.rows.size() << std::endl;

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

Database *read_sql_file(string path)
{

    std::ifstream in("data/testFile.sql", std::ios::in | std::ios::binary);
    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    //cout << content; //you can do anything with the string!!!

    vector<string> commands = split_text(content, ";");
    //cout << commands.size();
    Database *db;
    string table_name;
    string db_name;

    for (auto &statement : commands)
    {
    	statement = Utils::trim(statement);

        if (Parser::to_lower(statement).rfind("create database", 0) == 0)
        {
            db_name = statement.substr(statement.find_last_of(' ') + 1, statement.find_last_of(';') - statement.find_last_of(' ') - 1);
            db = new Database();
            db->database_name = db_name;
        }
        else if (Parser::to_lower(statement).find("create table") == 0)
        {
        	table_name = Utils::split(statement, " \n")[2];
        	table_name.erase(remove_if(table_name.begin(), table_name.end(), ::isspace), table_name.end());
        	vector<string> columns = Parser::get_create_columns(statement);

			current_db_name = db_name;

            Table *tbl = create_table(db, table_name, columns);

            db->AddTable(*tbl);
        }
        else if (Parser::to_lower(statement).find("insert into") == 0)
        {
			table_name = Utils::split(statement, " \n")[2];
        	vector<vector<string> > rows = Parser::get_insert_rows(statement, table_name);
            vector<string> columns = Parser::get_insert_columns(statement, table_name);

            db->insert_into_table(table_name, columns, rows);
        }
    }

    db->Save();

    return db;

}
