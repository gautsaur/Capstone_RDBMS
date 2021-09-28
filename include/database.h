/* 
*	File: 	    database.h
*   Author:     Andrew Nunez
*   Date:       Sept. 23, 2021
*
*   This file holds a class that defines a database and it's data to read and write from a .bin file.
*/

#include "table.h";

class Database {
	public:
		/// Table constructor
		//Database();
		
		/// The name of the database
		std::string database_name;
		
		/// The collection of tables
		std::vector<Table> tables;
		
		// TODO: Accept a list of columns
		void AddTable(Table &tbl) {
			tables.push_back(tbl);
		}
		
		Database();
		
		Database(std::string name) {
			database_name = name;
		}
						
};
