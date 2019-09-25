/*
 * CDynamicsReportExecutor.cpp
 *
 *  Created on: Feb 25, 2019
 *      Author: kraychik
 */

#include "CDynamicsReportExecutor.h"

std::shared_ptr<const CCalcResult> CDynamicsReportExecutor::ExecuteTask(std::shared_ptr<const CBaseTask> baseTask) {
	//std::cout << "In DynamicsReport" << std::endl;
	//std::cout << baseTask->ToString() << std::endl;
	auto task = static_pointer_cast<const CDynamicsTask>(baseTask);
	auto projectInfo = GetProjectInfo(task->projectId);
	const FunMode funMode = (task->dynamicsTaskType == DynamicsTaskType::IncomeDynamics) ? FunMode::Sum : FunMode::AvgCheck;
	CIntervalSeries intervals(task->filter.intervals);
	std::shared_ptr<const CSalesHistory> salesHistory = projectInfo->GetSalesHistory();
	const std::vector<double> dynamics = salesHistory->CalculateDynamics(task->filter, funMode);
	//std::string resultString = MakeResultStringFromVector<double>(dynamics);
	auto resultString = MakeResultStringFromVector(dynamics);
	const auto calcResult = std::make_shared<const CCalcResult>(task->projectId, task->calcId, resultString);
	return calcResult;
}

CDynamicsReportExecutor::CDynamicsReportExecutor() {
}

CDynamicsReportExecutor::~CDynamicsReportExecutor() {

}

