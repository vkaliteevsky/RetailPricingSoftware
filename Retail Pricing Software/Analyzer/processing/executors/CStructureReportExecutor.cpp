/*
 * CStructureReportExecutor.cpp
 *
 *  Created on: Feb 25, 2019
 *      Author: kraychik
 */

#include "CStructureReportExecutor.h"

std::shared_ptr<const CCalcResult> CStructureReportExecutor::ExecuteTask(std::shared_ptr<const CBaseTask> baseTask) {
	auto task = static_pointer_cast<const CCheckStructureTask>(baseTask);
	auto projectInfo = GetProjectInfo(task->projectId);
	//FilterMode groupFilterMode = (task->filter.useSupplementGroups) ? FilterMode::UseSupplementSet : FilterMode::Standard;
	//FilterMode assetFilterMode = (task->filter.useSupplementAssets) ? FilterMode::UseSupplementSet : FilterMode::Standard;
	//FilterMode shopFilterMode = (task->filter.useSupplementShops) ? FilterMode::UseSupplementSet : FilterMode::Standard;
	CIntervalSeries intervals(task->filter.intervals);

	if (projectInfo == nullptr) {
		throw CSmartException("CStructureReportExecutor::ExecuteTask: bad read of projectInfo!\n" + task->ToString());
	}
	const auto salesHistory = projectInfo->GetSalesHistory();

	std::map<int, double> resultMap;
	int amountOfChecks = 0;
	CStructureMap<int, double> structureMap;
	salesHistory->CalculateCheckStructure(task->filter, structureMap, amountOfChecks);

	string resultString = MakeResultStringForStructure(structureMap.structure, amountOfChecks);
	return std::make_shared<const CCalcResult>(task->projectId, task->calcId, resultString);
}

CStructureReportExecutor::CStructureReportExecutor() {
}

CStructureReportExecutor::~CStructureReportExecutor() {
	// TODO Auto-generated destructor stub
}

