/*
 * CDynamicsClass.h
 *
 *  Created on: Jan 19, 2019
 *      Author: kraychik
 */
#pragma once
#ifndef CDYNAMICSCLASS_H_
#define CDYNAMICSCLASS_H_

#include "CReportTask.h"
#include <vector>
#include <set>
#include <list>
#include <map>
#include "../structures/CIntervalSeries.h"
#include "../structures/Structures.h"

class CDynamicsTask: public CReportTask {
public:
	//CDynamicsTask();
	//CDynamicsTask(int projectId, uint calcId, const std::map<std::string, std::string> & params);
	CDynamicsTask(int projectId, int calcId, const CFilter & filter, DynamicsTaskType taskType);
	virtual ~CDynamicsTask();
	virtual std::string ToString() const;
	virtual int TypeId() const override;
	DynamicsTaskType dynamicsTaskType = DynamicsTaskType::IncomeDynamics;
};

#endif /* CDYNAMICSCLASS_H_ */
