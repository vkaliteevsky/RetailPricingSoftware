/*
 * CDynamicsClass.cpp
 *
 *  Created on: Jan 19, 2019
 *      Author: kraychik
 */

#include "CDynamicsTask.h"

/*CDynamicsTask::CDynamicsTask() : CBaseTask() {
	// TODO Auto-generated constructor stub
}*/

CDynamicsTask::~CDynamicsTask() {
	// TODO Auto-generated destructor stub
}

std::string CDynamicsTask::ToString() const {
	std::string res = "CDynamicsTask. Project: " + std::to_string(this->projectId) + "; ";
	res += "CalcId: " + std::to_string(this->calcId) + "\n";
	res += filter.ToString();
	return res;
}

CDynamicsTask::CDynamicsTask(int projectId, int calcId, const CFilter & filter, DynamicsTaskType taskType)
		: CReportTask(projectId, calcId, filter) {
	this->dynamicsTaskType = taskType;
}

int CDynamicsTask::TypeId() const {
	return 4;
}
