/*
 * CPriceOptimizationTask.cpp
 *
 *  Created on: Jun 19, 2019
 *      Author: kraychik
 */

#include "CPriceOptimizationTask.h"

CPriceOptimizationTask::CPriceOptimizationTask(int projectId, int calcId, double monthSince, int periodDays, int amountOfDenseAssets)
	: CBaseTask(projectId, calcId), monthSince(monthSince), periodDays(periodDays), amountOfDenseAssets(amountOfDenseAssets) {
}

CPriceOptimizationTask::~CPriceOptimizationTask() {
}

int CPriceOptimizationTask::TypeId() const {
	return 8;
}
