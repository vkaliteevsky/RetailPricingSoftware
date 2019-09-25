/*
 * CSupplementReportExecutor.cpp
 *
 *  Created on: Feb 25, 2019
 *      Author: kraychik
 */

#include "CSupplementReportExecutor.h"


std::shared_ptr<const CCalcResult> CSupplementReportExecutor::ExecuteTask(std::shared_ptr<const CBaseTask> baseTask) {
	auto task = static_pointer_cast<const CSupplementTask>(baseTask);
	//auto projectInfo = GetProjectInfo(task->projectId);
	auto projectInfo = CDataManager::GetInstance()->GetOrLoadProjectInfo(task->projectId);
	const auto salesHistory = projectInfo->GetSalesHistory();
	const auto prs = projectInfo->GetOrLoadOptimizerParams();
	const auto groupsInfoPtr = prs->CalcGroupsInfo();
	CFilterAdvanced aFilter(task->filter, groupsInfoPtr);
	auto supplementMatrix = salesHistory->BuildSupplementMatrix(aFilter);
	std::list<std::shared_ptr<CStructureMap<int, double>>> listOfStructures;
	std::vector<int> targetAssetIds;
	if (task->useSupplementTargetAssets) {
		std::set<int> allAssetsSet = salesHistory->GetAllAssets();
		for (const int targetAsset : task->targetAssetIds) {
			allAssetsSet.erase(targetAsset);
		}
		targetAssetIds.reserve(allAssetsSet.size());
		for (const int assetId : allAssetsSet) {
			targetAssetIds.push_back(assetId);
		}
	} else {
		targetAssetIds = task->targetAssetIds;
	}
	supplementMatrix->SliceAndBuildStructureMap(targetAssetIds, listOfStructures);
	std::vector<std::string> strStructures;
	std::string resultingString;
	if (task->responseFormat == ResponseFormat::ArrayOfStructures) {
		for (const auto & mapStructure : listOfStructures) {
			//std::cout << mapStructure->ToString() << std::endl;
			std::string jsonString = mapStructure->ToJSON(std::string("assetId"), std::string("value"));
			strStructures.push_back(jsonString);
		}
		for (uint i = 0; i < strStructures.size(); ++i) {
			int targetAssetId = targetAssetIds[i];
			//std::string keyStr = CJSONCreator::CreateKeyValueItem("assetId", targetAssetId);
			//std::string valueStr = CJSONCreator::CreateKeyValueItem("structure", strStructures[i]);
			strStructures[i] = CJSONCreator::CreateKeyValueItem(std::to_string(targetAssetId), strStructures[i]);
			strStructures[i] = CJSONCreator::MakeObject(strStructures[i]);
		}
		resultingString = CJSONCreator::SeparateObjects(strStructures);
		resultingString = CJSONCreator::MakeObject(resultingString);
	} else if (task->responseFormat == ResponseFormat::ArrayOfArrays) {
		std::string usedAssets = CJSONCreator::MakeArray(supplementMatrix->GetAssetIds());
		usedAssets = CJSONCreator::CreateKeyValueItem("assetIds", usedAssets);
		strStructures.push_back(usedAssets);
		for (const int targetAssetId : targetAssetIds) {
			std::string jsonArray = supplementMatrix->ToJSONArray(targetAssetId);
			std::string res = CJSONCreator::CreateKeyValueItem(std::to_string(targetAssetId), jsonArray);
			strStructures.push_back(res);
		}
		resultingString = CJSONCreator::SeparateObjects(strStructures);
		resultingString = CJSONCreator::MakeObject(resultingString);
	} else {
		throw CSmartException("CProcessing::ExecuteSupplementReport: This case for task->responseFormat not implemented yet");
	}
	return std::make_shared<CCalcResult>(task->projectId, task->calcId, resultingString);

}

CSupplementReportExecutor::CSupplementReportExecutor() {
}

CSupplementReportExecutor::~CSupplementReportExecutor() {
	// TODO Auto-generated destructor stub
}

