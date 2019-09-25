/*
 * IExecutor.cpp
 *
 *  Created on: Feb 25, 2019
 *      Author: kraychik
 */

#include "IExecutor.h"

IExecutor::~IExecutor() {

}

std::shared_ptr<const CProjectInfo> IExecutor::GetProjectInfo(int projectId) const {
	CDataManager *dataManager = CDataManager::GetInstance();
	std::shared_ptr<const CProjectInfo> projectInfo = dataManager->GetOrLoadProjectInfo(projectId);
	if (projectInfo == nullptr) {
		throw CSmartException("IExecutor::GetProjectInfo: nullptr returned for projectId = " + std::to_string(projectId));
	}
	return projectInfo;
}

template <typename T>
std::string IExecutor::MakeResultStringFromVector(const vector<T>& vect) const {
	std::string res;
	auto it = vect.begin();
	res += std::to_string(*it);
	it++;
	while (it != vect.end()) {
		res += "," + std::to_string(*it);
		it++;
	}
	return res;
}

std::string IExecutor::MakeResultStringForStructure(const std::map<int, double> & mapVals, int amountOfChecks) const {
	string arrMap = CJSONCreator::MakeKeyValueArray<int, double>(mapVals, string("AssetId"), string("Amount"));
	string result = CJSONCreator::CreateKeyValueItem(string("structure"), arrMap);
	result = CJSONCreator::SeparateObjects(result, CJSONCreator::CreateKeyValueItem(string("AmountOfChecks"), amountOfChecks));
	result = CJSONCreator::MakeObject(result);
	return result;
}

IExecutor::IExecutor() {

}

template std::string IExecutor::MakeResultStringFromVector(const vector<double>& vect) const;

