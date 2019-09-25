/*
 * CCheckStructureTaskFactory.cpp
 *
 *  Created on: Feb 26, 2019
 *      Author: kraychik
 */

#include "CCheckStructureTaskFactory.h"

CCheckStructureTaskFactory::CCheckStructureTaskFactory() {
}

std::shared_ptr<const CBaseTask> CCheckStructureTaskFactory::CreateTask(const std::string& modeRequest, int projectId, const std::map<std::string, std::string>& params) {
	CFilter filter;
	ParseFilter(params, filter);
	const int calcId = GetNewCalcId();
	auto resultingPtr = std::make_shared<const CCheckStructureTask>(projectId, calcId, filter);
	return std::static_pointer_cast<const CBaseTask>(resultingPtr);
}

CCheckStructureTaskFactory::~CCheckStructureTaskFactory() {
}

