#include "IO.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <random>
#include <sstream>
#include <cstdint>

void IO::writeToFile(const std::string fileName, const int size)
{
	srand((unsigned int)time(NULL));
	std::ofstream file(fileName);
	file << "1 0" << std::endl;
	int s = 2;
	int counter = 0;
	for (int i = 2; i <= size; i++) {
		if (i >= 2 && i <= 4) {
			file << i << ' ' << 1 << std::endl;
		}
		else if (i > 4 && counter == 2) {
			file << i << ' ' << s << std::endl;
			s++;
			counter = 0;
		}
		else {
			file << i << ' ' << s << std::endl;
			counter++;
		}
	}
	file.close();
}

std::string randomString(int len = 16)
{
	std::string str("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

	std::random_device rd;
	std::mt19937 generator(rd());

	std::shuffle(str.begin(), str.end(), generator);

	return str.substr(0, len);    // assumes 32 < number of characters in str         
}

void IO::writeDataToFile(const std::string fileName, const int size)
{
	srand((unsigned int)time(NULL));
	std::ofstream file(fileName);
	int s = 2;
	int counter = 0;
	for (int i = 1; i <= size; i++) {
		file << i << "\t" << randomString() << "\t" << randomString() << "\t" << rand() % 100 << "\t" << rand() % 900000 + 50000 << "\t" << rand() % 9000 + 1000 << "\t" << rand() % 200 << "\t" << rand() % 100 << "\t" << randomString() << "\t" << randomString() << std::endl;;
	}
	file.close();
}

void IO::readFromFile(const std::string fileName, const int size, Node ** nodes, Node aaa[], int nNodes)
{
	std::ifstream in(fileName);
	std::string line;
	int id = 0, sub = 0;
	int iii = 0;
	if (in.is_open())
	{
		while (in >> id >> sub) {
			aaa[iii].setID(id);
			aaa[iii].setSubordinates(sub);
			iii++;
			if (nNodes != 0) {
				if (iii == nNodes) {
					break;
				}
			}
		}
	}
	for (int i = 0; i < size; i++) {
		nodes[i] = &aaa[i];
	}
}

NodeData IO::getNodeData(const char * fileName, unsigned int ID)
{
	std::ifstream in(fileName);
	std::string line;
	NodeData data;
	unsigned int id = 0, staff = 0;
	float annualBudget = 0.f, expenses = 0.f, income = 0.f, loss = 0.f;
	std::string name, head, description, keyAchievements;

	if (in.is_open())
	{
		while (std::getline(in, line))
		{
			std::istringstream iss(line);
			if (!(iss >> id >> name >> head >> staff >> annualBudget >> expenses >> income >> loss >> description >> keyAchievements)) {
				break;
			}
			if (id == ID) {
				data.unitName = name;
				data.headOfUnit = head;
				data.numStaff = staff;
				data.annualBudget = annualBudget;
				data.expenses = expenses;
				data.income = income;
				data.loss = loss;
				data.activityDesc = description;
				data.keyAchievements = keyAchievements;
				break;
			}
		}
	}
	in.close();
	return data;
}

std::vector<NodeData> IO::getMultiNodeData(const char * fileName, std::vector<unsigned int>& ID)
{
	std::ifstream in(fileName);
	std::string line;
	NodeData data;
	unsigned int id = 0, staff = 0;
	float annualBudget = 0.f, expenses = 0.f, income = 0.f, loss = 0.f;
	std::string name, head, description, keyAchievements;

	std::vector<NodeData> dataset;

	if (in.is_open())
	{
		for (int i = 0; i < ID.size(); i++)
		{

			while (std::getline(in, line))
			{
				std::istringstream iss(line);
				if (!(iss >> id >> name >> head >> staff >> annualBudget >> expenses >> income >> loss >> description >> keyAchievements)) {
					break;
				}
				if (id == ID[i])
				{

					data.unitID = id;
					data.unitName = name;
					data.headOfUnit = head;
					data.numStaff = staff;
					data.annualBudget = annualBudget;
					data.expenses = expenses;
					data.income = income;
					data.loss = loss;
					data.activityDesc = description;
					data.keyAchievements = keyAchievements;
					dataset.push_back(data);
					break;
				}
			}
		}
	}
	in.close();
	return dataset;
}

bool IO::writeHierarchyToFile(const std::string filePath, NodeTree & tree)
{
	std::ofstream out(filePath);
	for (unsigned int i = 0; i < tree.nodeArraySize; i++)
	{
		if (tree.nodeArray[i]->getParent() == nullptr)
		{
			out << tree.nodeArray[i]->getID() << ' ' << 0 << std::endl;
		}
		else
		{
			out << tree.nodeArray[i]->getID() << ' ' << tree.nodeArray[i]->getParent()->getID() << std::endl;
		}
	}
	out.close();
	return false;
}

std::vector<NodeData> IO::getFilteredDataBudget(const std::string filePath, std::string condition, float budget)
{
	std::ifstream in(filePath);
	std::string line;

	std::vector<NodeData> dataArray;
	NodeData data;

	unsigned int id = 0, staff = 0;
	float annualBudget = 0.f, expenses = 0.f, income = 0.f, loss = 0.f;
	std::string name, head, description, keyAchievements;

	if (in.is_open())
	{
		while (std::getline(in, line))
		{
			std::istringstream iss(line);
			if (!(iss >> id >> name >> head >> staff >> annualBudget >> expenses >> income >> loss >> description >> keyAchievements))
			{
				break;
			}
			data.unitID = id;
			if (condition == ">")
			{
				if (annualBudget > budget)
				{
					data.annualBudget = annualBudget;
					dataArray.push_back(data);
				}
			}
			else if (condition == "<")
			{
				if (annualBudget < budget)
				{
					data.annualBudget = annualBudget;
					dataArray.push_back(data);
				}
			}
			else if (condition == "=")
			{
				if (annualBudget == budget)
				{
					data.annualBudget = annualBudget;
					dataArray.push_back(data);
				}
			}
			else
			{
				data.annualBudget = 0;
			}

		}
	}
	in.close();

	return dataArray;
}

