/*
 * CSupplementTask.cpp
 *
 *  Created on: Feb 6, 2019
 *      Author: kraychik
 */

#include "CSupplementTask.h"

/* CSupplementTask::CSupplementTask() {
	// TODO Auto-generated constructor stub
} */

CSupplementTask::CSupplementTask(int projectId, int calcId, const CFilter& filter, const std::vector<int>& targetAssetIds, bool useSupplementTargetAssets, ResponseFormat responseFormat)
	: CReportTask(projectId, calcId, filter) {
	this->targetAssetIds = targetAssetIds;
	this->responseFormat = responseFormat;
	this->useSupplementTargetAssets = useSupplementTargetAssets;
}

std::string CSupplementTask::ToString() const {
	std::string res = "CDynamicsTask. Project: " + std::to_string(this->projectId) + "; ";
	res += "CalcId: " + std::to_string(this->calcId) + "\n";
	for (const int assetId : targetAssetIds) {
		res += std::to_string(assetId) + ", ";
	}
	res += std::string("\n");
	res += filter.ToString();
	return res;
}

CSupplementTask::~CSupplementTask() {
	// TODO Auto-generated destructor stub
}

int CSupplementTask::TypeId() const {
	return 6;
}
