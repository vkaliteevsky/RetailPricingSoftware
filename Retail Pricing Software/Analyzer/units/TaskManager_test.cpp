/*
 * test1.cpp
 *
 *  Created on: Jan 22, 2019
 *      Author: kraychik
 */


#include "gtest/gtest.h"
#include "CTaskManager.h"
using namespace std;

/*
CTaskManager *taskManager = CTaskManager::GetInstance();
bool sumEqs(set<int> arr, int v) {
	int s = 0;
	for (auto x : arr) s += x;
	return s == v;
}

TEST(CreateTask, normal_long_input) {
	const char *req = "/report/12/?type=IncomeDynamics&periods=100,200,300,400&shops=11,12&assets=14,15,165&groups=1&useSupplementAssets=0&useSupplementGroups=0&useSupplementShops=0";
	auto taskBase = taskManager->CreateTask(req);
	shared_ptr<const CDynamicsTask> task = static_pointer_cast<const CDynamicsTask>(taskBase);
	EXPECT_EQ(0, task->filter.useSupplementAssets);
	EXPECT_EQ(0, task->filter.useSupplementGroups);
	EXPECT_EQ(0, task->filter.useSupplementShops);
	EXPECT_EQ(2, (int)task->filter.shopIds.size());
	EXPECT_EQ(3, (int)task->filter.assetIds.size());
	EXPECT_EQ(1, (int)task->filter.groupIds.size());
	EXPECT_EQ(3, (int)task->filter.intervals.size());
	EXPECT_EQ(12, task->projectId);
	bool b1 = (task->filter.shopIds.find(11) != task->filter.shopIds.end());
	bool b2 = (task->filter.shopIds.find(12) != task->filter.shopIds.end());
	EXPECT_EQ(true, b1 && b2);
	b1 = (task->filter.intervals[0].first == 100) && (task->filter.intervals[0].second == 199);
	b2 = (task->filter.intervals[1].first == 200) && (task->filter.intervals[1].second == 299);
	bool b3 = (task->filter.intervals[2].first == 300) && (task->filter.intervals[2].second == 399);
	EXPECT_EQ(true, b1 && b2 && b3);
	EXPECT_EQ(1, *task->filter.groupIds.begin());
	b1 = task->filter.assetIds.find(14) != task->filter.assetIds.end();
	b2 = task->filter.assetIds.find(15) != task->filter.assetIds.end();
	b3 = task->filter.assetIds.find(165) != task->filter.assetIds.end();
	EXPECT_EQ(true, b1 && b2 && b3);
	EXPECT_EQ(DynamicsTaskType::IncomeDynamics, task->dynamicsTaskType);
}

TEST(CreateTask, short_normal_input) {
	const char * urlSimple = "/report/100/?type=IncomeDynamics";
	shared_ptr<const CBaseTask> baseTask = taskManager->CreateTask(urlSimple);
	shared_ptr<const CDynamicsTask> task = static_pointer_cast<const CDynamicsTask>(baseTask);
	EXPECT_EQ(true, task->filter.intervals.empty() && task->filter.shopIds.empty() && task->filter.assetIds.empty() && task->filter.groupIds.empty());
	EXPECT_EQ(100, task->projectId);
	EXPECT_EQ(DynamicsTaskType::IncomeDynamics, task->dynamicsTaskType);
}

TEST(CreateTask, getResponse) {
	auto task = make_shared<CGetResultTask>(100, 12345);
	EXPECT_EQ(12345, task->calcId);
	auto task4 = taskManager->CreateTask(string("/result/100/?calcId=12345"));
	EXPECT_EQ(12345, task4->calcId);
}
*/
