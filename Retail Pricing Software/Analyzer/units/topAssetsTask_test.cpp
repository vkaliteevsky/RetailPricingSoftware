/*
 * topReport_test.cpp
 *
 *  Created on: Feb 21, 2019
 *      Author: kraychik
 */

#include "gtest/gtest.h"
#include "CTaskManager.h"
#include "CProcessing.h"
#include "CTopAssetsTask.h"
using namespace std;

/*
TEST(CTopAssetsTask, Init) {
	auto baseTask = CTaskManager::CreateTask("/report/100/?type=topReport&amount=2&kpi=income");
	auto task = dynamic_pointer_cast<const CTopAssetsTask>(baseTask);
	EXPECT_NE(nullptr, task);
	EXPECT_EQ(2, task->amount);
	EXPECT_EQ(KPI::Income, task->kpi);
	EXPECT_EQ(true, task->filter.IsEmpty());

	baseTask = CTaskManager::CreateTask("/report/100/?type=topReport&amount=1");
	task = dynamic_pointer_cast<const CTopAssetsTask>(baseTask);
	EXPECT_NE(nullptr, task);
	EXPECT_EQ(1, task->amount);
	EXPECT_EQ(KPI::Income, task->kpi);
	EXPECT_EQ(true, task->filter.IsEmpty());
}

TEST(CTopAssetsTask, ExecutionIncome) {
	CProcessing *processing = new CProcessing();
	CResultManager *resultManager = CResultManager::GetInstance();
	auto baseTask = CTaskManager::CreateTask("/report/100/?type=topReport&amount=2&kpi=income");
	processing->ExecuteTask(baseTask);
	auto calcResult = resultManager->GetResult(baseTask->calcId);
	EXPECT_STREQ("[ { \"assetId\": 5, \"income\": 402.000000, \"volume\": 4.000000, \"incomePercent\": 0.580925, \"volumePercent\": 0.444444 }, { \"assetId\": 6, \"income\": 150.000000, \"volume\": 3.000000, \"incomePercent\": 0.216763, \"volumePercent\": 0.333333 } ]", calcResult->resultString.c_str());

	baseTask = CTaskManager::CreateTask("/report/100/?type=topReport&amount=1&kpi=income");
	processing->ExecuteTask(baseTask);
	calcResult = resultManager->GetResult(baseTask->calcId);
	EXPECT_STREQ("[ { \"assetId\": 5, \"income\": 402.000000, \"volume\": 4.000000, \"incomePercent\": 0.580925, \"volumePercent\": 0.444444 } ]", calcResult->resultString.c_str());

	baseTask = CTaskManager::CreateTask("/report/100/?type=topReport&amount=1");
	processing->ExecuteTask(baseTask);
	calcResult = resultManager->GetResult(baseTask->calcId);
	EXPECT_STREQ("[ { \"assetId\": 5, \"income\": 402.000000, \"volume\": 4.000000, \"incomePercent\": 0.580925, \"volumePercent\": 0.444444 } ]", calcResult->resultString.c_str());

	baseTask = CTaskManager::CreateTask("/report/100/?type=topReport&amount=0");
	processing->ExecuteTask(baseTask);
	calcResult = resultManager->GetResult(baseTask->calcId);
	EXPECT_STREQ("[ ]", calcResult->resultString.c_str());

	//cout << calcResult->resultString << endl;
}
*/
