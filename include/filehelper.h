#pragma once
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <cstring>

class FileHelper {
	private:
		bool static CheckExtension(std::string filename, std::string ext);
	
	public:
		void static listfiles(std::string dir, std::string ext);
		std::string static readfile(std::string dir, std::string filename);
		std::string static writefile(std::string contents, std::string filename, bool isbinary);

};

// Private Functions
bool FileHelper::CheckExtension(std::string filename, std::string ext) {
	return filename.size() >= ext.size() && 0 == filename.compare(filename.size()-ext.size(), ext.size(), ext);
}

// Public Functions
void FileHelper::listfiles(std::string dir, std::string ext) {
	char cDir[dir.length()];
	
	strcpy(cDir, dir.c_str()); 
	
	DIR *dirp = opendir(cDir);
	struct dirent *dp;
	std::string ending = ".db";
	
	std::string str = "";
	
	// TODO: return string instead of output to console
	while((dp = readdir(dirp)) != NULL) {
				
		if(FileHelper::CheckExtension(dp->d_name, ext)){
			std::cout << dp->d_name << std::endl;
		}
		
	}
}

std::string FileHelper::readfile(std::string dir, std::string filename){
	std::string ret = "";	
	std::ifstream file ("data/" + filename);
	std::stringstream buffer;
	
	if (file.is_open()){
		buffer << file.rdbuf();
		ret = buffer.str();
		
		file.close();
	}
	
	return ret;
}

std::string FileHelper::writefile(std::string contents, std::string filename, bool isbinary) {
	std::ofstream out(filename);
		
	out << contents;
	out.close();
}
