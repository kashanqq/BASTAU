#pragma once
#include "Node.h"
#include <vector>

class NodeTree {

private:
	Node * root;

public:
	NodeTree();
	NodeTree(Node* node);
	void setRoot(Node* node);
	Node* getRoot();
	static void printTree(Node& node); 
	unsigned int getHeight(Node* node);
	static unsigned int getDepth(Node* leaf);
	unsigned int getnNodes(Node* node);
	static Node* findNode(Node* node, unsigned int ID);
	void buildTree(Node* node, Node** list, int size);
	bool findAndSetRoot(Node** list, int size);
	static std::string getNodeInfo();
};
