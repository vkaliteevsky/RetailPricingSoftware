/*
 * supplementTask_test.cpp
 *
 *  Created on: Feb 6, 2019
 *      Author: kraychik
 */

#include "gtest/gtest.h"
#include "CTaskManager.h"
#include "CResultManager.h"
#include "CPriceItem.h"
#include "CProcessing.h"
#include "CSupplementMatrix.h"
#include "CStructureMap.h"
#include "CFilter.h"
using namespace std;

/*
TEST(SupplementTask, MainTest) {
	CTaskManager *taskManager = CTaskManager::GetInstance();
	auto baseTask = taskManager->CreateTask("/report/100/?type=AssetSupplement&targetAssets=5,7");
	auto task = dynamic_pointer_cast<const CSupplementTask>(baseTask);
	EXPECT_NE(nullptr, task);
	EXPECT_EQ(2, (int)task->targetAssetIds.size());
	EXPECT_EQ(100, task->projectId);
	EXPECT_EQ(5, task->targetAssetIds[0]);
	EXPECT_EQ(7, task->targetAssetIds[1]);
	EXPECT_EQ(true, task->filter.useSupplementShops);

	CResultManager *resultManager = CResultManager::GetInstance();
	CProcessing *processing = new CProcessing();
	processing->ExecuteTask(task);
	auto calcResult = resultManager->GetResult(task->calcId);
	//cout << calcResult->resultString << endl;
}
*/
