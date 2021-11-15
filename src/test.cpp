#include <fstream>
#include <algorithm>

#include <regex>
#include "../include/database.h"
#include "../include/utils.h"
using namespace std;

void check_command(string enteredCommand, string commandToCheck)
{
    if (enteredCommand.find(commandToCheck) != string::npos)
    {
        std::cout << "found!" << '\n';
    }
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

	// Add the table to the database
	db->AddTable(*tbl);

	return tbl;
}


void tokenize(std::string const &str, const char* delim,
            std::vector<std::string> &out)
{
    char *token = strtok(const_cast<char*>(str.c_str()), delim);
    while (token != nullptr)
    {
        out.push_back(std::string(token));
        token = strtok(nullptr, delim);
    }

}

string splitString(string str, string delimiter)
{
    int start = 0;
    int end = str.find(delimiter);
    while (end != -1) {
        cout << str.substr(start, end - start) << endl;
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }
    return str.substr(start, end - start);
}


vector<string> split_text(string input, string delimeter)
{
    // Declare delimiters
    //string delimeters = " ,.-':;?()+*/%$#!\"@^&";

    // Get string split results
    auto results = Utils::split(input, delimeter);
    return results;
}


void read_sql_file_v2(){
    ifstream infile { "testFile.sql" };
    string file_contents { istreambuf_iterator<char>(infile), istreambuf_iterator<char>() };
    file_contents.erase(std::remove(file_contents.begin(), file_contents.end(), '\n'), file_contents.end());

    cout<<file_contents;
    vector<string> commands = split_text(file_contents,";");
    cout<< commands.size();
    Database *db;
    string table_name;
    string current_db_name;

    for(auto& statement: commands){
        string statement_lowercase = statement;
        std::for_each(statement_lowercase.begin(), statement_lowercase.end(), [](char & c){
            c = ::tolower(c);
        });

        if (statement_lowercase.find("create database") == 0){
		    current_db_name = statement.substr(statement.find_last_of(' ') + 1, statement.find_last_of(';') - statement.find_last_of(' ') - 1);
            db = new Database(current_db_name);
        }else if(statement_lowercase.find("create table") == 0){
            cout << statement << "\n";
		    vector<string> yo = split_text(statement, " (),'");
            std::map<std::string, std::string> read_columns;
            string key = "";
            string value = "";
            int count = 0;
            for(auto& it: yo){
                cout << it << "\n";
                string it_lowercase = it;
                std::for_each(it_lowercase.begin(), it_lowercase.end(), [](char & c){
                    c = ::tolower(c);
                });

                if(count == 2){
                    table_name = it;
                }
                if (count >2 ){
                    if(count %3 == 0){
                        key = it;
                    }
                    if(count %3 ==1){
                        if(it_lowercase == "int"){
                            value = "int";
                        }else{
                            value = "string";
                        }
                    }
                    if(count%3 == 2){
                        read_columns.insert({key,value});
                        key ="";
                        value="";
                    }
                }
                count = count +1;
            }

            create_table(db,table_name,read_columns);
		}else if(statement_lowercase.find("insert into") == 0){
		    vector<string> yo = split_text(statement, " (),'");
            vector<string> columns;
            bool columnSet = false;
            vector<string> values;
            int count = 0;
            for(auto& it: yo){

                string it_lowercase = it;
                std::for_each(it_lowercase.begin(), it_lowercase.end(), [](char & c){
                    c = ::tolower(c);
                });

                if(count == 2){
                    table_name = it;
                }
                if (it_lowercase == "values"){
                    columnSet = true;
                    continue;
                }
                if (count >2 ){
                    if (!columnSet){
                        columns.push_back(it);
                    }else{
                        values.push_back(it);
                    }
                }
                count = count +1;
            }

            if (columns.size() != values.size()){
                cout << "INSERT INTO function is formatted wrong!";
                return;
            }else{
                Table current_table = db->get_table(table_name);
                vector<string> row_entry;
                vector<string> col_names = current_table.get_column_names();
                reverse(col_names.begin(),col_names.end());

                for(int i=0; i < col_names.size(); i++){
                    cout << col_names.at(i) << ' ';
                }
                int index = -1;
                for( int i = 0; i <columns.size(); i++){

                    cout << "Looking column:" << columns[i] << "\n";

                    auto it = std::find(col_names.begin(), col_names.end(), columns[i]);
                    if (it == col_names.end())
                    {
                      // name not in vector
                    } else
                    {
                      index = distance(col_names.begin(), it);
                    }

                    cout << "Index is :"<<index;

                    if (!index){
                        row_entry.push_back("NULL");
                    }else{
                        row_entry.push_back(values[index]);
                    }
                }
            }
        }
    }
}

int main()
{
    read_sql_file_v2();
}

