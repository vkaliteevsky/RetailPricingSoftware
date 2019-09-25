/*
 * CalcIdsManager_test.cpp
 *
 *  Created on: Feb 25, 2019
 *      Author: kraychik
 */

#include "gtest/gtest.h"
#include "CTaskManager.h"
#include "CProcessing.h"
#include "CSupplementMatrix.h"
#include "CStructureMap.h"
#include "CFilter.h"
#include "CCalcIdsManager.h"

/*
TEST(CalcIdsManager, All) {
	auto idManager = CCalcIdsManager::GetInstance();
	EXPECT_EQ(CCalcIdsManager::IdStatus::NotExists, idManager->GetStatus(12345));
	idManager->AddCalcId(123, CCalcIdsManager::IdStatus::UnderExecution);
	EXPECT_EQ(CCalcIdsManager::IdStatus::UnderExecution, idManager->GetStatus(123));
	idManager->MakeNotExecutedYet(123);
	EXPECT_EQ(CCalcIdsManager::IdStatus::NotExecutedYet, idManager->GetStatus(123));
	idManager->RemoveCalcId(321);
	EXPECT_EQ(CCalcIdsManager::IdStatus::NotExists, idManager->GetStatus(321));
	idManager->MakeExecuted(123);
	EXPECT_EQ(CCalcIdsManager::IdStatus::Executed, idManager->GetStatus(123));
	idManager->RemoveCalcId(123);
	EXPECT_EQ(CCalcIdsManager::IdStatus::NotExists, idManager->GetStatus(123));
}
*/
