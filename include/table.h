
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
		
		/// The collection of column names to a table
		std::map<std::string, std::string> columns;
		
		/// The collection of arrays of rows for the table.
		std::vector<std::vector<std::string> > rows;
		
		// TODO: Tie into user input
		void Insert(std::vector<std::string> row) {
			rows.push_back(row);
		}
		
		// TODO: Tie into user input
		void Delete() {
			delete this;
		}
		
		// TODO: Add column names
		std::vector<std::vector<std::string> > Select(std::vector<std::string> col_names) {
			return rows;
		}
		
		// Use this as as create in DB CreateTable method
		// TODO: Tie into user input
		Table(std::string name) {
			table_name = name;
		}
		
};