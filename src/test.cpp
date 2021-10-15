#include <fstream>
#include <regex>
#include "../include/database.h"
using namespace std;

void check_command(string enteredCommand, string commandToCheck)
{
    if (enteredCommand.find(commandToCheck) != string::npos)
    {
        std::cout << "found!" << '\n';
    }
}

void split_text(string input)
{
    regex re("[\\|,: ]");
    sregex_token_iterator first{input.begin(), input.end(), re, -1}, last; //the '-1' is what makes the regex split (-1 := what was not matched)
    vector<std::string> tokens{first, last};
    for (auto t : tokens)
    {
        std::cout << t << std::endl;
    }
}

void read_sql_file()
{
    fstream newfile;
    newfile.open("../data/test.sql", ios::in); //open a file to perform read operation using file object
    if (newfile.is_open())
    { //checking whether the file is open
        string tp;
        while (getline(newfile, tp))
        {
            split_text(tp); //print the data of the string
        }
        newfile.close(); //close the file object.
    }
}

int main()
{
    read_sql_file();
}