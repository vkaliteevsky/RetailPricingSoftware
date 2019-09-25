/*
 * CDynamicsTaskFactory.cpp
 *
 *  Created on: Feb 26, 2019
 *      Author: kraychik
 */

#include "CDynamicsTaskFactory.h"

CDynamicsTaskFactory::CDynamicsTaskFactory() {
}

CDynamicsTaskFactory::~CDynamicsTaskFactory() {
}

std::shared_ptr<const CBaseTask> CDynamicsTaskFactory::CreateTask(const std::string& modeRequest, int projectId, const std::map<std::string, std::string>& params) {
	const int calcId = GetNewCalcId();
	CFilter filter;
	ParseFilter(params, filter);
	DynamicsTaskType dynamicsTaskType = DynamicsTaskType::IncomeDynamics;
	std::string typeString("");
	ParseParam(params, "type", typeString);
	dynamicsTaskType = CTypeConverter::ConvertDynamicsType(typeString);
	auto resultingPtr = std::make_shared<const CDynamicsTask>(projectId, calcId, filter, dynamicsTaskType);
	return std::static_pointer_cast<const CBaseTask>(resultingPtr);
}
