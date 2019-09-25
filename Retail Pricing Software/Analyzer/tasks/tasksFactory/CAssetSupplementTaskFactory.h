/*
 * CAssetSupplementFactory.h
 *
 *  Created on: Feb 26, 2019
 *      Author: kraychik
 */

#ifndef TASKS_TASKSFACTORY_CASSETSUPPLEMENTTASKFACTORY_H_
#define TASKS_TASKSFACTORY_CASSETSUPPLEMENTTASKFACTORY_H_

#include <memory>
#include "CTasksFactory.h"

class CAssetSupplementTaskFactory: public CTasksFactory {
public:
	CAssetSupplementTaskFactory();
	virtual std::shared_ptr<const CBaseTask> CreateTask(const std::string& modeRequest, int projectId, const std::map<std::string, std::string>& params);
	virtual ~CAssetSupplementTaskFactory();
};

#endif /* TASKS_TASKSFACTORY_CASSETSUPPLEMENTTASKFACTORY_H_ */
