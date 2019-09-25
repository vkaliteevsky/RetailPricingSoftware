/*
 * IExecutor.h
 *
 *  Created on: Feb 25, 2019
 *      Author: kraychik
 */

#ifndef PROCESSING_EXECUTORS_IEXECUTOR_H_
#define PROCESSING_EXECUTORS_IEXECUTOR_H_

#include <memory>
#include <vector>
#include "../../tasks/CBaseTask.h"
#include "../../storage/CDataManager.h"
#include "../CCalcResult.h"

class CProjectInfo;
class IExecutor {
public:
	IExecutor();
	virtual std::shared_ptr<const CCalcResult> ExecuteTask(std::shared_ptr<const CBaseTask> baseTask) = 0;
	virtual ~IExecutor();
protected:
	template <typename T>
	std::string MakeResultStringFromVector(const vector<T> & vect) const;
	std::string MakeResultStringForStructure(const std::map<int, double> & mapVals, int amountOfChecks) const;
	std::shared_ptr<const CProjectInfo> GetProjectInfo(int projectId) const;
};

#include "CTopAssetsExecutor.h"
#include "CDynamicsReportExecutor.h"
#include "CStructureReportExecutor.h"
#include "CSupplementReportExecutor.h"
#include "CPriceOptimizationExecutor.h"

#endif /* PROCESSING_EXECUTORS_IEXECUTOR_H_ */
