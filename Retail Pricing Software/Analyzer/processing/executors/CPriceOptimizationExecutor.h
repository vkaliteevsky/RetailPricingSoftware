/*
 * CPriceOptimizationExecutor.h
 *
 *  Created on: Jun 19, 2019
 *      Author: kraychik
 */

#ifndef PROCESSING_EXECUTORS_CPRICEOPTIMIZATIONEXECUTOR_H_
#define PROCESSING_EXECUTORS_CPRICEOPTIMIZATIONEXECUTOR_H_

#include "IExecutor.h"
#include "../../tasks/CPriceOptimizationTask.h"
#include <memory>

class CPriceOptimizationExecutor: public IExecutor {
public:
	CPriceOptimizationExecutor();
	virtual std::shared_ptr<const CCalcResult> ExecuteTask(std::shared_ptr<const CBaseTask> baseTask) override;
	virtual ~CPriceOptimizationExecutor();
};

#endif /* PROCESSING_EXECUTORS_CPRICEOPTIMIZATIONEXECUTOR_H_ */
