/*
 * CGetResultTask.cpp
 *
 *  Created on: Jan 26, 2019
 *      Author: kraychik
 */

#include "CGetResultTask.h"

/* CGetResultTask::CGetResultTask() : CBaseTask() {
} */

CGetResultTask::~CGetResultTask() {
	// TODO Auto-generated destructor stub
}

std::string CGetResultTask::ToString() const {
	std::string result = "GetResultTask (";
	result += "projectId: " + std::to_string(projectId) + ", ";
	result += "calcId: " + std::to_string(calcId) + ")\n";
	return result;
}

CGetResultTask::CGetResultTask(int projectId, int calcId) : CBaseTask(projectId, calcId) {
}

int CGetResultTask::TypeId() const {
	return 2;
}
