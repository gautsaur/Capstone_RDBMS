/* 
*	File: 	    database.h
*   Author:     Andrew Nunez
*   Date:       Sept. 23, 2021
*
*   This file holds a class that defines a database and it's data to read and write from a .bin file.
*/

#include "table.h"

class Database {
	public:		
		/// The name of the database
		std::string database_name;
		
		/// The collection of tables
		std::vector<Table> tables;
		
		// TODO: Accept a list of columns, tie into user input. This might change to accepting a table name and a list of columns and creating a Table constructor. That may be the cleanest way
		void AddTable(Table &tbl) {
			tables.push_back(tbl);
		}
		
		// TODO: Tie into user input
		void Delete() {
			delete this;
		}
		
		Database();
		
		// User this a the create
		// TODO: Tie into user input
		Database(std::string name) {
			database_name = name;
		}
						
};
