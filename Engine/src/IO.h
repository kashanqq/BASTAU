#pragma once
#include "Node.h"
#include "NodeData.h"
#include "NodeTree.h"
#include <string>
#include <vector>

class IO {
public:
	void writeToFile(const std::string fileName, const int size);
	void writeDataToFile(const std::string fileName, const int size);
	void readFromFile(const std::string fileName, const int size, Node** nodes, Node aaa[], int nNodes = 0);
	NodeData getNodeData(const char* fileName, unsigned int ID);
	std::vector<NodeData> getMultiNodeData(const char* fileName, std::vector<unsigned int>& ID);

	bool writeHierarchyToFile(const std::string filePath, NodeTree& tree);


	static std::vector<NodeData> getFilteredDataBudget(const std::string filePath, std::string condition, float budget);

	void writeToFileWithParams(const std::string fileName, const int size);
	void readFromFileWithParams(const std::string fileName, const int size, Node** nodes, Node aaa[], int nNodes = 0);
};