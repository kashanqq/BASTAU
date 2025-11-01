#pragma once

#ifndef NODETREE_H
#define NODETREE_H

#include "NodeData.h"
#include "Node.h"

class NodeTree
{
private:
	friend class IO;

	Node * root;

	//irr::core::list<Node*> nodes; TO BE IMPLEMENTED

	//Main array for nodes
	Node ** nodeArray;
	unsigned int nodeArraySize;

	//Temporary array for new nodes
	Node ** tempArray; //
	unsigned int tempArraySize; //
	int capacity = 10;

	//Size of whole tree
	int actualSize;

public:
	///Constructors
	NodeTree();
	NodeTree(Node* node);

	///Destructor
	~NodeTree();

	///Setters
	void setRoot(Node* node);
	void setNodeArray(Node** array, unsigned int size);
	void setNodeInfo(const char* filename);

	///Getters
	Node* getRoot();
	Node** getNodeArray();
	int getNodeArraySize();
	unsigned int getnNodes(Node* node);
	unsigned int getHeight(Node* node);
	static unsigned int getDepth(Node* leaf);
	int getCurrentSize();
	int getActualSize();

	///Initialization
	void buildTreeFromDB();
	void buildTreeLogical(Node* node);
	void buildTreeGraphicalCone(Node * node, irr::video::IVideoDriver* driver, scene::ISceneManager* smgr, core::vector3df& position, core::vector3df rootPosition, float radius, float yHeight, const irr::io::path& defaultTextureFileName);
	void buildTreeGraphicalBalloon(Node * node, irr::video::IVideoDriver* driver, scene::ISceneManager* smgr, core::vector3df& position, float K, float P, float radius, float yHeight, const irr::io::path& defaultTextureFileName);
	void buildTreeGraphicalH(Node * node, irr::video::IVideoDriver* driver, scene::ISceneManager* smgr, core::vector3df& position, float radius, float yHeight, const irr::io::path& defaultTextureFileName);
	void buildTreeGraphicalHemisphere(Node * node, irr::video::IVideoDriver* driver, scene::ISceneManager* smgr, core::vector3df& origin, core::vector3df& position);

	bool findAndSetRoot(Node** list, int size);

	///Manipulation
	bool update(scene::ISceneManager* smgr);
	void merge();
	void addNode(Node* parent, Node* child);
	void deleteNode(Node* node, int state = 0);

	///Traversal
	static Node* findNode(Node* node, unsigned int ID);
	Node* findNode(scene::ISceneNode* ptr);
	Node* findNodei(scene::ISceneNode* ptr);
	Node* findNode(unsigned int ID);
	///Visual

	//*Render link between node and its parent
	void render(Node * node, irr::video::IVideoDriver* driver);

	///Debugging
	static void printTree(Node& node);

	///Undefined

};

#endif