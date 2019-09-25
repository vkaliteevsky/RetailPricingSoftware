/*
 * CTopAssetsTaskFactory.cpp
 *
 *  Created on: Feb 26, 2019
 *      Author: kraychik
 */

#include "CTopAssetsTaskFactory.h"

CTopAssetsTaskFactory::CTopAssetsTaskFactory() {

}

std::shared_ptr<const CBaseTask> CTopAssetsTaskFactory::CreateTask(const std::string& modeRequest, int projectId, const std::map<std::string, std::string>& params) {
	const int calcId = GetNewCalcId();
	CFilter filter;
	ParseFilter(params, filter);
	int amount = 0;
	std::string kpiString("");
	ParseParam(params, "amount", amount);
	ParseParam(params, "kpi", kpiString);
	KPI kpi = KPI::Income;
	if (!kpiString.empty()) {
		kpi = CTypeConverter::ConvertKPI(kpiString);
	}
	auto resultingPtr = std::make_shared<const CTopAssetsTask>(projectId, calcId, filter, amount, kpi);
	return std::static_pointer_cast<const CBaseTask>(resultingPtr);
}

CTopAssetsTaskFactory::~CTopAssetsTaskFactory() {

}

