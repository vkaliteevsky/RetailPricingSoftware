/*
 * CPriceOptimizationTaskFactory.cpp
 *
 *  Created on: Jun 19, 2019
 *      Author: kraychik
 */

#include "CPriceOptimizationTaskFactory.h"

CPriceOptimizationTaskFactory::CPriceOptimizationTaskFactory() {
	// TODO Auto-generated constructor stub

}

std::shared_ptr<const CBaseTask> CPriceOptimizationTaskFactory::CreateTask(const std::string& modeRequest, int projectId, const std::map<std::string, std::string>& params) {
	const int calcId = GetNewCalcId();

	double monthSince;
	ParseParam(params, "monthsince", monthSince);
	if (monthSince <= 0.0) monthSince = 1.5;

	int periodDays;
	ParseParam(params, "perioddays", periodDays);
	if (periodDays <= 0) periodDays = 1;

	int amountOfDenseAssets;
	ParseParam(params, "amountofdenseassets", amountOfDenseAssets);
	if (amountOfDenseAssets <= 0) amountOfDenseAssets = 0;

	auto resultingPtr = std::make_shared<const CPriceOptimizationTask>(projectId, calcId, monthSince, periodDays, amountOfDenseAssets);
	return std::static_pointer_cast<const CBaseTask>(resultingPtr);
}

CPriceOptimizationTaskFactory::~CPriceOptimizationTaskFactory() {

}

