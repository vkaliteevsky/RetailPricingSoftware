/*
 * CCheckStructureTask.cpp
 *
 *  Created on: Jan 30, 2019
 *      Author: kraychik
 */

#include "CCheckStructureTask.h"

/* CCheckStructureTask::CCheckStructureTask() {
	// TODO Auto-generated constructor stub

} */

CCheckStructureTask::~CCheckStructureTask() {
	// TODO Auto-generated destructor stub
}

std::string CCheckStructureTask::ToString() const {
	std::string res = "CCheckStructureTask. Project: " + std::to_string(this->projectId) + "; ";
	res += "CalcId: " + std::to_string(this->calcId) + "\n";
	res += filter.ToString();
	return res;
}

CCheckStructureTask::CCheckStructureTask(int projectId, int calcId, const CFilter & filter)
		: CReportTask(projectId, calcId, filter)
{
}

int CCheckStructureTask::TypeId() const {
	return 5;
}
