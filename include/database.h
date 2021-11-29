/*
*	File: 	    database.h
*   Author:     Andrew Nunez
*   Date:       Sept. 23, 2021
*
*   This file holds a class that defines a database and it's data to read and write from a .bin file.
*/
#pragma once
#include <algorithm>
#include "filehelper.h"
#include "table.h"
#include <stdbool.h>

class Database {
	private:
		void Read(std::string db_name);

	public:
		/// The name of the database
		std::string database_name;

		/// The collection of tables
		std::vector<Table> tables;

		static void List();

		void List_Tables();

		void AddTable(Table &tbl);

		void Delete();

		void Save();

		void DropTable(std::string name);

		bool find_table(std::string name);

		void insert_into_table(string table_name, vector<string> cols, vector<vector<string> > data);

		//Table * get_table(std::string tbl_name);
		Table get_table(std::string tbl_name);

		Database(){

		}

		// Use this a the create a DB
		// TODO: Tie into user input
		Database(std::string name) {
			Read(name);

		}

};

/// Author: Andrew Nunez
/// Date: 09-28-2021
/// Deletes the current data base
/// Todo: Delete the associated .db file aswell.
void Database::Delete() {
    //remove file
	delete this;

}

/// Author: Andrew Nunez
/// Date: 09-28-2021
/// Saves the current database state to disk.
void Database::Save(){
	std::string line;
	std::string contents;
	std::ofstream out("data/" + database_name + ".db");

	contents = "database:" + database_name + "\n";

	auto table = tables.begin();

	for( ; table != tables.end(); table++){
		contents += ("table_name:" + table->table_name);

		// Add keys
		if(table->keys.size() > 0){
			contents += "\nkeys:";
			for (auto const& key : table->keys) {
				contents += (key.first + " " + key.second + ",");

			}

			contents.pop_back();
		}




		// Add Columns
		if(table->columns.size() > 0){
			contents += "\ncolumns:";
			for (auto const& column : table->columns) {
				contents += (column.first + " " + column.second + ",");

			}

			contents.pop_back();
		}


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

void Database::DropTable(std::string name) {
    int count = 0;
    for (auto& it: tables){
        if (it.table_name == name){
            tables.erase(tables.begin()+count);

            this->Save();
        }
        count = count +1;
    }

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
				tmp_parent_array = Parser::split_str(line, ',');

				for(i = 0; i < tmp_size; i++) {
					tmp_v.push_back(tmp_parent_array[i]);
				}

				rows.push_back(tmp_v);

			} else if (line.find(",") != std::string::npos) {
				tmp_parent_array = Parser::split_str(line, ',');

				for(i = 0; i < tmp_size; i++) {
					tmp_child_array = Parser::split_str(tmp_parent_array[i], ' ');

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

void Database::List_Tables() {
	for(Table tbl : tables) {
		std::cout << tbl.table_name << std::endl;
	}
}

//Janita Aamir
//Date: mm-dd-yy
//This function checks to see if the given table name exists within a database.
bool Database::find_table(std::string name) {
	for(Table tbl : tables) {

		if (name == tbl.table_name){
            return true;
		}
		else return false;
	}
}

void Database::insert_into_table(string table_name, vector<string> cols, vector<vector<string> > data) {
	for(Table tbl : tables){
		if(tbl.table_name == table_name){
			// Begin Insert
			string message;

			vector<int> indexes;

			for(vector<string> row : data){
				if(row.size() != tbl.columns.size()){
					message = "Row count does not match column count.";
				}
			}

			if(message.length() <= 0){
				for(string u_col : cols){
					string col = Utils::trim(u_col);
					int index = tbl.get_column_index(col);


					if(index > -1){
						indexes.push_back(index);
					} else {
						message = "Invalid column name:" + col + ";";
					}
				}
			}

			if(message.length() <= 0){
				vector<string> new_row;

				for(vector<string> row : data){
					if(row.size() == tbl.columns.size()) {
						new_row = row;
					} else {
						int current_index = 0;

						for(int index : indexes){

							while(current_index < index){
								new_row.push_back("NULL");

								current_index += 1;
							}

							new_row.push_back(Utils::trim(row[current_index]));

							current_index += 1;
						}

					}

					tbl.Insert(new_row);

				}

				message = "Success!";

				DropTable(table_name);
				AddTable(tbl);

			}



			cout << message << endl;

			return;
		}
	}


}

Table Database::get_table(std::string name){
	Table ret;

	for(Table tbl : tables){
		if(tbl.table_name == name){
			ret = tbl;

			break;
		}
	}

	return ret;

}
