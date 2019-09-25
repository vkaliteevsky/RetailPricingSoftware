/*
 * CTopAssetsExecutor.h
 *
 *  Created on: Feb 25, 2019
 *      Author: kraychik
 */

#ifndef PROCESSING_EXECUTORS_CTOPASSETSEXECUTOR_H_
#define PROCESSING_EXECUTORS_CTOPASSETSEXECUTOR_H_

#include "IExecutor.h"
#include "../../tasks/CTopAssetsTask.h"

class CTopAssetsExecutor: public IExecutor {
public:
	CTopAssetsExecutor();
	virtual std::shared_ptr<const CCalcResult> ExecuteTask(std::shared_ptr<const CBaseTask> baseTask) override;
	virtual ~CTopAssetsExecutor();
};

#endif /* PROCESSING_EXECUTORS_CTOPASSETSEXECUTOR_H_ */
