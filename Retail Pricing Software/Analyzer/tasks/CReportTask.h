/*
 * CReportTask.h
 *
 *  Created on: Jan 30, 2019
 *      Author: kraychik
 */

#ifndef TASKS_CREPORTTASK_H_
#define TASKS_CREPORTTASK_H_

#include "CBaseTask.h"
#include "../structures/CFilter.h"
#include "../structures/CIntervalSeries.h"

class CReportTask: public CBaseTask {
public:
	//CReportTask();
	CReportTask(int projectId, int calcId, const CFilter & filter);
	virtual ~CReportTask();
	virtual std::string ToString() const;
	CFilter filter;
	virtual int TypeId() const override;
};

#endif /* TASKS_CREPORTTASK_H_ */
