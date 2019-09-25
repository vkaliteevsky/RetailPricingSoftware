/*
 * CGetResultTask.h
 *
 *  Created on: Jan 26, 2019
 *      Author: kraychik
 */
#pragma once
#ifndef TASKS_CGETRESULTTASK_H_
#define TASKS_CGETRESULTTASK_H_

#include "CBaseTask.h"
#include <string>

class CGetResultTask: public CBaseTask {
public:
	//CGetResultTask();
	CGetResultTask(int projectId, int calcId);
	virtual ~CGetResultTask();
	virtual std::string ToString() const;
	virtual int TypeId() const override;
};

#endif /* TASKS_CGETRESULTTASK_H_ */
