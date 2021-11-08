#include <fstream>
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
    ifstream infile { "../data/test.sql" };
    string file_contents { istreambuf_iterator<char>(infile), istreambuf_iterator<char>() };
    file_contents.erase(std::remove(file_contents.begin(), file_contents.end(), '\n'), file_contents.end());
    
    vector<string> commands = split_text(file_contents,";");

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
		    table_name = statement.substr(statement.find_last_of(' ' ) + 1, statement.find_last_of(';') - statement.find_last_of(' ') - 1);
		    Table *tbl = new Table(table_name);
		    db->AddTable(*tbl);
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

                cout << it << "\n";

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
                for( int i = 0; i <columns.size(); i++){
                    vector<string> row;
                    row.push_back(columns[i]);
                    row.push_back(values[i]);
                    current_table.Insert(row);
                }
            }
        }
    }
}

int main()
{
    read_sql_file_v2();
}
