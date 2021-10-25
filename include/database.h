/* 
*	File: 	    database.h
*   Author:     Andrew Nunez
*   Date:       Sept. 23, 2021
*
*   This file holds a class that defines a database and it's data to read and write from a .bin file.
*/

#include <algorithm>
#include "filehelper.h"
#include "table.h"

class Database {
	private:
		void Read(std::string db_name);
		
	public:		
		/// The name of the database
		std::string database_name;
		
		/// The collection of tables
		std::vector<Table> tables;
		
		static void List();
		
		void AddTable(Table &tbl);
		
		void Delete();
			
		void Save();
		
		Table get_table(std::string tbl_name);
				
		Database();
		
		// Use this a the create a DB
		// TODO: Tie into user input
		Database(std::string name) {
			Read(name);
			
		}
						
};

/// Author: Andrew
/// Date: 09-28-2021
/// Splits the provided string on the specified delimiter
std::string* split_str(std::string str, char delim){
	int size = std::count(str.begin(), str.end(), delim) + 1;
	std::string* ret = new std::string[size];
	int i = 0;
	std::stringstream ssin(str.substr(str.find(":") + 1));
		
	while(ssin.good() && i < size) { 
		getline(ssin, ret[i], delim);
		++i;
	}	
	
	return ret;
}

/// Author: Andrew Nunez
/// Date: 09-28-2021
/// Deletes the current data base
/// Todo: Delete the associated .db file aswell.
void Database::Delete() {
	delete this;
}

/// Author: Andrew Nunez
/// Date: 09-28-2021
/// Saves the current database state to disk.	
void Database::Save(){
	std::string line;
	std::string contents;
	std::ofstream out("data/" + database_name + "_V2" + ".db");
	
	contents = "Database:" + database_name + "\n";
	
	auto table = tables.begin();
	
	for( ; table != tables.end(); table++){
		contents += ("table_name:" + table->table_name + "\n");
		
		// Add keys
		contents += "keys:";
		for (auto const& key : table->keys) {
			contents += (key.first + " " + key.second + ",");
						
		}
		
		contents.pop_back();
		
		// Add Columns
		contents += "\ncolumns:";
		for (auto const& column : table->columns) {
			contents += (column.first + " " + column.second + ",");
			
		}
		
		contents.pop_back();
		
		// Add Rows
		for(auto& row:table->rows){
			contents += "\nrow:";
			
			for(auto& val:row){
				contents += (val + ",");
			}
			
			contents.pop_back();
			
		}
		
		contents += "\n;\n";
	}
	
	out << contents;
	out.close();
	
}
	
// TODO: Accept a list of columns, tie into user input. This might change to accepting a table name and a list of columns and creating a Table constructor. That may be the cleanest way
void Database::AddTable(Table &tbl) {
	tables.push_back(tbl);
}
	
/// Author: Andrew Nunez
/// Date: 09-28-2021
/// Reads the given file name and initializes an object from the contents
void Database::Read(std::string db_name) {	
	int tmp_size, i = 0;
	std::string line;
	std::string *tmp_parent_array;
	std::string *tmp_child_array;
	std::ifstream file ("data/" + db_name + ".db");
	
	std::string tbl_name;
	std::map<std::string, std::string> keys;
	std::vector<std::vector<std::string> > rows;
	std::map<std::string, std::string> columns;
		
    if (file.is_open())	{
		while ( getline (file,line) ) {
			tmp_size = std::count(line.begin(), line.end(), ',') + 1;
			
			// Begin parsing each line
			if(line.find("database:") == 0){
				database_name = line.substr(line.find(":") + 1);
				
			} else if (line.find("table_name:") == 0) {
				tbl_name = line.substr(line.find(":") + 1);
				
			}  else if (line.find("row:") == 0) {
				std::vector<std::string> tmp_v;
				tmp_parent_array = split_str(line, ',');
								
				for(i = 0; i < tmp_size; i++) {
					tmp_v.push_back(tmp_parent_array[i]);
				}
				
				rows.push_back(tmp_v);
				
			} else if (line.find(",") != std::string::npos) {
				tmp_parent_array = split_str(line, ',');
				
				for(i = 0; i < tmp_size; i++) {
					tmp_child_array = split_str(tmp_parent_array[i], ' ');
					
					if (line.find("keys:") == 0) {
						keys.insert({tmp_child_array[0], tmp_child_array[1]});
						
					} else if (line.find("columns:") == 0) {
						columns.insert({tmp_child_array[0], tmp_child_array[1]});
					} 
					
				}
				
			} else if (line.find(";") == 0){
				Table *tbl = new Table(tbl_name);
				tbl->keys = keys;
				tbl->rows = rows;
				tbl->columns = columns;
				
				AddTable(*tbl);
								
				rows.clear();
				columns.clear();
				keys.clear();
				
				delete tbl;
				
			}
			else {
				std::cout << "Database is Corrupt!" << std::endl;
			}
			
		}
		
		file.close();
		
	} else {
		std::cout << "Database does not exist!" << std::endl;
		
	} 
	
}

void Database::List() {
	FileHelper::listfiles("data", ".db");
	
}

Table Database::get_table(std::string name){
	Table ret;
	
	auto tbl = find_if(tables.begin(), tables.end(), [&name](const Table& obj) {
		return obj.table_name == name;
	});
	
	if(tbl != tables.end()) {
		ret = *tbl;
	}
	
	return ret;
	
}
