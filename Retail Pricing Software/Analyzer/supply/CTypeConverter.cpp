/*
 * CTypeConverter.cpp
 *
 *  Created on: Feb 21, 2019
 *      Author: kraychik
 */

#include "CTypeConverter.h"

KPI CTypeConverter::ConvertKPI(const std::string& kpiStr) {
	if (!kpiStr.compare("income")) return KPI::Income;
	else if (!kpiStr.compare("volume")) return KPI::Volume;
	else if (!kpiStr.compare("gain")) return KPI::Gain;
	else {
		//throw CSmartException("CTypeConverter::ConvertKPI: not implemented yet\n");
		return KPI::NotImplemented;
	}
}

std::string CTypeConverter::ConvertKPI(KPI kpi) {
	if (kpi == KPI::Income) return std::string("Income");
	else if (kpi == KPI::Gain) return std::string("Gain");
	else if (kpi == KPI::Volume) return std::string("Volume");
	else {
		throw CSmartException("CTypeConverter::ConvertKPI: not implemented yet\n");
		//return KPI::NotImplemented;
	}
}

DynamicsTaskType CTypeConverter::ConvertDynamicsType(const std::string& dynamicsType) {
	if (!dynamicsType.compare("incomedynamics")) {
		return DynamicsTaskType::IncomeDynamics;
	} else if (!dynamicsType.compare("avgcheckdynamics")) {
		return DynamicsTaskType::AvgCheckDynamics;
	} else {
		//throw CSmartException("CTypeConverter::ConvertDynamicsType: not implement yet\n");
		return DynamicsTaskType::NotImplemented;
	}
}

std::string CTypeConverter::ConvertDynamicsType(DynamicsTaskType dynamicsTaskType) {
	if (dynamicsTaskType == DynamicsTaskType::IncomeDynamics) {
		return std::string("IncomeDynamics");
	} else if (dynamicsTaskType == DynamicsTaskType::AvgCheckDynamics) {
		return std::string("AvgCheckDynamics");
	} else {
		throw CSmartException("CTypeConverter::ConvertDynamicsType: not implemented yet\n");
	}
}

FilterMode CTypeConverter::BoolToFilterMode(bool useSupplement) {
	return (useSupplement ? FilterMode::UseSupplementSet : FilterMode::Standard);
}

ResponseFormat CTypeConverter::ConvertResponseFormat(const std::string& responseFormat) {
	if (!responseFormat.compare("arrayofarrays")) return ResponseFormat::ArrayOfArrays;
	else if (!responseFormat.compare("arrayofstructures")) return ResponseFormat::ArrayOfStructures;
	else {
		return ResponseFormat::NotImplemented;
	}
}
