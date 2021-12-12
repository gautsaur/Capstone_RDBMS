/*
*	File: 	    database.h
*   Author:     Andrew Nunez, Janita Aamir, Saurav Gautam, Sanjeev Thakur
*   Date:       Sept. 23, 2021
*
*   This file holds a class that defines a database and it's data to read and write from a .bin file.
*/
#pragma once
#include <algorithm>
#include "filehelper.h"
#include "table.h"
#include <stdbool.h>

class Database
{
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

	void SaveTable(Table table);
	void DropTable(std::string name);

	void CreateTable(std::string tableName);

	bool find_table(std::string name);

	void insert_into_table(string table_name, vector<string> cols, vector<vector<string>> data);

	void insert_into(std::string statement, std::string table_name);

	Table get_table(std::string tbl_name);


	// Base constructor database
	Database()
	{}

	// Use this a the create a DB
	// TODO: Tie into user input
	Database(std::string name)
	{
		Read(name);
	}
};

/// Author: Andrew Nunez
/// Date: 09-28-2021
/// Deletes the current data base
/// Todo: Delete the associated .db file aswell.
void Database::Delete()
{
	//remove file
	delete this;
}

/// Author: Andrew Nunez
/// Date: 09-28-2021
/// Saves the current database state to disk.
void Database::Save()
{
	std::string line;
	std::string contents;
	std::ofstream out("data/" + database_name + ".db");

	contents = "database:" + database_name + "\n";

	auto table = tables.begin();

	for (; table != tables.end(); table++)
	{
		contents += ("table_name:" + table->table_name);

		// Add keys
		if (table->keys.size() > 0)
		{
			contents += "\nkeys:";
			for (auto const &key : table->keys)
			{
				contents += (key.first + " " + key.second + ",");
			}

			contents.pop_back();
		}

		// Add Columns
		if (table->columns.size() > 0)
		{
			contents += "\ncolumns:";
			for (auto const &column : table->columns)
			{
				contents += (column.first + " " + column.second + ",");
			}

			contents.pop_back();
		}

		// Add Rows
		for (auto &row : table->rows)
		{
			contents += "\nrow:";

			for (auto &val : row)
			{
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
void Database::AddTable(Table &tbl)
{
	tables.push_back(tbl);
}

void Database::DropTable(std::string name)
{
	int count = 0;
	for (auto &it : tables)
	{
		if (it.table_name == name)
		{
			tables.erase(tables.begin() + count);

			this->Save();
		}
		count = count + 1;
	}
}

/// Author: Andrew Nunez
/// Date: 09-28-2021
/// Reads the given file name and initializes an object from the contents
void Database::Read(std::string db_name)
{
	int tmp_size, i = 0;
	std::string line;
	std::string *tmp_parent_array;
	std::string *tmp_child_array;
	std::ifstream file("data/" + db_name + ".db");

	std::string tbl_name;
	std::map<std::string, std::string> keys;
	std::vector<std::vector<std::string>> rows;
	std::map<std::string, std::string> columns;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			tmp_size = std::count(line.begin(), line.end(), ',') + 1;

			// Begin parsing each line
			if (line.find("database:") == 0)
			{
				database_name = line.substr(line.find(":") + 1);
			}
			else if (line.find("table_name:") == 0)
			{
				tbl_name = line.substr(line.find(":") + 1);
			}
			else if (line.find("row:") == 0)
			{
				std::vector<std::string> tmp_v;
				tmp_parent_array = Parser::split_str(line, ',');

				for (i = 0; i < tmp_size; i++)
				{
					tmp_v.push_back(tmp_parent_array[i]);
				}

				rows.push_back(tmp_v);
			}
			else if (line.find(",") != std::string::npos)
			{
				tmp_parent_array = Parser::split_str(line, ',');

				for (i = 0; i < tmp_size; i++)
				{
					tmp_child_array = Parser::split_str(tmp_parent_array[i], ' ');

					if (line.find("keys:") == 0)
					{
						keys.insert({tmp_child_array[0], tmp_child_array[1]});
					}
					else if (line.find("columns:") == 0)
					{
						columns.insert({tmp_child_array[0], tmp_child_array[1]});
					}
				}
			}
			else if (line.find(";") == 0)
			{
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
			else
			{
				std::cout << "Database is Corrupt!" << std::endl;
			}
		}
		file.close();
	}
	else
	{
		std::cout << "Database does not exist!" << std::endl;
	}
}

void Database::List()
{
	FileHelper::listfiles("data", "db");
}

void Database::List_Tables()
{
	for (Table tbl : tables)
	{
		std::cout << tbl.table_name << std::endl;
	}
}

bool Database::find_table(std::string name)
{
	for (Table tbl : tables)
	{

		if (name == tbl.table_name)
		{
			return true;
		}
		else
			return false;
	}
}

void Database::insert_into_table(string table_name, vector<string> cols, vector<vector<string>> data)
{
	for (Table tbl : tables)
	{
		if (tbl.table_name == table_name)
		{
			// Begin Insert
			string message;

			vector<int> indexes;

			for (vector<string> row : data)
			{
				if (row.size() != tbl.columns.size())
				{
					message = "Row count does not match column count.";
				}
			}

			if (message.length() <= 0)
			{
				for (string u_col : cols)
				{
					string col = Utils::trim(u_col);
					int index = tbl.get_column_index(col);

					if (index > -1)
					{
						indexes.push_back(index);
					}
					else
					{
						message = "Invalid column name:" + col + ";";
					}
				}
			}

			if (message.length() <= 0)
			{
				vector<string> new_row;

				for (vector<string> row : data)
				{
					if (row.size() == tbl.columns.size())
					{
						new_row = row;
					}
					else
					{
						int current_index = 0;

						for (int index : indexes)
						{

							while (current_index < index)
							{
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

Table Database::get_table(std::string name)
{
	Table ret;

	for (Table tbl : tables)
	{
		if (tbl.table_name == name)
		{
			ret = tbl;

			break;
		}
	}

	return ret;
}


/// Author: Saurav Gautam
/// Save the table after being updated in the console
/// Replaces the table by creating a new one
void Database::SaveTable(Table table)
{

	std::string tableName = table.table_name;
	int count = 0;

	for (Table tbl : tables)
	{
		if (tbl.table_name == tableName)
		{
			break;
		}
		count = count + 1;
	}
	
	tables[count] = table;
}


/// Author: Saurav Gautam
/// Read the insert statement and insert values in the table
void Database::insert_into(std::string statement, std::string table_name)
{
	cout << "Beginning Insert for:" << table_name << ";" << endl;
	Table current_table = get_table(table_name);;
	vector<string> columns = Parser::get_insert_columns(statement, table_name);
	vector<vector<string> > values = Parser::get_insert_rows(statement, table_name);	

	vector<string> col_names = current_table.get_column_names();

	vector<int> order;

	cout << "Beginning Column sorting" << endl;

	for (int i = 0; i < columns.size(); i++)
	{
		auto itr = std::find(columns.begin(), columns.end(), col_names.at(i));
		if (itr != columns.cend())
		{

			order.push_back((std::distance(columns.begin(), itr)));
		}
		else
		{
			order.push_back(-1);
		}
	}

	int cnt = 0;

	cout << "Done. Beginning row Sorting" << endl;

	for(vector<string> row : values)
	{
		vector<string> temp;

		for (int j = 0; j < order.size(); j++)
		{
			cout << order[j]<<"\n";
			if (order[j] == -1)
			{
				temp.push_back("NULL");
			}
			else
			{
				temp.push_back(row[order[j] + (cnt * columns.size())]);
			}
		}
		current_table.Insert(temp);
	}

	SaveTable(current_table);
	
	cout << "Inserted values in rows!"
		 << "\n";
}
