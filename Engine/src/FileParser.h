#pragma once
#include <fstream>
#include <iostream>
#include <string>
class FileParser
{
public:
	void readFile(std::string fileName);
	void file1(std::string fileName);
	void file2(std::string fileName);
	void file3(std::string fileName);
	std::string find(std::string fileName, std::string name);
};