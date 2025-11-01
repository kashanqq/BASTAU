#include "NodeTree.h"
#include <iostream>
#include <string>
#include <math.h>
#include "DBHelper.h"
#include "Functionality.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <unordered_set>
#include <algorithm>

using namespace std::chrono_literals;

NodeTree::NodeTree()
{
	root = nullptr;
	nodeArray = nullptr;
	tempArray = new Node*[capacity];
	nodeArraySize = 0;
	tempArraySize = 0;
	actualSize = 0;
}

NodeTree::NodeTree(Node* root)
{
	this->root = root;
}

NodeTree::~NodeTree()
{
	//TO DO
}

void NodeTree::setRoot(Node* node)
{
	root = node;
}
Node* NodeTree::getRoot()
{
	return root;
}

void NodeTree::setNodeArray(Node ** array, unsigned int size)
{
	nodeArray = array;
	nodeArraySize = size;
}

void NodeTree::setNodeInfo(const char * filename)
{
	std::ifstream in(filename);
	std::string line;
	NodeData data;
	unsigned int id = 0, staff = 0;
	float annualBudget = 0.f, expenses = 0.f, income = 0.f, loss = 0.f;
	std::string name, head, description, keyAchievements;
	std::vector<NodeData> set;


	if (in.is_open())
	{
		while (std::getline(in, line))
		{
			std::istringstream iss(line);
			if (!(iss >> id >> name >> head >> staff >> annualBudget >> expenses >> income >> loss >> description >> keyAchievements)) {
				break;
			}
			data.unitID = id;
			if (name.find('_') != std::string::npos)
			{
				std::replace(name.begin(), name.end(), '_', ' ');
			}
			data.unitName = name;
			if (head.find('_') != std::string::npos)
			{
				std::replace(head.begin(), head.end(), '_', ' ');
			}
			data.headOfUnit = head;
			data.numStaff = staff;
			data.annualBudget = annualBudget;
			data.expenses = expenses;
			data.income = income;
			data.loss = loss;
			data.activityDesc = description;
			data.keyAchievements = keyAchievements;
			set.push_back(data);
		}
	}
	in.close();

	for (unsigned int i = 0; i < nodeArraySize; i++)
	{
		for (unsigned int j = 0; j < set.size(); j++)
		{
			if (nodeArray[i]->getID() == set[j].unitID)
			{
				nodeArray[i]->data = new NodeData(set[j]);
			}
		}
	}

}

Node ** NodeTree::getNodeArray()
{
	return nodeArray;
}

int NodeTree::getNodeArraySize()
{
	return nodeArraySize;
}

