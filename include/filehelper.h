#include <fstream>
#include <sstream>
#include <dirent.h>
#include <string.h>
#include <iostream>

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
	
	while((dp = readdir(dirp)) != NULL) {
				
		if(FileHelper::CheckExtension(dp->d_name, ext)){
			std::cout << dp->d_name << std::endl;
		}
		
	}
}

std::string FileHelper::readfile(std::string dir, std::string filename){
	std::string ret = "";
}

std::string FileHelper::writefile(std::string contents, std::string filename, bool isbinary) {
	
}