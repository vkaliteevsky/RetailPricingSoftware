/*
 * CTopAssetsExecutor.cpp
 *
 *  Created on: Feb 25, 2019
 *      Author: kraychik
 */

#include "CTopAssetsExecutor.h"

CTopAssetsExecutor::CTopAssetsExecutor() {

}

std::shared_ptr<const CCalcResult> CTopAssetsExecutor::ExecuteTask(std::shared_ptr<const CBaseTask> baseTask) {
	auto task = static_pointer_cast<const CTopAssetsTask>(baseTask);
	auto projectInfo = GetProjectInfo(task->projectId);
	const auto salesHistory = projectInfo->GetSalesHistory();
	const auto assetsSummary = salesHistory->GetAssetsSummary(task->filter);
	const auto resultsVect = assetsSummary->GetTopItems(task->amount, task->kpi, true);
	std::vector<std::string> objectsStr;
	objectsStr.reserve(resultsVect.size());
	for (const auto & itemPtr : resultsVect) {
		objectsStr.push_back(itemPtr->ToJSON());
	}
	const std::string resultingString = CJSONCreator::MakeArray(objectsStr);
	return std::make_shared<const CCalcResult>(task->projectId, task->calcId, resultingString);
}

CTopAssetsExecutor::~CTopAssetsExecutor() {
	// TODO Auto-generated destructor stub
}

