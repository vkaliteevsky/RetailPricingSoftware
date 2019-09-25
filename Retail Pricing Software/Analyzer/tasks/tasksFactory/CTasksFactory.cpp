/*
 * CTasksFactory.cpp
 *
 *  Created on: Feb 26, 2019
 *      Author: kraychik
 */

#include "CTasksFactory.h"

CTasksFactory::CTasksFactory() {
}

CTasksFactory::~CTasksFactory() {
}

void CTasksFactory::SplitString(std::string str, char delim, std::list<std::string> & resultingContainer) const {
	if (str.size() <= 0) return;
	const char *s = str.c_str();
	int startNum = 0, endNum = 0;
	while (*s) {
		if (*s == delim) {
			std::string appendStr = str.substr(startNum, endNum - startNum + 1);
			resultingContainer.push_back(appendStr);
			startNum = endNum + 1;
			endNum = startNum;
		}
		else if (*s == ' ') {
			// все пробелы пропускаем, ничего не делаем
		} else {
			// нужно накопить текущий символ *s
			endNum++;
		}
		s++;
	}
	std::string appendStr = str.substr(startNum, endNum - startNum + 1);
	resultingContainer.push_back(appendStr);
}

template <typename T>
void CTasksFactory::SplitStringToNumericVector(const std::string& str, char delim, std::vector<T>& resultingContainer) const {
	std::list<std::string> values;
	SplitString(str, delim, values);  // заполняет список periodStarts
	//resultingContainer.resize(values.size());
	T addValue;
	for (const auto val : values) {
		if (std::is_same<T, long int>::value) addValue = std::stol(val.c_str());
		else if (std::is_same<T, int>::value) addValue = std::atoi(val.c_str());
		else if (std::is_same<T, double>::value) addValue = std::atof(val.c_str());
		else addValue = atoi(val.c_str());
		resultingContainer.push_back(addValue);
	}
}

template <typename T>
void CTasksFactory::SplitStringToNumericSet(const std::string& str, char delim, std::set<T>& resultingContainer) const {
	std::vector<T> values;
	SplitStringToNumericVector<T>(str, delim, values);
	for (const auto v : values) {
		resultingContainer.insert(v);
	}
}

std::shared_ptr<const CBaseTask> CTasksFactory::CreateTask(const std::string& modeRequest, int projectId, const std::map<std::string, std::string>& params) {
	std::shared_ptr<const CBaseTask> newTask = nullptr;
	std::unique_ptr<CTasksFactory> factory = nullptr;
	if (!modeRequest.compare("result")) {
		factory = std::make_unique<CGetResultTaskFactory>();
	} else if (!modeRequest.compare("report")) {
		std::string typeString("");
		ParseParam(params, "type", typeString);
		if ((!typeString.compare("incomedynamics")) || (!typeString.compare("avgcheckdynamics"))) {
			factory = std::make_unique<CDynamicsTaskFactory>();
		} else if (!typeString.compare("checkstructure")) {
			factory = std::make_unique<CCheckStructureTaskFactory>();
		} else if (!typeString.compare("assetsupplement")) {
			factory = std::make_unique<CAssetSupplementTaskFactory>();
		} else if (!typeString.compare("topreport")) {
			factory = std::make_unique<CTopAssetsTaskFactory>();
		} else if (!typeString.compare("priceoptimization")) {
			factory = std::make_unique<CPriceOptimizationTaskFactory>();
		} else {
			factory = nullptr;
		}
	} else {
		factory = nullptr;
	}
	if (factory != nullptr) {
		newTask = factory->CreateTask(modeRequest, projectId, params);
	}
	return newTask;
}

void CTasksFactory::ParsePeriods(const std::string & strPeriods, char delim, std::vector<interval> & resultingContainer) const {
	//std::list<std::string> periodStarts;
	std::vector<time_t> times;
	SplitStringToNumericVector<long int>(strPeriods, delim, times);  // заполняет список periodStarts
	// далее из массива times формируем вектор интервалов
	unsigned int i = 0, l = times.size();
	while (i < l - 1) {
		resultingContainer.push_back(interval(times[i], times[i+1]-1));
		i++;
	}
}

void CTasksFactory::ParseParam(const std::map<std::string, std::string> & params, const char* paramName, int & paramValue) const {
	paramValue = 0;
	auto it = params.find(paramName);
	if (it != params.end()) {
		paramValue = std::atoi(it->second.c_str());
	}
}

void CTasksFactory::ParseParam(const std::map<std::string, std::string>& params, const char* paramName, double& paramValue) const {
	paramValue = 0.0;
	auto it = params.find(paramName);
	if (it != params.end()) {
		paramValue = std::atof(it->second.c_str());
	}
}

void CTasksFactory::ParseParam(const std::map<std::string, std::string>& params, const char* paramName, std::string& paramValue) const {
	paramValue = "";
	auto it = params.find(paramName);
	if (it != params.end()) {
		paramValue = it->second;
	}
}

void CTasksFactory::ParseParam(const std::map<std::string, std::string>& params, const char* paramName, std::set<int>& paramValues, char delim) const {
	paramValues.clear();
	auto it = params.find(paramName);
	if (it != params.end()) {
		SplitStringToNumericSet<int>(it->second, delim, paramValues);
	}
}

void CTasksFactory::ParseParam(const std::map<std::string, std::string>& params, const char* paramName, std::vector<int>& paramValues, char delim) const {
	paramValues.clear();
	auto it = params.find(paramName);
	if (it != params.end()) {
		SplitStringToNumericVector<int>(it->second, delim, paramValues);
	}
}

void CTasksFactory::ParseParam(const std::map<std::string, std::string>& params, const char* paramName, bool& paramValue) const {
	auto it = params.find(paramName);
	if (it != params.end()) {
		paramValue = std::atoi(it->second.c_str());
	}
}

void CTasksFactory::ParseFilter(const std::map<std::string, std::string>& params, CFilter & filter) const {
	filter.Clear();
	auto itPeriods = params.find("periods");
	if (itPeriods != params.end()) {
		ParsePeriods(itPeriods->second, delim, filter.intervals);
	} else {
		itPeriods = params.find("intervals");
		if (itPeriods != params.end()) {
			ParsePeriods(itPeriods->second, delim, filter.intervals);
		}
	}
	ParseParam(params, "shops", filter.shopIds, ',');
	ParseParam(params, "assets", filter.assetIds, ',');
	ParseParam(params, "groups", filter.groupIds, ',');

	bool useSupplementAssets = true, useSupplementGroups = true;
	bool useSupplementShops = true, useSupplementIntervals = true;

	ParseParam(params, "usesupplementassets", useSupplementAssets);
	ParseParam(params, "usesupplementgroups", useSupplementGroups);
	ParseParam(params, "usesupplementshops", useSupplementShops);
	ParseParam(params, "usesupplementintervals", useSupplementIntervals);

	filter.useSupplementAssets = useSupplementAssets;
	filter.useSupplementGroups = useSupplementGroups;
	filter.useSupplementIntervals = useSupplementIntervals;
	filter.useSupplementShops = useSupplementShops;
}

int CTasksFactory::GetNewCalcId() const {
	auto calcIdManager = CCalcIdsManager::GetInstance();
	return calcIdManager->GenerateCalcId();
}
