/*
 * CTopAssetsTaskFactory.h
 *
 *  Created on: Feb 26, 2019
 *      Author: kraychik
 */

#ifndef TASKS_TASKSFACTORY_CTOPASSETSTASKFACTORY_H_
#define TASKS_TASKSFACTORY_CTOPASSETSTASKFACTORY_H_

#include "CTasksFactory.h"
#include "../CTopAssetsTask.h"
#include <memory>

class CTopAssetsTaskFactory: public CTasksFactory {
public:
	CTopAssetsTaskFactory();
	virtual std::shared_ptr<const CBaseTask> CreateTask(const std::string& modeRequest, int projectId, const std::map<std::string, std::string>& params) override;
	virtual ~CTopAssetsTaskFactory();
};

#endif /* TASKS_TASKSFACTORY_CTOPASSETSTASKFACTORY_H_ */
