#pragma once
#ifndef NODE_H
#define NODE_H

#include <irrlicht.h>
#include "NodeData.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace io;
using namespace gui;

class Node {
private:
	//NODE
	scene::ISceneNode* node;

	///For drawing connections between nodes
	core::line3df connection;
	video::SMaterial material;
	video::SColor color;

	float angle;
	float angleRange;


	//NODE

	Node* parent;
	Node** childNodes;

	unsigned int ID;
	unsigned int index;
	int nChildNodes;
	unsigned int subordinates;

	int capacity = 50;

public:
	NodeData * data;

	///Polar coordinates
	float r;
	float phi;
	float theta;
	///Constructors
	Node();
	Node(const Node& node); //TO DO: UPDATE
							///Destructor
	~Node();
	///Setters

	///Getters

	//*Returns pointer to pointer array of child nodes
	Node** getChildNodes();

	bool init(scene::ISceneManager* smgr, irr::f32 posX, irr::f32 posY, irr::f32 posZ, video::IVideoDriver* videoDriver, video::ITexture* texture);
	void render(video::IVideoDriver* driver, video::SMaterial* lineMaterial = 0, video::SColor* lineColor = 0);
	void setLineMaterial(video::SMaterial& lineMaterial);
	void setLineColor(video::SColor& lineColor);

	//TO DO
	void setTexture(const std::string filePath);

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
	scene::ISceneNode* getISceneNode();

	void setAngle(float angle);
	void setAngleRange(float angleRange);
	float getAngle();
	float getAngleRange();

	bool addChild(Node& child);
	void removeParent();
	bool removeChild(unsigned int index);
	unsigned int getIndex();
	void printChildren();

};

#endif