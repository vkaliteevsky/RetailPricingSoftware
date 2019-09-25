/*
 * CGetResultTaskFactory.h
 *
 *  Created on: Feb 26, 2019
 *      Author: kraychik
 */

#ifndef TASKS_TASKSFACTORY_CGETRESULTTASKFACTORY_H_
#define TASKS_TASKSFACTORY_CGETRESULTTASKFACTORY_H_

#include <memory>
#include "CTasksFactory.h"

class CGetResultTaskFactory: public CTasksFactory {
public:
	CGetResultTaskFactory();
	virtual std::shared_ptr<const CBaseTask> CreateTask(const std::string& modeRequest, int projectId, const std::map<std::string, std::string>& params);
	virtual ~CGetResultTaskFactory();
};

#endif /* TASKS_TASKSFACTORY_CGETRESULTTASKFACTORY_H_ */
