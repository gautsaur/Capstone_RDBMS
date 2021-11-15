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
void drop_database(Database *db);
bool has_special_char(std::string const &str);
void table_info(Table tbl);
void read_sql_file(string path);

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
		
		} else if (statement == "list database;") {
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
				
				std::string tbl_name = statement.substr(statement.find(" from") + 6);
				
				tbl_name = remove_char(tbl_name, ';');
				
				Table tbl = db->get_table(tbl_name);

				if(tbl.table_name.length() > 0){
					std::vector<std::string> cols = Parser::get_select_columns(statement);
					tbl.Print_Rows(cols);

				} else {
					std::cout << "Table does not exist." << std::endl;
				}

			} catch(const std::exception&) {

			}

		}  else if(statement.find("create table ") == 0){
		    vector<string> result;
		    stringstream ss (statement);
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

		    vector<string> columns = Parser::get_create_columns(statement);

		    Table *tbl =  create_table(db, table_name, columns);

		} else if (statement.find("insert into") == 0) {
            table_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
            vector<string> yo = split_text(statement, " (),'");
            insert_into(db,yo);
        } else if(statement.find("table info ") == 0) {
		    // table_name = statement.substr(statement.find_last_of(' ') + 1, statement.find_last_of(';') - statement.find_last_of(' ') - 1);
		    // Table *tbl =  new Table(table_name);
		    // table_info(tbl);

            table_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
		    Table tbl =  db->get_table(table_name);
		    table_info(tbl);

		} else if(statement.find("drop database ") == 0) {
		    current_db_name = statement.substr(statement.find_last_of(' ') + 1, statement.find_last_of(';') - statement.find_last_of(' ') - 1);
		 	db = new Database(current_db_name);
		 	drop_database(db);
		} else if (statement.find("load sqlfile ") == 0) {
            string target_file_path = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
            read_sql_file(target_file_path);
        } else {
			std::cout << "Invalid Command." << std::endl;
		}
	}

	return 0;
}

/// Converts a string to lower
std::string to_lower(std::string s)
{
    std::for_each(s.begin(), s.end(), [](char &c)
                  { c = ::tolower(c); });

    return s;
}

void insert_into(Database *db, vector<string> split_commands)
{
    vector<string> columns;
    bool columnSet = false;
    vector<string> values;
    int count = 0;
    for (auto &it : split_commands)
    {

        string it_lowercase = it;
        std::for_each(it_lowercase.begin(), it_lowercase.end(), [](char &c)
                      { c = ::tolower(c); });

        if (count == 2)
        {
            table_name = it;
        }
        if (it_lowercase == "values")
        {
            columnSet = true;
            continue;
        }
        if (count > 2)
        {
            if (!columnSet)
            {
                columns.push_back(it);
            }
            else
            {
                values.push_back(it);
            }
        }
        count = count + 1;
    }

    if (columns.size() != values.size())
    {
        cout << "INSERT INTO function is formatted wrong!";
    }
    else
    {
        Table current_table = db->get_table(table_name);
        vector<string> row_entry;
        vector<string> col_names = current_table.get_column_names();
        reverse(col_names.begin(), col_names.end());

        for (int i = 0; i < col_names.size(); i++)
        {
            cout << col_names.at(i) << ' ';
        }
        int index = -1;
        for (int i = 0; i < columns.size(); i++)
        {

            cout << "Looking column:" << columns[i] << "\n";

            auto it = std::find(col_names.begin(), col_names.end(), columns[i]);
            if (it == col_names.end())
            {
                // name not in vector
            }
            else
            {
                index = distance(col_names.begin(), it);
            }

            cout << "Index is :" << index;

            if (!index)
            {
                row_entry.push_back("NULL");
            }
            else
            {
                row_entry.push_back(values[index]);
            }
        }
    }
}

std::string remove_char(std::string str, char delim)
{
    str.erase(std::remove(str.begin(), str.end(), delim), str.end());

    return str;
}

