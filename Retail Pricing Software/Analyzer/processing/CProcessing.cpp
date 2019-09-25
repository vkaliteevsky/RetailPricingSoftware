/*
 * CProcessing.cpp
 *
 *  Created on: Jan 19, 2019
 *      Author: kraychik
 */

#include "CProcessing.h"
#include "executors/CPriceOptimizationExecutor.h"
#include <iostream>

CProcessing::CProcessing() {
	SetTimer();
	SetUpExecutor(4, std::make_shared<CDynamicsReportExecutor>());
	SetUpExecutor(5, std::make_shared<CStructureReportExecutor>());
	SetUpExecutor(6, std::make_shared<CSupplementReportExecutor>());
	SetUpExecutor(7, std::make_shared<CTopAssetsExecutor>());
	SetUpExecutor(8, std::make_shared<CPriceOptimizationExecutor>());
}

CProcessing::~CProcessing() {
	//delete timer;
}

void CProcessing::WakeUp() {
	//printf("Woken Up\n");
	std::shared_ptr<const CBaseTask> task = CTaskManager::GetInstance()->GetTask();
	if (task == nullptr) {
		return;
	} else {
		ExecuteTask(task);
	}
}

void CProcessing::ExecuteTask(std::shared_ptr<const CBaseTask> task) {
	//std::cout << "Executing task" << std::endl;
	const int projectId = task->projectId;
	CCalcIdsManager *idManager = CCalcIdsManager::GetInstance();
	idManager->MakeUnderExecution(task->calcId);

	const int taskTypeId = task->TypeId();
	const auto itMap = mapExecutor.find(taskTypeId);
	if (itMap == mapExecutor.end()) {
		throw CSmartException("CProcessing::ExecuteTask: no pre-defined command for taskTypeId = " + std::to_string(taskTypeId));
	}
	const auto taskExecutor = itMap->second;
	//std::cout << "Executing task2" << std::endl;
	std::shared_ptr<const CCalcResult> calcResult = taskExecutor->ExecuteTask(task);
	//std::cout << "Executed. Got CalcResults" << std::endl;
	CResultManager *resultManager = CResultManager::GetInstance();
	idManager->MakeExecuted(calcResult->calcId);
	resultManager->AddResult(calcResult);
	//return calcResult;
}

void CProcessing::WakeUpTimer(CProcessing *processing) {
	cout << "Timer awake ... " << endl;
	processing->WakeUp();
}

void CProcessing::SetUpExecutor(int taskTypeId, std::shared_ptr<IExecutor> executor) {
	mapExecutor[taskTypeId] = executor;
}

void CProcessing::SetTimer() {
	std::thread t([=]() {
		while(true) {
			//if(this->clear) return;
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
			//if(this->clear) return;
			WakeUp();
		}
	});
	t.detach();
}
