/*
 * processing_test.cpp
 *
 *  Created on: Jan 24, 2019
 *      Author: kraychik
 */

#include "gtest/gtest.h"
#include "CProcessing.h"

using namespace std;

/*
CProcessing *processing = new CProcessing();

void test_execute_task(const char * url, string resultString, int projectId) {
	CTaskManager *taskManager = CTaskManager::GetInstance();
	CResultManager *resultManager = CResultManager::GetInstance();
	const char * urlSimple = url;
	shared_ptr<const CBaseTask> baseTask = taskManager->CreateTask(urlSimple);
	processing->ExecuteTask(baseTask);
	shared_ptr<const CCalcResult> result = resultManager->GetResult(baseTask->calcId);
	EXPECT_DOUBLE_EQ(atof(resultString.c_str()), atof(result->resultString.c_str()));
	EXPECT_EQ(projectId, baseTask->projectId);
	EXPECT_EQ(baseTask->calcId, result->calcId);
}

TEST(ProcessingTest, ExecuteTask) {
	//CResultManager *resultManager = CResultManager::GetInstance();
	test_execute_task("/report/100/?type=IncomeDynamics", "692", 100);
	test_execute_task("/report/100/?type=IncomeDynamics", "692.0000", 100);
	test_execute_task("/report/100/?type=IncomeDynamics&shops=12,13&useSupplementShops=0", "692.0000", 100);
	test_execute_task("/report/100/?type=IncomeDynamics&shops=12&useSupplementShops=1", "201.0", 100);
	test_execute_task("/report/100/?type=IncomeDynamics&assets=6&useSupplementAssets=0", "150.0", 100);
	test_execute_task("/report/100/?type=IncomeDynamics&assets=6&useSupplementAssets=0&useSupplementGroups=0", "0.0", 100);
	test_execute_task("/report/100/?type=IncomeDynamics&assets=10,11,12&useSupplementAssets=1", "692", 100);
	test_execute_task("/report/100/?type=IncomeDynamics&assets=5,6,7", "0.00", 100);
	test_execute_task("/report/100/?type=IncomeDynamics&assets=10,12,15&shops=12", "201.00", 100);
	test_execute_task("/report/100/?type=IncomeDynamics&assets=10,12,15&shops=12&useSupplementShops=0", "491.00", 100);
}
*/
