#include <iostream>
#include <exception>
#include <filesystem>
#include <string>
#include <fstream>
#include <iterator>

class FileNotFound : public std::runtime_error
{
public:
   explicit FileNotFound(std::filesystem::path filePath) 
        : std::runtime_error("File not found: " + filePath.string()) {}

    std::filesystem::path filepath() const 
    { 
        return fullFileName; 
    }

private:
	std::string fullFileName;

};


class NotRegularFile : public std::runtime_error
{

public:
	explicit NotRegularFile(std::filesystem::path filePath)
		: std::runtime_error("Not a regular file: " + filePath.string()) {}
	std::filesystem::path targetPath() const
	{
		return fullFileName;
	}
private :
	std::filesystem::path fullFileName;
};


class CannotOpenFile : public std::runtime_error
{
public:
	explicit CannotOpenFile(std::filesystem::path filePath)
		:std::runtime_error("Cannot open file: " + filePath.string()) {}

	std::filesystem::path targetPath() const
	{
		return fullFileName;
	}
private :
	std::filesystem::path fullFileName;
};


std::string readFile(const std::filesystem::path& filePath){
	if (!std::filesystem::exists(filePath)){
		throw FileNotFound(filePath);
	}
	if (!std::filesystem::is_regular_file(filePath)){
		throw NotRegularFile(filePath);
	}
	std::ifstream file(filePath);
	if (!file.is_open()){
		throw CannotOpenFile(filePath);
	}
	return std::string(
		(std::istreambuf_iterator<char>(file)), 
		std::istreambuf_iterator<char>());
}



int main() {
	try {
		auto content = readFile("/home/michael/projects/c++/hands_on/static_variable.cpp");
		std::cout << content << std::endl;
		return 0;
	}
	catch (const FileNotFound& e) {
		std::cerr << "Cant find file: " << e.filepath() << std::endl;
		std::cerr << e.what() << std::endl;
	}
	catch (const NotRegularFile& e) {
		std::cerr << "Not a regular file: " << e.targetPath() << std::endl;	
		std::cerr << e.what() << std::endl;
	}
	catch (const CannotOpenFile& e) {
		std::cerr << "Cannot open file: " << e.targetPath() << std::endl;
		std::cerr << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
	}
	return 1;
}

