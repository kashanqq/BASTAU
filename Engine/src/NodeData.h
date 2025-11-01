#pragma once
#include <string>

class Subordinate {
public:
	unsigned int subID;
	std::string name;

};
class Report {
public:
	int reportID;
	std::string content;
};

class NodeData {
public:
	unsigned int unitID;
	std::string unitName;
	std::string headOfUnit;
	std::string headOrganization;
	Subordinate* units = nullptr;

	//*Number of staff
	unsigned int numStaff;
	float annualBudget;
	float expenses;
	float income;
	float loss;

	//*Description of the unit activity
	std::string activityDesc;
	std::string keyAchievements;
	Report* annualReports = nullptr;
	NodeData();
};