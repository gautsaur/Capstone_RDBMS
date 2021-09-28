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
		
		void AddRow(std::vector<std::string> row) {
			rows.push_back(row);
		}
		
		Table(std::string name) {
			table_name = name;
		}
		
};