void print_rows(Table tbl)
{
    int row_count = 0;
    int col_char_count = tbl.GetLargestColumnSize();
    int row_char_count = (tbl.columns.size() * (col_char_count + 3)) + 1;

    std::cout << "| ";

    for (auto const &it : tbl.columns)
    {
        std::cout << it.first;

        for (int i = 0; i < col_char_count - it.first.length(); i += 1)
        {
            std::cout << " ";
        }

        std::cout << " | ";
    }

    std::cout << std::endl;

    for (int i = 0; i < row_char_count; i += 1)
    {
        std::cout << "=";
    }

    std::cout << std::endl;

    for (std::vector<std::string> row : tbl.rows)
    {
        std::cout << "| ";

        for (std::string &value : row)
        {
            std::cout << value;

            for (int i = 0; i < col_char_count - value.length(); i += 1)
            {
                std::cout << " ";
            }

            std::cout << " | ";
        }

        std::cout << std::endl;

        for (int i = 0; i < row_char_count; i += 1)
        {
            std::cout << "-";
        }

        std::cout << std::endl;

        row_count += 1;
    }

    std::cout << row_count << " rows selected." << std::endl;
}

/// Shows the help menu
void show_help()
{
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


Table* create_table(Database *db, std::string table_name, std::vector<std::string> columns){
    // (create table) (test_table) (id int, name string)

    if (!current_db_name.empty() && has_special_char(current_db_name)!= true){
            if (db->find_table(table_name) != true ){

            Table *tbl = new Table(table_name);

            for (string i: columns){
                stringstream ss(i);
                string s;
                int n = 1;
                std::string key;
                std::string value;
                while (ss >> s) {
                        if (n ==1){
                            key = s;
                        }
                        if (n == 2){
                           value = s;
                        }
                        tbl->columns.insert({key, value});
                        n++;
                }
            }

			cout << "Creating Table: " << table_name << endl;

            // Add the table to the database
            db->AddTable(*tbl);

			db->Save();

            return tbl;
        }
    }
}

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

Table insert_into(std::string table_name, std::map<std::string, std::string> columns){
	// Seperate values from commas
	// Create a std::vector<std::string> using values
	// Database->table->insert(rows);

//    std::vector<std::string> v1
//	columns.Insert(v1);
//	return columns;
}

void drop_database(Database *db){
    //delete the database file in the database.h


    std::string s1 = "data/";
    std::string s2 = db->database_name;
    std::string s3 = ".db";

    std::string s = s1 + s2 + s3;

    std::cout << s << endl;

    if (std::remove(s.c_str()) != 0)
        perror("Error deleting file");
    else
        puts("File successfully deleted");

    db->Delete();
    db->List();
}

void table_info(Table tbl){
    std::cout << "Table name: " + tbl.table_name << std::endl;
    std::cout << "Column names: " << std::endl;
    std::vector<std::string> it = tbl.get_column_names();

    std::cout << "Number of Rows: " + tbl.rows.size() << std::endl;

}

bool has_special_char(std::string const &s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (!std::isalpha(s[i]) && !std::isdigit(s[i]) && s[i] != '_')
            return false;
    }
}

void read_sql_file(string path)
{
    
    std::ifstream in("data/testFile.sql", std::ios::in | std::ios::binary);
    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    //cout << content; //you can do anything with the string!!!
    
    vector<string> commands = split_text(content, ";");
    //cout << commands.size();
    Database *db;
    string table_name;
    string db_name;

    for (auto &orig_statement : commands)
    {
    	string statement = to_lower(orig_statement);
        
        cout << statement << endl;
        
        cout << statement.find("create table") << endl;

        if (statement.rfind("create database", 0) == 0)
        {
            db_name = statement.substr(statement.find_last_of(' ') + 1, statement.find_last_of(';') - statement.find_last_of(' ') - 1);
            db = new Database();
            db->database_name = db_name;
        }
        else if (statement.find("create table") == 4)
        {
            //cout << statement << "\n";
            vector<string> yo = split_text(statement, " (),'");
            vector<string> read_columns;
            string column;
            string key = "";
            string value = "";
            int count = 0;
            for (auto &it : yo)
            {
                //cout << it << "\n";
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
                    	column = it + " ";
                        //key = it;
                    }
                    if (count % 3 == 1)
                    {
                    	column += it_lowercase;
//                        if (it_lowercase == "int")
//                        {
//                            value = "int";
//                        }
//                        else
//                        {
//                            value = "string";
//                        }
                    }
                    if (count % 3 == 2)
                    {
                        read_columns.push_back(column);
                        column = "";
                        key = "";
                        value = "";
                    }
                }
                count = count + 1;
            }

			cout << db_name << endl;

			current_db_name = db_name;

            create_table(db, table_name, read_columns);
        }
        else if (statement.find("insert into") == 0)
        {
            vector<string> yo = split_text(statement, " (),'");
            insert_into(db,yo);
        }
    }
    
    db->Save();
    
}
