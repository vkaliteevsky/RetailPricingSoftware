/*
 * CGetResultTaskFactory.cpp
 *
 *  Created on: Feb 26, 2019
 *      Author: kraychik
 */

#include "CGetResultTaskFactory.h"

CGetResultTaskFactory::CGetResultTaskFactory() {
	// TODO Auto-generated constructor stub

}

std::shared_ptr<const CBaseTask> CGetResultTaskFactory::CreateTask(const std::string& modeRequest, int projectId, const std::map<std::string, std::string>& params) {
	int calcId = 0;
	ParseParam(params, "calcid", calcId);
	auto getResultTaskPtr = std::make_shared<const CGetResultTask>(projectId, calcId);
	return std::static_pointer_cast<const CBaseTask>(getResultTaskPtr);
}

CGetResultTaskFactory::~CGetResultTaskFactory() {
	// TODO Auto-generated destructor stub
}

