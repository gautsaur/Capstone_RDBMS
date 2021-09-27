/* 
*	File: 	   table.h
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
		/// Table constructor
		Table();
		
		/// The name of the table
		string table_name;
		
		/// The name of the Database
		string database_name;
		
		/// The collection of keys <type, value> of the table
		std:map<string, string> keys;
		
		/// The collection of column names to a table
		vector<string> column_names;
		
		/// The collection of arrays of rows for the table.
		vector<vector<string>> rows;
		
};
