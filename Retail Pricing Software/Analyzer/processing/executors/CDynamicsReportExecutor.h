/*
 * CDynamicsReportExecutor.h
 *
 *  Created on: Feb 25, 2019
 *      Author: kraychik
 */

#ifndef PROCESSING_EXECUTORS_CDYNAMICSREPORTEXECUTOR_H_
#define PROCESSING_EXECUTORS_CDYNAMICSREPORTEXECUTOR_H_

#include "IExecutor.h"
#include "../../tasks/CDynamicsTask.h"

class CDynamicsReportExecutor: public IExecutor {
public:
	CDynamicsReportExecutor();
	virtual std::shared_ptr<const CCalcResult> ExecuteTask(std::shared_ptr<const CBaseTask> baseTask) override;
	virtual ~CDynamicsReportExecutor();
};

#endif /* PROCESSING_EXECUTORS_CDYNAMICSREPORTEXECUTOR_H_ */
