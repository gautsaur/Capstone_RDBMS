/* 
*	File: 	    table.h
*   Author:     Andrew Nunez
*   Date:       Sept. 23, 2021
*
*   This file holds a class that defines a table and it's data to read and write from a .bin file.
*/

#include <iostream>
#include <map>
#include <string>
#include <vector>

class Table {
	public:		
		/// The name of the table
		std::string table_name;
		
		/// The name of the Database
		std::string database_name;
		
		/// The collection of keys <type, value> of the table
		std::map<std::string, std::string> keys;
		
		/// The collection of column names and types to a table
		std::map<std::string, std::string> columns;
		
		/// The collection of arrays of rows for the table.
		std::vector<std::vector<std::string> > rows;
		
		void Insert(std::vector<std::string> row);
		
		void AddKey(std::string key, std::string value);
		
		// TODO: Add column names
		std::vector<std::vector<std::string> > Select(std::vector<std::string> col_names);
		
		void Delete();
		
		Table() {
			
		}
		
		// Use this as as create in DB CreateTable method
		// TODO: Tie into user input
		Table(std::string name) {
			table_name = name;
		}
		
		~Table() {
			
		}
		
};

void Table::AddKey(std::string key, std::string value) {
	std::cout << "Adding: " << key << " " << value << std::endl;
	
	keys.insert(std::pair<std::string, std::string>(key, value));
	
}

// TODO: Tie into user input
void Table::Insert(std::vector<std::string> row) {
	rows.push_back(row);
}

// TODO: Add column names
std::vector<std::vector<std::string> > Table::Select(std::vector<std::string> col_names) {
	return rows;
}

// TODO: Tie into user input
void Table::Delete() {
	delete this;
}
