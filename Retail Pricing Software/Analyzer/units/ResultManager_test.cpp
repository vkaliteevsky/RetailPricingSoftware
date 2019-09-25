/*
 * ResultManager_test.cpp
 *
 *  Created on: Jan 25, 2019
 *      Author: kraychik
 */

#include "gtest/gtest.h"
#include "CResultManager.h"

using namespace std;
/*
CResultManager *resultManager = CResultManager::GetInstance();

TEST(CalcResult, testConstructor) {
	CCalcResult calcResult(100, 1245, "hello");
	EXPECT_EQ(100, calcResult.projectId);
	EXPECT_EQ(1245, calcResult.calcId);
	EXPECT_STREQ("hello", calcResult.resultString.c_str());
}

TEST(ResultManager, AddAndGet) {
	resultManager->AddResult(make_shared<CCalcResult>(100, 1245, "hello100"));
	resultManager->AddResult(make_shared<CCalcResult>(101, 1246, "hello101"));
	resultManager->AddResult(make_shared<CCalcResult>(102, 1247, "hello102"));
	auto r1 = resultManager->GetResult(1245);
	auto r2 = resultManager->GetResult(1246);
	auto r3 = resultManager->GetResult(1247);
	EXPECT_STREQ("hello100", r1->resultString.c_str());
	EXPECT_STREQ("hello101", r2->resultString.c_str());
	EXPECT_STREQ("hello102", r3->resultString.c_str());
	EXPECT_EQ(CalcResultStatus::SUCCESS, r1->status);

	auto r4 = resultManager->GetResult(100);
	EXPECT_EQ(CalcResultStatus::ERROR, r4->status);

	resultManager->AddResult(make_shared<CCalcResult>(105, 1248, "hello105"));
	//EXPECT_ANY_THROW(resultManager->AddResult(make_shared<CCalcResult>(106, 1248, "hello106")));
	EXPECT_EQ(CalcResultStatus::ERROR, resultManager->GetResult(1249)->status);

	//resultManager->InformAboutTaskExistance(1249);
	CCalcIdsManager *calcIdManager = CCalcIdsManager::GetInstance();
	calcIdManager->MakeUnderExecution(1249);
	EXPECT_EQ(CalcResultStatus::WAIT, resultManager->GetResult(1249)->status);
}
*/
