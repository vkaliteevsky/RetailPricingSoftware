/*
 * CTopAssetsTask.h
 *
 *  Created on: Feb 21, 2019
 *      Author: kraychik
 */

#ifndef TASKS_CTOPASSETSTASK_H_
#define TASKS_CTOPASSETSTASK_H_

#include "CReportTask.h"
#include "../structures/CFilter.h"

class CTopAssetsTask: public CReportTask {
public:
	CTopAssetsTask(int projectId, int calcId, const CFilter & filter, int amount, KPI kpi);
	virtual ~CTopAssetsTask();
	virtual int TypeId() const override;
	int amount = 0;
	KPI kpi = KPI::Income;
};

#endif /* TASKS_CTOPASSETSTASK_H_ */
