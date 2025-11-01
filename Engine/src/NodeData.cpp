#include "NodeData.h"

NodeData::NodeData()
{
	unitID = 0;
	unitName = "";
	headOfUnit = "";
	headOrganization = "";
	units = nullptr;

	numStaff = 0;
	annualBudget = 0.f;
	expenses = 0.f;
	income = 0.f;
	loss = 0.f;

	activityDesc = "";
	keyAchievements = "";
	annualReports = nullptr;
}
