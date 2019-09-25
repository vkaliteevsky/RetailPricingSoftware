/*
 * CStructureReportExecutor.h
 *
 *  Created on: Feb 25, 2019
 *      Author: kraychik
 */

#ifndef PROCESSING_EXECUTORS_CSTRUCTUREREPORTEXECUTOR_H_
#define PROCESSING_EXECUTORS_CSTRUCTUREREPORTEXECUTOR_H_

#include "IExecutor.h"
#include "../../tasks/CCheckStructureTask.h"

class CStructureReportExecutor: public IExecutor {
public:
	CStructureReportExecutor();
	virtual std::shared_ptr<const CCalcResult> ExecuteTask(std::shared_ptr<const CBaseTask> baseTask) override;
	virtual ~CStructureReportExecutor();
};

#endif /* PROCESSING_EXECUTORS_CSTRUCTUREREPORTEXECUTOR_H_ */
