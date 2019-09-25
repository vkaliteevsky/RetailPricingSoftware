/*
 * CSupplementReportExecutor.h
 *
 *  Created on: Feb 25, 2019
 *      Author: kraychik
 */

#ifndef PROCESSING_EXECUTORS_CSUPPLEMENTREPORTEXECUTOR_H_
#define PROCESSING_EXECUTORS_CSUPPLEMENTREPORTEXECUTOR_H_

#include "IExecutor.h"
#include "../../tasks/CSupplementTask.h"

class CSupplementReportExecutor : public IExecutor {
public:
	CSupplementReportExecutor();
	virtual std::shared_ptr<const CCalcResult> ExecuteTask(std::shared_ptr<const CBaseTask> baseTask) override;
	virtual ~CSupplementReportExecutor();
};

#endif /* PROCESSING_EXECUTORS_CSUPPLEMENTREPORTEXECUTOR_H_ */
