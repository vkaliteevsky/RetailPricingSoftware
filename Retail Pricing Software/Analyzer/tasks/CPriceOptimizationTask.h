/*
 * CPriceOptimizationTask.h
 *
 *  Created on: Jun 19, 2019
 *      Author: kraychik
 */

#ifndef TASKS_CPRICEOPTIMIZATIONTASK_H_
#define TASKS_CPRICEOPTIMIZATIONTASK_H_

#include "CBaseTask.h"

class CPriceOptimizationTask: public CBaseTask {
public:
	CPriceOptimizationTask(int projectId, int calcId, double monthSince, int periodDays, int amountOfDenseAssets);
	virtual ~CPriceOptimizationTask();
	virtual int TypeId() const;
	double monthSince;
	int periodDays;
	int amountOfDenseAssets;
};

#endif /* TASKS_CPRICEOPTIMIZATIONTASK_H_ */
