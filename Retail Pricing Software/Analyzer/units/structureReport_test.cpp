#include "gtest/gtest.h"
#include "CTaskManager.h"
#include "CProcessing.h"
#include "CStructureMap.h"
using namespace std;

/*
TEST(StructureReport, SimpleTask) {
	auto idManager = CCalcIdsManager::GetInstance();
	//cout << idManager->ToString() << endl << endl;
	CTaskManager *taskManager = CTaskManager::GetInstance();
	auto tk = taskManager->CreateTask("/report/100/?type=CheckStructure");
	shared_ptr<const CCheckStructureTask> task = dynamic_pointer_cast<const CCheckStructureTask>(tk);
	EXPECT_NE(nullptr, task);

	bool isEmpty = (task->filter.groupIds.empty() && task->filter.assetIds.empty() && task->filter.shopIds.empty());
	EXPECT_EQ(true, isEmpty);
	EXPECT_EQ(true, task->filter.useSupplementAssets && task->filter.useSupplementGroups && task->filter.useSupplementShops);

	CProcessing *processing = new CProcessing();
	//cout << idManager->ToString() << endl << endl;
	EXPECT_EQ(CCalcIdsManager::IdStatus::NotExecutedYet, idManager->GetStatus(task->calcId));
	processing->ExecuteTask(task);
	//cout << idManager->ToString() << endl << endl;
	CResultManager *resultManager = CResultManager::GetInstance();
	auto calcResult = resultManager->GetResult(task->calcId);


	//cout << calcResult->resultString << endl;
	//EXPECT_STREQ(CJSONCreator::CreateClassicResponse(task->calcId, ), nullptr, true), calcResult->resultString.c_str());
}

TEST(StructureReport, SimpleTaskWithShops) {
	CTaskManager *taskManager = CTaskManager::GetInstance();
	auto tk = taskManager->CreateTask("/report/100/?type=CheckStructure&shops=13&useSupplementShops=0");
	shared_ptr<const CCheckStructureTask> task = dynamic_pointer_cast<const CCheckStructureTask>(tk);
	EXPECT_NE(nullptr, task);

	//cout << "1" << endl;
	bool correctStructs = (task->filter.groupIds.empty() && task->filter.assetIds.empty() && (task->filter.shopIds.size() == 1));
	EXPECT_EQ(true, correctStructs);
	EXPECT_EQ(true, task->filter.useSupplementAssets && task->filter.useSupplementGroups && !(task->filter.useSupplementShops));

	//cout << "2" << endl;
	CProcessing *processing = new CProcessing();
	processing->ExecuteTask(task);
	CResultManager *resultManager = CResultManager::GetInstance();
	auto calcResult = resultManager->GetResult(task->calcId);

	cout << calcResult->resultString << endl;
	//EXPECT_STREQ(CJSONCreator::CreateClassicResponse(task->calcId, ), nullptr, true), calcResult->resultString.c_str());
}
*/
