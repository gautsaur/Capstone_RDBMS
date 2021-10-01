#include <fstream>
#include "../include/database.h"
using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void example_write();
void example_read(std::string db_name);

int main(int argc, char **argv)
{

    std::cout << "ISU RDBMS Project" << std::endl;

    example_write();
    example_read("test");

    return 0;
}

void example_write()
{
    // (create database) (test);
    Database *db = new Database("test");

    // (create table) (test_table) (id int, name string)
    Table *tbl = new Table("test_table");

    // Set the columns
    tbl->columns.insert({"int", "id"});
    tbl->columns.insert({"string", "name"});

    // insert into test_table (id, name) values((1, "test 1"), (2, "test 2"))
    // Insert the rows
    std::vector<std::string> v1 = {1, "test 1"};
    std::vector<std::string> v2 = {2, "test 2"};
    tbl->Insert(v1);
    tbl->Insert(v2);

    // Add the table to the database
    db->AddTable(*tbl);

    // open test
    // File writing process
    std::ofstream wf("data/" + db->database_name + ".bin", std::ios::out | std::ios::binary);

    if (!wf)
    {
        std::cout << "Cannot open file!" << std::endl;
    }

    wf.write((char *)&db, sizeof(Database));

    if (!wf.good())
    {
        std::cout << "Error occurred at writing time!" << std::endl;
    }

    // Clean up
    delete db;
    delete tbl;
}

/// FYI, This is still not functioning yet. Still in progress.
void example_read(std::string db_name)
{
    std::ifstream rf("data/" + db_name + ".bin", std::ios::out | std::ios::binary);
    if (!rf)
    {
        std::cout << "Cannot open file!" << std::endl;
    }

    Database *db;

    rf.read((char *)&db, sizeof(Database));

    rf.close();

    if (!rf.good())
    {
        std::cout << "Error occurred at reading time!" << std::endl;
    }
    else
    {
        std::cout << "Successfully Opened: " << db->database_name << std::endl;
    }
}

void read_sql_file()
{
    fstream newfile;
    newfile.open("../data/text.sql", ios::out); // open a file to perform write operation using file object
    if (newfile.is_open())                //checking whether the file is open
    {
        newfile << "Tutorials point \n"; //inserting text
        newfile.close();                 //close the file object
    }

    newfile.open("tpoint.txt", ios::in); //open a file to perform read operation using file object
    if (newfile.is_open())
    { //checking whether the file is open
        string tp;
        while (getline(newfile, tp))
        {                       //read data from file object and put it into string.
            cout << tp << "\n"; //print the data of the string
        }
        newfile.close(); //close the file object.
    }
}