/*
 * CPriceOptimizationExecutor.cpp
 *
 *  Created on: Jun 19, 2019
 *      Author: kraychik
 */

#include "CPriceOptimizationExecutor.h"
#include "../../analizer/CRandomPriceOptimizer.h"
#include "../../analizer/CElastPriceOptimizer.h"
#include "../../analizer/forcast/CGroupElasticityForcaster.h"
#include "../../analizer/forcast/CSplineForcaster.h"

CPriceOptimizationExecutor::CPriceOptimizationExecutor() {

}

std::shared_ptr<const CCalcResult> CPriceOptimizationExecutor::ExecuteTask(std::shared_ptr<const CBaseTask> baseTask) {
	auto task = static_pointer_cast<const CPriceOptimizationTask>(baseTask);
	auto dataManager = CDataManager::GetInstance();
	auto projectInfoPtr = dataManager->GetOrLoadProjectInfo(task->projectId);

	srand(12345);
	//CRandomPriceOptimizer priceOptimizer;
	auto params = projectInfoPtr->GetOrLoadOptimizerParams();
	auto salesHistory = projectInfoPtr->GetSalesHistory();
	auto curPriceList = projectInfoPtr->GetCurrentPriceList();
	const int companyId = task->projectId;
	//auto forcaster = make_shared<CGroupElasticityForcaster>(salesHistory, curPriceList, params);
	auto forcaster = make_shared<CSplineForcaster>(salesHistory, curPriceList, params);
	CElastPriceOptimizer priceOptimizer(companyId, salesHistory, curPriceList, params, forcaster);

	//priceOptimizer.SetParams(params);
	string resultString("");
	try {
		auto optimalPriceList = priceOptimizer.CalcOptimalPriceList(task->monthSince, task->periodDays, task->amountOfDenseAssets);
		CDataBase::GetInstance()->InsertOptimalPrices(companyId, optimalPriceList);
		cout << "Optimal Prices Calculated" << endl;
		std::map<int, bool> newMap;
		resultString = optimalPriceList->ToJSON(priceOptimizer.GetParams());
	} catch (const std::exception & e) {
		cout << "Error accured during execution: " << e.what() << endl;
		resultString = "error";
	}

	//auto resultString = "";

	const auto calcResult = std::make_shared<const CCalcResult>(task->projectId, task->calcId, resultString);
	return calcResult;
}

CPriceOptimizationExecutor::~CPriceOptimizationExecutor() {
}

