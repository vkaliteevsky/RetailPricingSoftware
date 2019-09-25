/*
 * SalesHistory_test.cpp
 *
 *  Created on: Jan 25, 2019
 *      Author: kraychik
 */

#include "gtest/gtest.h"
#include "CSalesHistory.h"
#include "CDataBase.h"
#include <numeric>
#include <memory>

using namespace std;

TEST(CSalesHistory, CMatrix) {
	const int companyId = 2;
	auto projectInfo = CDataManager::GetInstance()->GetOrLoadProjectInfo(companyId);
	const auto salesHistory = projectInfo->LoadSalesHistory();
	const auto params = projectInfo->GetOrLoadOptimizerParams();
	const auto groupsInfoPtr = params->CalcGroupsInfo();

	vector<interval> vect;
	vect.push_back(interval(1485907200, 1485993600));
	set<int> emptySet;
	set<int> assetSet = {1};
	CMatrix priceMatrix, volumeMatrix, incomeMatrix;
	map<time_t, int> amtOfChecks;
	amtOfChecks = salesHistory->GetDaysAmountOfChecks();

	{
	CFilter filter(vect, assetSet, emptySet, emptySet, false, false, true, true);	// filter by groupId only
	CFilterAdvanced aFilter(filter, groupsInfoPtr);
	salesHistory->FillDaysStatistics(priceMatrix, volumeMatrix, incomeMatrix, aFilter);
	//cout << priceMatrix.ToString() << endl;
	//cout << volumeMatrix.ToString() << endl;
	EXPECT_EQ(volumeMatrix.GetAmountOfRows(), 1);
	//cout << amtOfChecks.size() << endl;
	//cout << priceMatrix.GetAmountOfColumns() << endl;
	//EXPECT_EQ((int)amtOfChecks.size(), priceMatrix.GetAmountOfColumns());
	}


	CFilter filter(vect, emptySet, assetSet, emptySet, false, true, false, true);	// filter by assetIds only
	CFilterAdvanced aFilter(filter, groupsInfoPtr);
	salesHistory->FillDaysStatistics(priceMatrix, volumeMatrix, incomeMatrix, aFilter);
	//cout << "Woring" << endl;
	//cout << volumeMatrix.ToString() << endl;
	//cout << priceMatrix.ToString() << endl;
	//cout << incomeMatrix.ToString() << endl;
	EXPECT_EQ(volumeMatrix.GetAmountOfRows(), 1);
	//cout << amtOfChecks.size() << endl;
	//EXPECT_EQ(amtOfChecks.size(), priceMatrix.GetAmountOfColumns());
	//cout << amtOfChecks << endl;
}
TEST(SalesHistory, CommonActions) {
	const int companyId = 2;
	auto projectInfo = CDataManager::GetInstance()->GetOrLoadProjectInfo(companyId);
	const auto salesHistory = projectInfo->LoadSalesHistory();
	vector<interval> vect;
	vect.push_back(interval(1483401600, 1483488000));
	set<int> emptySet;
	set<int> assetSet = {2, 3};
	CFilter filter(vect, assetSet, emptySet, emptySet, false, false, true, true);
	vector<double> income = salesHistory->CalculateDynamics(filter, FunMode::Sum);
	//cout << "Income: " << income[0] << endl;
	EXPECT_DOUBLE_EQ(income[0], 3418.0);
}
TEST(SupplementMatrix, All) {
	const int companyId = 2;
	auto projectInfo = CDataManager::GetInstance()->GetOrLoadProjectInfo(companyId);
	const auto salesHistory = projectInfo->LoadSalesHistory();
	const auto params = projectInfo->GetOrLoadOptimizerParams();
	const auto groupsInfoPtr = params->CalcGroupsInfo();
	CFilter filter(true);
	filter.assetIds.insert(1);
	filter.assetIds.insert(2);
	filter.assetIds.insert(23);
	filter.assetIds.insert(47);
	filter.useSupplementAssets = false;
	CFilterAdvanced aFilter(filter, groupsInfoPtr);
	auto mtx = salesHistory->BuildSupplementMatrix(aFilter);
	//mtx->CalcGroupMatrix(groupsInfoPtr, aFilter.assetIds);
	//auto groupMtx = mtx->GetGroupMatrix();
	cout << mtx->ToString() << endl;
	//cout << groupMtx->ToString() << endl;
	set<int> denseAssetIds;
	mtx->ChooseTopDenseAssets(10, denseAssetIds);
	for (int assetId : denseAssetIds) {
		cout << assetId << " ";
	}
	cout << endl;
}
/*
TEST(SalesHistory, AddLineTest) {
	CSalesHistory sales;
	CDataBase db;
	auto items = db.ReadSales(100);
	for (const auto & item : *items) {
		sales.AddLine(item);
	}
	EXPECT_DOUBLE_EQ(692, sales.Income());


	CIntervalSeries intervals;
	intervals.push_back(pair<time_t, time_t>(10, 9999999999999));
	set<int> emptySet = { };
	set<int> set12; set12.insert(12);
	set<int> set6; set6.insert(6);
	set<int> set100; set100.insert(100);
	set<int> set1 = {1}, set13 = {13};
	set<int> set5 = {5}, set10 = {10}, set7 = {7};
	const FunMode sm = FunMode::Sum;

	vector<double> res = sales.CalculateDynamics(CFilter(intervals, emptySet, emptySet, set12, false, true, true, false), FunMode::Sum);
	EXPECT_DOUBLE_EQ(491.0, accumulate(begin(res), end(res), 0.0));

	res = sales.CalculateDynamics(CFilter(intervals, emptySet, emptySet, emptySet, false, true, true, true), sm);
	EXPECT_DOUBLE_EQ(692.0, accumulate(begin(res), end(res), 0.0));

	res = sales.CalculateDynamics(CFilter(intervals, emptySet, emptySet, set12, false, true, true, true), sm);
	EXPECT_DOUBLE_EQ(692.0-491.0, accumulate(begin(res), end(res), 0.0));

	res = sales.CalculateDynamics(CFilter(intervals, set6, emptySet, emptySet, false, false, true, true), sm);
	EXPECT_DOUBLE_EQ(150.0, accumulate(begin(res), end(res), 0.0));

	res = sales.CalculateDynamics(CFilter(intervals, set6, emptySet, emptySet, false, true, true, true), sm);
	EXPECT_DOUBLE_EQ(692.0-150.0, accumulate(begin(res), end(res), 0.0));

	res = sales.CalculateDynamics(CFilter(intervals, set100, emptySet, emptySet, false, true, true, true), sm);
	EXPECT_DOUBLE_EQ(692.0, accumulate(begin(res), end(res), 0.0));

	const vector<int> assetIdsInfo = {5, 6, 7};
	const vector<int> groupIdsInfo = {1, 2, 2};
	auto groupInfo = make_shared<CGroupsInfo>(assetIdsInfo, groupIdsInfo);
	sales.SetGroupsInfo(groupInfo);

	res = sales.CalculateDynamics(CFilter(intervals, emptySet, set1, emptySet, false, true, false, true), sm);
	EXPECT_DOUBLE_EQ(402.0, accumulate(begin(res), end(res), 0.0));

	res = sales.CalculateDynamics(CFilter(intervals, set5, set1, emptySet, false, false, true, true), sm);
	EXPECT_DOUBLE_EQ(0.0, accumulate(begin(res), end(res), 0.0));

	res = sales.CalculateDynamics(CFilter(intervals, set7, set10, set12, false, false, true, false), sm);
	EXPECT_DOUBLE_EQ(140.0, accumulate(begin(res), end(res), 0.0));

	intervals.clear();

	res = sales.CalculateDynamics(CFilter(intervals, emptySet, emptySet, emptySet, true, true, true, true), sm);
	EXPECT_DOUBLE_EQ(692.0, accumulate(begin(res), end(res), 0.0));

	intervals.push_back(pair<time_t, time_t>(1, 9999999999999));
	CStructureMap<int, double> checkStructure;
	int amountOfChecks = 0;

	sales.CalculateCheckStructure(CFilter(intervals, emptySet, emptySet, emptySet, false, true, true, true), checkStructure, amountOfChecks);
	EXPECT_DOUBLE_EQ(1, checkStructure.structure[5]);
	EXPECT_DOUBLE_EQ(0.75, checkStructure.structure[6]);
	EXPECT_DOUBLE_EQ(0.5, checkStructure.structure[7]);

	checkStructure.Clear();
	amountOfChecks = 0;

	sales.CalculateCheckStructure(CFilter(intervals, emptySet, emptySet, set13, false, true, true, false), checkStructure, amountOfChecks);
	EXPECT_DOUBLE_EQ(2, checkStructure.structure[5]);
	EXPECT_EQ(1, amountOfChecks);
}
*/
