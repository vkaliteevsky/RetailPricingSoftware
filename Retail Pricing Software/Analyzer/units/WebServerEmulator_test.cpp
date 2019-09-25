/*
 * WebServerEmulator_test.cpp
 *
 *  Created on: Jan 26, 2019
 *      Author: kraychik
 */

#include "gtest/gtest.h"
#include "CWebServer.h"
#include "CProcessing.h"
#include <iostream>

using namespace std;
/*
TEST(WebEmulator, FullTaskPath) {
	CProcessing *processing = new CProcessing();
	CResultManager *resultManager = CResultManager::GetInstance();
	CTaskManager *taskManager = CTaskManager::GetInstance();
	string correctResp;

	taskManager->Clear();
	resultManager->EraseAll();

	auto task1 = taskManager->CreateTask("/report/100/?type=IncomeDynamics&shops=12&useSupplementShops=0");
	EXPECT_STREQ(CJSONCreator::CreateClassicResponse(task1->calcId, nullptr, nullptr, true).c_str(), CWebServer::FormResponseString(task1).c_str());
	auto task2 = taskManager->CreateTask("/report/100/?type=IncomeDynamics&shops=12&useSupplementShops=1");
	EXPECT_STREQ(CJSONCreator::CreateClassicResponse(task2->calcId, nullptr, nullptr, true).c_str(), CWebServer::FormResponseString(task2).c_str());
	auto task3 = taskManager->CreateTask("/report/100/?type=IncomeDynamics");
	EXPECT_STREQ(CJSONCreator::CreateClassicResponse(task3->calcId, nullptr, nullptr, true).c_str(), CWebServer::FormResponseString(task3).c_str());
	//std::cout << CWebServer::FormResponseString(task3).c_str() << std::endl;

	auto task4 = taskManager->CreateTask(string("/result/100/?calcId=")+to_string(task1->calcId));
	string correct = CJSONCreator::CreateClassicResponse(task4->calcId, "wait", nullptr, true);
	EXPECT_STREQ(correct.c_str(), CWebServer::FormResponseString(task4).c_str());
	auto task5 = taskManager->CreateTask(string("/result/100/?calcId=")+to_string(415235616));
	EXPECT_STREQ(CJSONCreator::CreateClassicResponse(task5->calcId, "error", nullptr, false).c_str(), CWebServer::FormResponseString(task5).c_str());
	auto task6 = taskManager->CreateTask(string("/result/100/?calcId=")+to_string(task1->calcId));
	EXPECT_STREQ(CJSONCreator::CreateClassicResponse(task6->calcId, "wait", nullptr, true).c_str(), CWebServer::FormResponseString(task6).c_str());

	EXPECT_EQ(3, taskManager->Size());
	processing->WakeUp();
	EXPECT_EQ(2, taskManager->Size());

	auto task7 = taskManager->CreateTask(string("/result/100/?calcId=")+to_string(task2->calcId));
	EXPECT_STREQ(CJSONCreator::CreateClassicResponse(task7->calcId, "wait", nullptr, true).c_str(), CWebServer::FormResponseString(task7).c_str());
	auto task8 = taskManager->CreateTask(string("/result/100/?calcId=")+to_string(task3->calcId));
	EXPECT_STREQ(CJSONCreator::CreateClassicResponse(task8->calcId, "wait", nullptr, true).c_str(), CWebServer::FormResponseString(task8).c_str());
	auto task9 = taskManager->CreateTask(string("/result/100/?calcId=")+to_string(task1->calcId));
	EXPECT_STREQ(CJSONCreator::CreateClassicResponse(task9->calcId, "491.000000", nullptr, true).c_str(), CWebServer::FormResponseString(task9).c_str());

	auto task10 = taskManager->CreateTask(string("/result/100/?calcId=")+to_string(task1->calcId));
	EXPECT_STREQ(CJSONCreator::CreateClassicResponse(task10->calcId, "491.000000", nullptr, true).c_str(), CWebServer::FormResponseString(task10).c_str());
	EXPECT_EQ(2, taskManager->Size());
	processing->WakeUp();
	auto task11 = taskManager->CreateTask(string("/result/100/?calcId=")+to_string(task2->calcId));
	EXPECT_STREQ(CJSONCreator::CreateClassicResponse(task11->calcId, "201.000000", nullptr, true).c_str(), CWebServer::FormResponseString(task11).c_str());
	EXPECT_EQ(1, taskManager->Size());
	processing->WakeUp();
	processing->WakeUp();
	processing->WakeUp();
	EXPECT_EQ(0, taskManager->Size());
}

TEST(DynamicProcessing, test1) {
	//CProcessing *processing = new CProcessing();
	CResultManager *resultManager = CResultManager::GetInstance();
	CTaskManager *taskManager = CTaskManager::GetInstance();
	taskManager->Clear();
	resultManager->EraseAll();
	auto task1 = taskManager->CreateTask("/result/100/?calcId=12345");
	EXPECT_STREQ(CJSONCreator::CreateClassicResponse(task1->calcId, "error", nullptr, false).c_str(), CWebServer::FormResponseString(task1).c_str());
}

TEST(DynamicProcessing, test2) {
	CProcessing *processing = new CProcessing();
	CResultManager *resultManager = CResultManager::GetInstance();
	CTaskManager *taskManager = CTaskManager::GetInstance();
	taskManager->Clear();
	resultManager->EraseAll();

	auto task1 = taskManager->CreateTask("/report/100/?type=IncomeDynamics&periods=1546041600,1546128000,1546128001");
	processing->ExecuteTask(task1);
	auto calcResult = resultManager->GetResult(task1->calcId);
	//std::cout << calcResult->ToString() << std::endl;
	std::cout << CJSONCreator::CreateClassicResponse(task1->calcId, calcResult->resultString.c_str(), nullptr, true) << std::endl;
	//EXPECT_STREQ(CJSONCreator::CreateClassicResponse(task1->calcId, "error", nullptr, false).c_str(), CWebServer::FormResponseString(task1).c_str());
}
*/
