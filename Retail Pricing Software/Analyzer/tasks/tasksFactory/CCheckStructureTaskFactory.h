/*
 * CCheckStructureTaskFactory.h
 *
 *  Created on: Feb 26, 2019
 *      Author: kraychik
 */

#ifndef TASKS_TASKSFACTORY_CCHECKSTRUCTURETASKFACTORY_H_
#define TASKS_TASKSFACTORY_CCHECKSTRUCTURETASKFACTORY_H_

#include <memory>
#include "CTasksFactory.h"

class CCheckStructureTaskFactory: public CTasksFactory {
public:
	CCheckStructureTaskFactory();
	virtual std::shared_ptr<const CBaseTask> CreateTask(const std::string& modeRequest, int projectId, const std::map<std::string, std::string>& params);
	virtual ~CCheckStructureTaskFactory();
};

#endif /* TASKS_TASKSFACTORY_CCHECKSTRUCTURETASKFACTORY_H_ */
