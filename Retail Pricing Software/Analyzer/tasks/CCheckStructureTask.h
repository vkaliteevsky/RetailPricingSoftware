/*
 * CCheckStructureTask.h
 *
 *  Created on: Jan 30, 2019
 *      Author: kraychik
 */

#ifndef TASKS_CCHECKSTRUCTURETASK_H_
#define TASKS_CCHECKSTRUCTURETASK_H_

#include "CReportTask.h"

class CCheckStructureTask: public CReportTask {
public:
	//CCheckStructureTask();
	CCheckStructureTask(int projectId, int calcId, const CFilter & filter);
	virtual ~CCheckStructureTask();
	virtual std::string ToString() const;
	virtual int TypeId() const override;
};

#endif /* TASKS_CCHECKSTRUCTURETASK_H_ */