void NodeTree::printTree(Node& node)
{
	int counter = node.getnChildNodes();
	std::cout << node.getID() << std::endl;
	std::string branch = "|-------";
	char tab = '\t';
	for (int i = 0; i < counter; i++)
	{
		for (unsigned int j = 0; j < getDepth(&node); j++)
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

bool NodeTree::update(scene::ISceneManager * smgr)
{
	if (!smgr)
	{
		return false;
	}
	smgr->drawAll();
	return true;
}

void NodeTree::merge()
{
	if (tempArraySize == 0)
	{
		return;
	}
	int size = nodeArraySize + tempArraySize;
	Node** temp = new Node*[size];
	for (unsigned int i = 0; i < nodeArraySize; i++)
	{
		temp[i] = nodeArray[i];
		nodeArray[i] = nullptr;
	}
	for (unsigned int i = nodeArraySize, j = 0; j < tempArraySize; i++, j++)
	{
		temp[i] = tempArray[j];
		tempArray[j] = nullptr;
	}
	delete[] nodeArray;
	delete[] tempArray;
	tempArraySize = 0;
	capacity = 10;
	tempArray = new Node*[capacity];

	nodeArray = new Node*[size];

	for (int i = 0; i < size; i++)
	{
		nodeArray[i] = temp[i];
		temp[i] = nullptr;
	}
	delete[] temp;
}

void NodeTree::addNode(Node * parent, Node * child)
{
	if (tempArraySize == capacity)
	{
		capacity += capacity;
		Node** temp = new Node*[capacity];
		for (unsigned int i = 0; i < tempArraySize; i++)
		{
			temp[i] = tempArray[i];
			tempArray[i] = 0;
		}
		delete[] tempArray;
		tempArray = new Node*[capacity];
		for (unsigned int i = 0; i < tempArraySize; i++)
		{
			tempArray[i] = temp[i];
			temp[i] = 0;
		}
		delete[] temp;
	}
	parent->addChild(*child);
	tempArray[tempArraySize] = child;
	tempArraySize++;
}

void NodeTree::deleteNode(Node * node, int state)
{
	if (state == 0)
	{
		int nodePlace = 0;
		for (int i = 0; i < nodeArraySize; i++)
		{
			if (nodeArray[i] == node)
			{
				nodePlace = i;
				break;
			}
		}
		for (int i = nodePlace; i < nodeArraySize - 1; i++)
		{
			nodeArray[i] = nodeArray[i + 1];
		}
		node->removeParent();
		for (int i = 0; i < node->getnChildNodes(); i++)
		{
			node->getChildNodes()[i]->removeParent();
		}
		delete node;
	}
}

Node* NodeTree::findNode(Node* node, unsigned int ID)
{
	if (node != nullptr)
	{
		if (node->getID() == ID)
		{
			return node;
		}
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
Node* NodeTree::findNode(scene::ISceneNode* ptr)
{
	if (ptr) {
		if (nodeArray != nullptr) {
			for (unsigned int i = 0; i < nodeArraySize; i++) {
				if (nodeArray[i]->getISceneNode() == ptr) {
					return nodeArray[i];
				}
			}
		}
		else if (tempArray != nullptr) {
			for (unsigned int i = 0; i < tempArraySize; i++) {
				if (tempArray[i]->getISceneNode() == ptr) {
					return tempArray[i];
				}
			}
		}
	}
	return nullptr;
}
Node * NodeTree::findNodei(scene::ISceneNode * ptr)
{
	if (ptr) {
		if (nodeArray != nullptr)
		{
			Node ** it = nodeArray;
			for (; it != nodeArray + nodeArraySize; ++it)
			{
				Node* node = *it;
				if (node->getISceneNode() == ptr)
				{
					return node;
				}
			}
		}
	}
	return nullptr;
}
Node* NodeTree::findNode(unsigned int ID)
{
	if (ID != 0) {
		if (nodeArray != nullptr) {
			for (unsigned int i = 0; i < nodeArraySize; i++) {
				if (nodeArray[i]->getID() == ID) {
					return nodeArray[i];
				}
			}
		}
		else if (tempArray != nullptr) {
			for (unsigned int i = 0; i < tempArraySize; i++) {
				if (nodeArray[i]->getID() == ID) {
					return tempArray[i];
				}
			}
		}
	}
	return nullptr;
}

void NodeTree::buildTreeLogical(Node * node)
{

	for (int i = 0; i < nodeArraySize; i++) {
		if ((nodeArray[i]->getSubordinates() == node->getID()) && findNode(root, nodeArray[i]->getID()) == nullptr) {
			node->addChild(*nodeArray[i]);
			buildTreeLogical(nodeArray[i]);
		}
	}
}

void NodeTree::buildTreeGraphicalCone(Node * node, irr::video::IVideoDriver* driver, scene::ISceneManager* smgr, core::vector3df& position, core::vector3df rootPosition, float radius, float yHeight, const irr::io::path& defaultTextureFileName)
{
	unsigned int level = getDepth(node);
	const float y = position.Y;
	core::vector3df pos = position;
	float yStep = yHeight;

	float xzStep;
	float r = 0;
	r = radius;

	int count = 0;
	pos.Y -= yStep;
	float xx = 0.0f;
	float zz = 0.0f;
	float angle = 0.0f;

	if (node->getISceneNode() == 0) {
		node->init(smgr, pos.X, y, pos.Z, driver, Functionality::generatePlainColorTexture(video::SColor(255, 22, 146, 255), driver));
	}
	else {
		node->getISceneNode()->setPosition(pos);
	}
	if (getDepth(node) <= 1) {
		pos.Y -= yStep / 0.5f;
	}

	if (getDepth(node) > 1 && getDepth(node) < 4) {
		//node->getISceneNode()->setMaterialTexture(0, driver->getTexture("irrlicht/media/gWt.jpg"));
		pos.Y -= yStep / 1;
	}
	else if (getDepth(node) > 3) {
		//node->getISceneNode()->setMaterialTexture(0, driver->getTexture("irrlicht/media/texture-1196531_960_720.jpg"));
		pos.Y -= yStep / 2;
	}
	for (int i = 0; i < node->getnChildNodes(); i++) {
		if (position.X == 0 && position.Z == 0)
		{
			xzStep = (PI * 2) / (irr::f32)node->getnChildNodes();
			if (node->getnChildNodes() != 1) {
				pos.X = position.X + sin(xzStep * count + (xzStep / 2)) * r;
				pos.Z = position.Z + cos(xzStep * count + (xzStep / 2)) * r;
			}
		}
		else if (position.X > 0 && position.Z > 0)
		{
			core::vector3df nodePos(position.X, 0, position.Z);
			irr::f32 opp = nodePos.getDistanceFrom(core::vector3df(position.X, 0, 0));
			irr::f32 adj = nodePos.getDistanceFrom(core::vector3df(0, 0, position.Z));
			angle = atan(opp / adj);

			xzStep = (PI - PI / 3) / (irr::f32)node->getnChildNodes();
			pos.X = position.X + sin((xzStep * count + (xzStep / 2)) - angle + PI / 6) * r;
			pos.Z = position.Z + cos((xzStep * count + (xzStep / 2)) - angle + PI / 6) * r;
		}
		else if (position.X < 0 && position.Z > 0)
		{
			core::vector3df nodePos(position.X, 0, position.Z);
			irr::f32 opp = nodePos.getDistanceFrom(core::vector3df(position.X, 0, 0));
			irr::f32 adj = nodePos.getDistanceFrom(core::vector3df(0, 0, position.Z));
			angle = atan(opp / adj);

			xzStep = (-PI + PI / 3) / (irr::f32)node->getnChildNodes();
			pos.X = position.X + sin((xzStep * count + (xzStep / 2)) + angle - PI / 6) * r;
			pos.Z = position.Z + cos((xzStep * count + (xzStep / 2)) + angle - PI / 6) * r;
		}
		else if (position.X < 0 && position.Z < 0)
		{
			core::vector3df nodePos(position.X, 0, position.Z);
			irr::f32 opp = nodePos.getDistanceFrom(core::vector3df(position.X, 0, 0));
			irr::f32 adj = nodePos.getDistanceFrom(core::vector3df(0, 0, position.Z));
			angle = atan(opp / adj);

			xzStep = (-PI + PI / 3) / (irr::f32)node->getnChildNodes();
			pos.X = position.X + sin((xzStep * count + (xzStep / 2)) - angle - PI / 6) * r;
			pos.Z = position.Z + cos((xzStep * count + (xzStep / 2)) - angle - PI / 6) * r;
		}
		else if (position.X > 0 && position.Z < 0)
		{
			core::vector3df nodePos(position.X, 0, position.Z);
			irr::f32 opp = nodePos.getDistanceFrom(core::vector3df(position.X, 0, 0));
			irr::f32 adj = nodePos.getDistanceFrom(core::vector3df(0, 0, position.Z));
			angle = atan(opp / adj);

			xzStep = xzStep = (PI - PI / 3) / (irr::f32)node->getnChildNodes();
			pos.X = position.X + sin((xzStep * count + (xzStep / 2)) + angle + PI / 6) * r;
			pos.Z = position.Z + cos((xzStep * count + (xzStep / 2)) + angle + PI / 6) * r;
		}
		else {
			xzStep = (PI * 2) / (irr::f32)node->getnChildNodes();
			pos.X = position.X + sin(xzStep * count + (xzStep / 2)) * r;
			pos.Z = position.Z + cos(xzStep * count + (xzStep / 2)) * r;
		}
		buildTreeGraphicalCone(node->getChildNodes()[i], driver, smgr, pos, pos, r, yStep, defaultTextureFileName);
		count++;
	}

}

void NodeTree::buildTreeGraphicalBalloon(Node * node, irr::video::IVideoDriver* driver, scene::ISceneManager* smgr, core::vector3df& position, float K, float P, float radius, float yHeight, const irr::io::path& defaultTextureFileName)
{
	unsigned int level = getDepth(node);
	const irr::f32 y = position.Y;
	core::vector3df pos = position;
	irr::f32 yStep = yHeight;

	irr::f32 xzStep;
	irr::f32 r = 0;
	r = radius / 2;

	int count = 0;
	pos.Y = yStep * getDepth(node) * (-1);
	irr::f32 xx = 0.0f;
	irr::f32 zz = 0.0f;
	irr::f32 angle = 0.0f;

	if (!node->getISceneNode()) {
		node->init(smgr, pos.X, y, pos.Z, driver, Functionality::generatePlainColorTexture(video::SColor(255, 22, 146, 255), driver));
		if (getDepth(node) > 1 && getDepth(node) < 4) {
			//node->getISceneNode()->setMaterialTexture(0, driver->getTexture("irrlicht/media/gWt.jpg"));
			pos.Y -= yStep / 1;
		}
		else if (getDepth(node) > 3) {
			//node->getISceneNode()->setMaterialTexture(0, driver->getTexture("irrlicht/media/225px-Solid_blue.svg.png"));
			pos.Y -= yStep / 2;
		}
	}
	else {
		node->getISceneNode()->setPosition(pos);
	}

	if (getDepth(node) <= 1) {
		pos.Y -= yStep / 0.5f;
	}
	for (int i = 0; i < node->getnChildNodes(); i++) {

		if (position.X == 0 && position.Z == 0)
		{
			xzStep = (PI * 2) / (irr::f32)node->getnChildNodes();
			if (node->getnChildNodes() != 1) {
				pos.X = position.X + sin(xzStep * count + (xzStep / 2)) * r;
				pos.Z = position.Z + cos(xzStep * count + (xzStep / 2)) * r;
			}
		}
		else
		{
			core::vector3df nodePos(position.X, 0, position.Y);
			irr::f32 opp = nodePos.getDistanceFrom(core::vector3df(position.X, 0, 0));
			irr::f32 adj = nodePos.getDistanceFrom(core::vector3df(0, 0, position.Z));
			angle = atan(opp / adj);

			xzStep = (PI * 2) / (irr::f32)node->getnChildNodes();
			pos.X = position.X + sin((xzStep * count + (xzStep / 2)) - angle) * r;
			pos.Z = position.Z + cos((xzStep * count + (xzStep / 2)) - angle) * r;
		}
		if (node->getChildNodes()[i]->getnChildNodes() != 0) {
			r = r * 2;
		}
		buildTreeGraphicalBalloon(node->getChildNodes()[i], driver, smgr, pos, K, P, r, yStep, defaultTextureFileName);
		count++;
	}
}

void rotate(float x, float z, float angle, float radius, float& nx, float& nz) {
	nx = (x * cos(angle) - z * sin(angle)) * radius;
	nz = (x * sin(angle) + z * cos(angle)) * radius;
};
void NodeTree::buildTreeGraphicalH(Node * node, irr::video::IVideoDriver* driver, scene::ISceneManager* smgr, core::vector3df& position, float radius, float yHeight, const irr::io::path& defaultTextureFileName)
{
	unsigned int depth = getDepth(node);


	float y = yHeight * getDepth(node) * (-1);

	if (!node->getISceneNode()) {
		node->init(smgr, position.X, y, position.Z, driver, Functionality::generatePlainColorTexture(video::SColor(255, 22, 146, 255), driver));
	}
	else {
		node->getISceneNode()->setPosition(position);
	}

	int n = node->getnChildNodes();

	for (int i = 0; i < n; i++) {
		float centerAdjust = 0;
		if (node->getParent() != 0) {
			centerAdjust = (-node->getAngleRange() + node->getAngleRange() / n) / 2;
		}

		node->getChildNodes()[i]->setAngle(node->getAngle() + node->getAngleRange() / n * i + centerAdjust);
		node->getChildNodes()[i]->setAngleRange(node->getAngleRange() / n);

		float x, z;

		rotate(40 * (float)getDepth(node->getChildNodes()[i]), 0, node->getChildNodes()[i]->getAngle(), (float)getDepth(node->getChildNodes()[i]), x, z);
		//rotate(40 , 0, node->getChildNodes()[i]->getAngle(), getDepth(node->getChildNodes()[i]), x, z);
		core::vector3df pos(x, 0, z);

		buildTreeGraphicalH(node->getChildNodes()[i], driver, smgr, pos, 0, yHeight, "");

	}


	//STAR?
	/*for (unsigned int i = 0; i < n; i++)
	{
	float centerAdjust = 0;
	if (root->getParent() != nullptr) {
	centerAdjust = (-node->getAngleRange() + node->getAngleRange() / n) / 2;
	}
	node->getChildNodes()[i]->setAngle(node->getAngle() + node->getAngleRange() / n * i + centerAdjust);
	root->getChildNodes()[i]->setAngleRange(node->getAngleRange() / node->getnChildNodes());

	float x, y;
	rotate(40 * depth, 0, node->getChildNodes()[i]->getAngle(), x, y);
	pos.X = 300 + x;
	pos.Y = 300 + y;

	buildTreeGraphicalH(node->getChildNodes()[i], driver, smgr, pos, K, P, radius, yHeight, defaultTextureFileName);
	}*/
}

int countMaxNodesOnHemisphere(const float r, const float theta, const float nodeRadius)
{
	int maxNodes = 0;
	float pPhi = 0.f;
	float pTheta = theta;

	float layerR = 0.f;
	float layerCirc = 0.f;
	float angleB = 0.f;
	float angleA = 0.f;

	float quart = PI - pTheta;
	float quartLen = quart * r;
	float maxLayers = floor(quartLen / nodeRadius);
	float thetaIncr = quart / maxLayers;

	int count = 0;

	for (int i = 0; i <= maxLayers; i++)
	{
		angleB = (PI - pTheta);
		angleA = PI - ((PI / 2) + angleB);
		layerR = cos(angleA) * r;
		layerCirc = 2 * PI * layerR;
		maxNodes = (int)abs(floor(layerCirc / 20));
		float phiIncr = 2 * PI / maxNodes;
		pTheta += thetaIncr;
		count += maxNodes;

	}
	return count;
}

void NodeTree::buildTreeGraphicalHemisphere(Node * node, irr::video::IVideoDriver* driver, scene::ISceneManager* smgr, core::vector3df& origin, core::vector3df& position)
{
	//std::this_thread::sleep_for(30ms);
	node->r = 21;
	node->theta = PI / 2 + (13.f * PI / 180);
	vector3df pos = position;
	while (node->getnChildNodes() > countMaxNodesOnHemisphere(node->r, node->theta, 20.f)) {
		node->r++;
	}

	if (!node->getISceneNode()) {
		node->init(smgr, pos.X, pos.Y, pos.Z, driver, Functionality::generatePlainColorTexture(video::SColor(255, 22, 146, 255), driver));
	}
	else {
		node->getISceneNode()->setPosition(position);
	}

	int newLayer = 0;
	float layerR = 0.f;
	float layerCirc = 0.f;
	float angleB = 0.f;
	float angleA = 0.f;
	int maxNodes = 0;

	float quart = PI - node->theta;
	float quartLen = quart * node->r;
	float maxLayers = floor(quartLen / 20);
	float thetaIncr = quart / maxLayers;

	for (int i = 0, j = 0; i < node->getnChildNodes(); i++, j++)
	{
		float radius = node->r;
		angleB = (PI - node->theta);
		angleA = PI - ((PI / 2) + angleB);
		layerR = cos(angleA) * node->r;
		layerCirc = 2 * PI * layerR;
		maxNodes = (int)abs(floor(layerCirc / 20));
		float phiIncr = 0.f;
		if (node->getnChildNodes() < maxNodes)
		{
			phiIncr = 2 * PI / node->getnChildNodes();
		}
		else
		{
			phiIncr = 2 * PI / maxNodes;
		}

		//float phiIncr = 2 * PI / maxNodes;
		node->phi = phiIncr * j;
		if (j >= maxNodes)
		{
			node->theta += thetaIncr;
			newLayer++;
			j = 0;
		}

		if (node->getChildNodes()[i]->getnChildNodes() != 0)
		{
			if (node->getChildNodes()[i]->getnChildNodes() < 9)
			{

				radius = radius * ((rand() % 2) + 3);

			}
			else
			{
				radius = radius * ((rand() % 11) + 5);
			}


		}

		float angle = PI - node->theta;

		pos.X = (radius * sin(node->theta) * cos(node->phi)) + origin.X;
		pos.Z = (radius * sin(node->theta) * sin(node->phi)) + origin.Z;
		pos.Y = (radius * cos(node->theta)) + origin.Y;


		buildTreeGraphicalHemisphere(node->getChildNodes()[i], driver, smgr, pos, pos);

		if (node->getChildNodes()[i]->getnChildNodes() == 0)
		{
			node->getChildNodes()[i]->getISceneNode()->setScale(vector3df(0.5f));
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


int NodeTree::getCurrentSize()
{
	return getnNodes(root);
}
int NodeTree::getActualSize()
{
	actualSize = nodeArraySize + tempArraySize;
	return actualSize;
}


void NodeTree::render(Node * node, irr::video::IVideoDriver* driver)
{
	for (int i = 0; i < node->getnChildNodes(); i++) {
		node->getChildNodes()[i]->render(driver);

		render(node->getChildNodes()[i], driver);
	}
}
