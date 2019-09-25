/*
 * CReportTask.cpp
 *
 *  Created on: Jan 30, 2019
 *      Author: kraychik
 */

#include "CReportTask.h"

/* CReportTask::CReportTask() {
	// TODO Auto-generated constructor stub
} */

CReportTask::CReportTask(int projectId, int calcId, const CFilter & filter) : CBaseTask(projectId, calcId) {
	/* filter.intervals = intervals;
	filter.shopIds = shopIds;
	filter.assetIds = assetIds;
	filter.assetIdsFromGroups = groupIds;
	filter.useSupplementGroups = toExcludeGroups;
	filter.useSupplementShops = toExcludeShops;
	filter.useSupplementAssets = toExcludeAssets;
	filter.useSupplementIntervals = toExcludeIntervals; */
	this->filter = filter;
}

CReportTask::~CReportTask() {
	// TODO Auto-generated destructor stub
}

std::string CReportTask::ToString() const {
	std::string res = "CReportTask. Project: " + std::to_string(this->projectId) + "; ";
	res += "CalcId: " + std::to_string(this->calcId) + "/n";
	/*res += "\nInteval Series:\n";
	for (auto interval : intervals) {
		res += "[" + std::to_string(interval.first) + ", " + std::to_string(interval.second) + "]\n";
	}
	res += "Shops (toExclude = " + std::to_string(toExcludeShops) + "): ";
	for (auto shopId : shopIds) {
		res += std::to_string(shopId) + ", ";
	}
	res += "\nAssets (toExclude = " + std::to_string(toExcludeAssets) + "): ";
	for (auto asset : assetIds) {
		res += std::to_string(asset) + ", ";
	}
	res += "\nGroups (toExclude = " + std::to_string(toExcludeGroups) + "): ";
	for (auto group : groupIds) {
		res += std::to_string(group) + ", ";
	}*/
	res += filter.ToString();
	return res;
}

int CReportTask::TypeId() const {
	return 3;
}
