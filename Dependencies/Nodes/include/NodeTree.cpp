#include "NodeTree.h"
#include <iostream>
#include <string>

NodeTree::NodeTree()
{
	root = nullptr;
}

NodeTree::NodeTree(Node* root)
{
	this->root = root;
}

void NodeTree::setRoot(Node* node)
{
	root = node;
}
Node* NodeTree::getRoot()
{
	return root;
}

void NodeTree::printTree(Node& node)
{
	int counter = node.getnChildNodes();
	std::cout << node.getID() << std::endl;
	std::string branch = "|-------";
	char tab = '\t';
	for (int i = 0; i < counter; i++)
	{
		for (int j = 0; j < getDepth(&node); j++)
		{
			std::cout << tab;
		}

		std::cout << branch;
		printTree(*node.getChildNodes()[i]);
	}
}

unsigned int NodeTree::getHeight(Node* node)
{
	if (node != nullptr)
	{
		int max = 0;
		for (int i = 0; i < node->getnChildNodes(); i++) 
		{
			int height = getHeight(node->getChildNodes()[i]);
			if (height > max)
			{
				max = height;
			}
		}
		return 1 + max;
	}
	else 
		return 0;
}

unsigned int NodeTree::getDepth(Node* leaf)
{
	if (leaf->getParent() != nullptr)
	{
		int max = 0;
		max = getDepth(leaf->getParent());
		return 1 + max;
	}
	else
		return 0;
}

unsigned int NodeTree::getnNodes(Node* node)
{
	if (node != nullptr)
	{
		int count = 1;
		for (int i = 0; i < node->getnChildNodes(); i++) 
		{
			count += getnNodes(node->getChildNodes()[i]);
		}
		return count;
	}
	else
		return 0;
}

Node* NodeTree::findNode(Node* node, unsigned int ID)
{
	if (node != nullptr)
	{
		if (node->getnChildNodes() != 0)
		{
			for (int i = 0; i < node->getnChildNodes(); i++)
			{
				if (node->getChildNodes()[i]->getID() == ID)
				{
					return node->getChildNodes()[i];
				}
				else
				{
					findNode(node->getChildNodes()[i], ID);
				}
			}
		}
	}
	return nullptr;
}

void NodeTree::buildTree(Node * node, Node** list, int size)
{
	for (int i = 0; i < size; i++) {
		if ((list[i]->getSubordinates() == node->getID()) && findNode(root, list[i]->getID()) == nullptr) {
			node->addChild(*list[i]);
			buildTree(list[i], list, size);
		}
	}
}

bool NodeTree::findAndSetRoot(Node** list, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (list[i]->getSubordinates() == 0) {
			setRoot(list[i]);
			return true;
		}
	}
	return false;
}
