#pragma once
#include <vector>

class Node {
private:
	Node* parent;
	Node** childNodes;
	
	
	unsigned int ID;
	unsigned int index;
	int nChildNodes;
	unsigned int subordinates;

	int capacity = 50;

public:
	Node();
	Node(const Node& node);
	~Node();

	Node** getChildNodes();

	void init();

	void setParent(Node& node);
	void setSubordinates(unsigned int subordinates);
	void setID(unsigned int ID);
	void setnChildNodes(unsigned int n);
	void setIndex(unsigned int i);
	void setChildNodes(Node** list);

	Node* getParent();
	unsigned int getSubordinates();
	unsigned int getID();
	int getnChildNodes();

	bool addChild(Node& child);
	void removeParent();
	bool removeChild(unsigned int index);
	unsigned int getIndex();
	void printChildren();
};