/*
 * assetsSummary_test.cpp
 *
 *  Created on: Feb 21, 2019
 *      Author: kraychik
 */

#include "gtest/gtest.h"
#include "CSalesHistory.h"
#include "CDataBase.h"

using namespace std;

/*
void test_summaryAssets(shared_ptr<CAssetsSummary> assetsSummary) {
	auto asset5 = assetsSummary->GetItem(5);
	auto asset6 = assetsSummary->GetItem(6);
	auto asset7 = assetsSummary->GetItem(7);
	EXPECT_DOUBLE_EQ(402.0, asset5.income);
	EXPECT_DOUBLE_EQ(150.0, asset6.income);
	EXPECT_DOUBLE_EQ(140.0, asset7.income);
	EXPECT_DOUBLE_EQ(4.0, asset5.volume);
	EXPECT_DOUBLE_EQ(3.0, asset6.volume);
	EXPECT_DOUBLE_EQ(2.0, asset7.volume);
	EXPECT_DOUBLE_EQ(402.0/692.0, asset5.incomePer);
	EXPECT_DOUBLE_EQ(150.0/692.0, asset6.incomePer);
	EXPECT_DOUBLE_EQ(140.0/692.0, asset7.incomePer);
	EXPECT_DOUBLE_EQ(4.0/9.0, asset5.amountPer);
	EXPECT_DOUBLE_EQ(3.0/9.0, asset6.amountPer);
	EXPECT_DOUBLE_EQ(2.0/9.0, asset7.amountPer);
}
TEST(SummaryAssets, Init) {
	cout << CCalcIdsManager::GetInstance()->ToString() << endl;
	CSalesHistory sales;
	CDataBase db;
	auto items = db.ReadSales(100);
	for (const auto & item : *items) {
		sales.AddLine(item);
	}
	EXPECT_DOUBLE_EQ(692, sales.Income());
	CFilter filter;
	auto assetsSummary = sales.GetAssetsSummary(filter);
	EXPECT_EQ(3, assetsSummary->Size());
	test_summaryAssets(assetsSummary);
}

TEST(SummaryAssets, GetTop) {
	CSalesHistory sales;
	CDataBase db;
	auto items = db.ReadSales(100);
	for (const auto & item : *items) {
		sales.AddLine(item);
	}
	EXPECT_DOUBLE_EQ(692, sales.Income());

	CFilter filter;
	auto assetsSummary = sales.GetAssetsSummary(filter);
	EXPECT_EQ(3, assetsSummary->Size());

	auto tops = assetsSummary->GetTopItems(2, KPI::Income, true);
	EXPECT_EQ(5, tops[0]->assetId);
	EXPECT_DOUBLE_EQ(402.0, tops[0]->income);
	EXPECT_DOUBLE_EQ(4.0, tops[0]->volume);
	EXPECT_EQ(6, tops[1]->assetId);
	EXPECT_DOUBLE_EQ(150.0, tops[1]->income);
	EXPECT_DOUBLE_EQ(3.0, tops[1]->volume);
	test_summaryAssets(assetsSummary);

	tops = assetsSummary->GetTopItems(2, KPI::Income, false);
	EXPECT_EQ(7, tops[0]->assetId);
	EXPECT_DOUBLE_EQ(140.0, tops[0]->income);
	EXPECT_DOUBLE_EQ(2.0, tops[0]->volume);
	EXPECT_EQ(6, tops[1]->assetId);
	EXPECT_DOUBLE_EQ(150.0, tops[1]->income);
	EXPECT_DOUBLE_EQ(3.0, tops[1]->volume);
	test_summaryAssets(assetsSummary);
}
*/
