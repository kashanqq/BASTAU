#include "Node.h"
#include <iostream>

Node::Node()
{
	phi = theta = 0.f;
	r = 0.0f;

	angle = 0.f;
	angleRange = 2.f * PI;

	ID = 0;
	parent = nullptr;
	nChildNodes = 0;
	index = nChildNodes + 1;
	childNodes = nullptr;
	subordinates = 0;
	childNodes = new Node*[capacity];
	node = nullptr;

	color.setAlpha(255);
	color.setRed(145);
	color.setGreen(80);
	color.setBlue(80);
	material.Lighting = false;
	material.Thickness = 0.1f;

}

Node::Node(const Node& node) // TO DO: UPDATE
{
	this->parent = node.parent;
	this->index = node.index;
	this->nChildNodes = node.nChildNodes;
	this->subordinates = node.subordinates;
	this->capacity = node.capacity;

	if (node.childNodes != nullptr) {
		childNodes = new Node*[capacity];
		for (int i = 0; i < nChildNodes; i++)
		{
			childNodes[i] = node.childNodes[i];
		}
	}
	else if (node.childNodes == nullptr)
		childNodes = nullptr;
}

Node::~Node()
{
	parent = nullptr;
	for (int i = 0; i < nChildNodes; i++)
	{
		childNodes[i] = nullptr;
	}
	delete[] childNodes;
	childNodes = nullptr;
	delete data;
}



Node** Node::getChildNodes()
{
	return childNodes;
}

//TODO optimize
bool Node::init(scene::ISceneManager* smgr, irr::f32 posX, irr::f32 posY, irr::f32 posZ, video::IVideoDriver* videoDriver, video::ITexture* texture)
{
	node = smgr->addSphereSceneNode(10, 50, 0, ID);
	if (node)
	{
		node->setPosition(core::vector3df(posX, posY, posZ));
		node->setMaterialTexture(0, texture);
		node->setMaterialFlag(video::EMF_LIGHTING, true);
		node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
		node->getMaterial(0).Shininess = 50.f;
		node->getMaterial(0).SpecularColor.set(255, 150, 150, 150);
		return true;
	}
	else
		return false;
}

void Node::render(video::IVideoDriver* driver, video::SMaterial* lineMaterial, video::SColor* lineColor)
{
	if (node)
	{
		if (parent != nullptr)
		{
			connection.start = node->getAbsolutePosition();
			connection.end = parent->getISceneNode()->getAbsolutePosition();

			if (lineMaterial != nullptr)
			{
				driver->setMaterial(*lineMaterial);
			}
			else
			{
				driver->setMaterial(material);
			}

			driver->setTransform(video::ETS_WORLD, core::matrix4());

			if (lineColor != nullptr)
			{
				driver->draw3DLine(connection.start, connection.end, *lineColor);
			}
			else
			{
				driver->draw3DLine(connection.start, connection.end, color);
			}

		}
	}
}

void Node::setLineMaterial(video::SMaterial & lineMaterial)
{
	material = lineMaterial;
}

void Node::setLineColor(video::SColor & lineColor)
{
	color = lineColor;
}


void Node::setParent(Node& node)
{
	parent = &node;
}

void Node::setSubordinates(unsigned int subordinates)
{
	this->subordinates = subordinates;
}

void Node::setID(unsigned int ID)
{
	this->ID = ID;
}

void Node::setnChildNodes(unsigned int n)
{
	nChildNodes = n;
}

void Node::setIndex(unsigned int i)
{
	index = i;
}

void Node::setChildNodes(Node ** list)
{
	//TO DO
}

Node * Node::getParent()
{
	return parent;
}

unsigned int Node::getSubordinates()
{
	return subordinates;
}

unsigned int Node::getID()
{
	return ID;
}
int Node::getnChildNodes()
{
	return nChildNodes;
}

scene::ISceneNode* Node::getISceneNode()
{
	return node;
}

void Node::setAngle(float angle)
{
	this->angle = angle;
}

void Node::setAngleRange(float angleRange)
{
	this->angleRange = angleRange;
}

float Node::getAngle()
{
	return angle;
}
float Node::getAngleRange()
{
	return angleRange;
}

bool Node::addChild(Node& child)
{
	try {
		child.parent = this;
		if (nChildNodes == capacity) {
			capacity = 2 * capacity;
			Node** temp = new Node*[capacity];
			int counter = 0;
			for (int i = 0; i < nChildNodes; i++) {
				temp[i] = childNodes[i];
				childNodes[i] = nullptr;
				counter++;
			}
			delete[] childNodes;
			childNodes = new Node*[capacity];
			for (int i = 0; i < counter; i++) {
				childNodes[i] = temp[i];
				temp[i] = nullptr;
			}
			delete[] temp;
		}
		childNodes[nChildNodes] = &child;
		child.index = nChildNodes + 1;
		nChildNodes++;
		return true;
	}
	catch (Node node) {
		return false;
	}
}

void Node::removeParent()
{
	parent = nullptr;
}

bool Node::removeChild(unsigned int index)
{
	try
	{
		if (index == 0) {
			return false;
		}
		else if (index == nChildNodes) {
			nChildNodes--;
			childNodes[nChildNodes]->removeParent();
			childNodes[nChildNodes] = nullptr;
			return true;

		}
		else if ((int)index < nChildNodes) {
			nChildNodes--;
			childNodes[index - 1]->removeParent();
			for (int i = index - 1; i < nChildNodes; i++) {
				childNodes[i] = childNodes[i + 1];
				childNodes[i]->index--;
			}
			childNodes[nChildNodes + 1] = nullptr;
			return true;
		}
		else
			return false;
	}
	catch (Node n)
	{
		return false;
	}
}

unsigned int Node::getIndex()
{
	return index;
}

void Node::printChildren()
{
	for (int i = 0; i < nChildNodes; i++)
	{
		std::cout << childNodes[i]->getID() << std::endl;
	}
}
