#include "FileParser.h"

void FileParser::readFile(std::string fileName)
{
	int i = 0;
	int index = 2;
	std::ifstream in(fileName);
	std::string fName = "data.txt";
	std::ofstream out1(fName);
	std::string parts[12];
	std::string line;
	if (in.is_open())
	{
		while (!in.eof())
		{
			while (std::getline(in, parts[i], '~'))
			{
				if (parts[0][0] == '\n') parts[0].erase(0, 1);
				i++;
				if (i == 12)
				{
					if (parts[2] == "0")
					{
						line = "1~" + parts[0] + '~' + parts[1] + '~' + parts[2] + '~' + parts[3] + '~' + parts[4] + '~' + parts[5] + '~'
							+ parts[6] + '~' + parts[7] + '~' + parts[8] + '~' + parts[9] + '~' + parts[10] + '~' + parts[11] + '~' + '\n';
					}
					else
					{
						line = std::to_string(index) + "~" + parts[0] + '~' + parts[1] + '~' + parts[2] + '~' + parts[3] + '~' + parts[4] + '~' + parts[5] + '~'
							+ parts[6] + '~' + parts[7] + '~' + parts[8] + '~' + parts[9] + '~' + parts[10] + '~' + parts[11] + '~' + '\n';
						index++;
					}
					out1 << line;
					i = 0;
				}
			}
		}
	}
	else
	{
		std::cout << "ERROR" << std::endl;
	}
	in.close();
	out1.close();
	file1(fName);
	file2(fName);
	file3(fName);
}

void FileParser::file1(std::string fileName)
{
	int i = 0;
	std::ifstream in(fileName);
	std::ofstream out1("unit_data1.txt");
	std::string parts[13];
	std::string line;
	if (in.is_open())
	{
		while (!in.eof())
		{
			while (std::getline(in, parts[i], '~'))
			{
				if (parts[0][0] == '\n') parts[0].erase(0, 1);
				i++;
				if (i == 13)
				{
					line = parts[0] + ' ' + parts[1] + ' ' + parts[2] + ' ' + parts[5] + ' '
						+ parts[6] + ' ' + parts[7] + ' ' + parts[8] + ' ' + parts[9] + ' ' + parts[10] + ' ' + parts[11] + '\n';
					out1 << line;
					i = 0;
				}
			}
		}
	}
	in.close();
	out1.close();
}

void FileParser::file2(std::string fileName)
{
	int i = 0, j = 0;
	std::ifstream in(fileName);
	std::ofstream out1("unit_docs.txt");
	std::string parts[13];
	std::string line;
	if (in.is_open())
	{
		while (!in.eof())
		{
			while (std::getline(in, parts[i], '~'))
			{
				if (parts[0][0] == '\n') parts[0].erase(0, 1);
				i++;
				if (i == 13)
				{
					line = parts[12] + '\n';
					line.erase(0, 1);
					while (line[j] != '\n') {
						if (line[j] == ';') line[j] = ' ';
						j++;
					}
					out1 << line;
					i = 0;
					j = 0;
				}
			}
		}
	}
	in.close();
	out1.close();
}


std::string FileParser::find(std::string fileName, std::string name)
{
	int i = 0;
	std::ifstream in(fileName);
	std::string parts[13];
	if (in.is_open())
	{
		while (!in.eof())
		{
			while (std::getline(in, parts[i], '~'))
			{
				if (parts[0][0] == '\n') parts[0].erase(0, 1);
				i++;
				if (i == 13)
				{
					if (parts[1] == name)
					{
						in.close();
						return parts[0];
					}
					i = 0;
				}
			}
		}
	}
	in.close();
	return NULL;
}

void FileParser::file3(std::string fileName)
{
	int i = 0;
	std::ifstream in(fileName);
	std::ofstream out1("node_link.txt");
	std::string parts[13];
	std::string line;
	if (in.is_open())
	{
		while (!in.eof())
		{
			while (std::getline(in, parts[i], '~'))
			{
				if (parts[0][0] == '\n') parts[0].erase(0, 1);
				i++;
				if (i == 13)
				{
					if (parts[3] != "0") {
						line = parts[0] + ' ' + find(fileName, parts[3]) + '\n';
					}
					else {
						line = parts[0] + " 0" + '\n';
					}
					out1 << line;
					i = 0;
				}
			}
		}
	}
	in.close();
	out1.close();
}