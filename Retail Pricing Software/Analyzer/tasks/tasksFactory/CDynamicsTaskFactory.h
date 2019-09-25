/*
 * CDynamicsTaskFactory.h
 *
 *  Created on: Feb 26, 2019
 *      Author: kraychik
 */

#ifndef TASKS_TASKSFACTORY_CDYNAMICSTASKFACTORY_H_
#define TASKS_TASKSFACTORY_CDYNAMICSTASKFACTORY_H_

#include "CTasksFactory.h"

class CDynamicsTaskFactory: public CTasksFactory {
public:
	CDynamicsTaskFactory();
	virtual std::shared_ptr<const CBaseTask> CreateTask(const std::string& modeRequest, int projectId, const std::map<std::string, std::string>& params);
	virtual ~CDynamicsTaskFactory();
};

#endif /* TASKS_TASKSFACTORY_CDYNAMICSTASKFACTORY_H_ */
