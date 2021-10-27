#include <fstream>
#include <regex>
#include <bits/stdc++.h>
#include "../include/database.h"
#include "../include/utils.h"
using namespace std;

class Parser {
	public:
		void check_command(string enteredCommand, string commandToCheck);	
		void tokenize(std::string const &str, const char* delim, std::vector<std::string> &out);
		string splitString(string str, string delimiter);
		vector<string> split_text(string input);
		void read_sql_file();
		
};

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


vector<string> split_text(string input)
{
    // Declare delimiters
    string delimeters = " ,.-':;?()+*/%$#!\"@^&";

    // Get string split results
    auto results = Utils::split(input, delimeters);
    return results;
}

void read_sql_file()
{
    fstream newfile;
    newfile.open("../data/test.sql", ios::in); //open a file to perform read operation using file object
    if (newfile.is_open())
    { //checking whether the file is open
        string tp;

        // attach current database if not there
        Database *db;
        Table *tbl;
        while (getline(newfile, tp))
        {
            cout<<"\nCurrent Line"<<tp;
            if(tp.find("CREATE DATABASE")!=string::npos){
                string databaseName = splitString(tp, "CREATE DATABASE");
                db = new Database(databaseName);
            }

            if(tp.find("CREATE TABLE")!= string::npos){
               string tableName = splitString(tp, "CREATE TABLE");
               tbl = new Table(tableName);

               db->AddTable(*tbl);
                while (getline(newfile, tp) && tp != ");")
                {
                    vector<string> temp = split_text(tp);
                    if (temp.size() == 0){
                        continue;
                    }
                    cout << "\nColumn Type: "<<temp[1] << "|| ColumnName: " <<temp[0];
                    string colType;

                    if (temp[1]=="varchar" || temp[1] == "char" || temp[1]=="string"){
                        colType = "string";
                    }
                    else{
                        colType ="int";
                    }

                    tbl->columns.insert({temp[1], temp[0]});
                }   
            }

            if(tp.find("INSERT INTO")!=string::npos){
                string tableName = splitString(tp, "INSERT INTO");
                vector<string> insertArrangement = split_text(tableName);

                for(int i = 0; i<insertArrangement.size(); i++) {
                    cout << insertArrangement[i] << endl;
                }
                while (getline(newfile, tp))
                {
                    string tableName = splitString(tp, "VALUES");
                    vector<string> insertArrangement = split_text(tableName);
                    tbl->Insert(insertArrangement);
                }   
            }

            //print the data of the string
        }
        newfile.close(); //close the file object.
    }
}
