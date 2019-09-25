/*
 * CPriceOptimizationTaskFactory.h
 *
 *  Created on: Jun 19, 2019
 *      Author: kraychik
 */

#ifndef TASKS_TASKSFACTORY_CPRICEOPTIMIZATIONTASKFACTORY_H_
#define TASKS_TASKSFACTORY_CPRICEOPTIMIZATIONTASKFACTORY_H_

#include "CTasksFactory.h"
#include <memory>

class CPriceOptimizationTaskFactory: public CTasksFactory {
public:
	CPriceOptimizationTaskFactory();
	virtual std::shared_ptr<const CBaseTask> CreateTask(const std::string& modeRequest, int projectId, const std::map<std::string, std::string>& params);
	virtual ~CPriceOptimizationTaskFactory();
};

#endif /* TASKS_TASKSFACTORY_CPRICEOPTIMIZATIONTASKFACTORY_H_ */
