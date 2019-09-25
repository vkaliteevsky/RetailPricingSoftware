/*
 * CAssetSupplementFactory.cpp
 *
 *  Created on: Feb 26, 2019
 *      Author: kraychik
 */

#include "CAssetSupplementTaskFactory.h"

CAssetSupplementTaskFactory::CAssetSupplementTaskFactory() {
}

std::shared_ptr<const CBaseTask> CAssetSupplementTaskFactory::CreateTask(const std::string& modeRequest, int projectId, const std::map<std::string, std::string>& params) {
	const int calcId = GetNewCalcId();
	CFilter filter;
	ParseFilter(params, filter);
	bool useSupplementAssets = true;
	std::vector<int> targetAssets;
	std::string responseFormatStr("");
	ResponseFormat responseFormat = ResponseFormat::ArrayOfArrays;
	ParseParam(params, "targetassets", targetAssets, ',');
	ParseParam(params, "usesupplementtargetassets", useSupplementAssets);
	ParseParam(params, "responseformat", responseFormatStr);
	responseFormat = CTypeConverter::ConvertResponseFormat(responseFormatStr);
	if (responseFormat == ResponseFormat::NotImplemented) {
		responseFormat = ResponseFormat::ArrayOfArrays;
	}
	auto resultingPtr = std::make_shared<const CSupplementTask>(projectId, calcId, filter, targetAssets, useSupplementAssets, responseFormat);
	return std::static_pointer_cast<const CBaseTask>(resultingPtr);
}

CAssetSupplementTaskFactory::~CAssetSupplementTaskFactory() {
}

