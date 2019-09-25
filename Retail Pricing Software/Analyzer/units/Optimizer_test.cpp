/*
 * Optimizer_test.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: kraychik
 */

/*
#include "gtest/gtest.h"
#include "CProcessing.h"
#include "CPriceList.h"
#include "COptimizer.h"
#include <memory>
using namespace std;

TEST(Optimizer, Basic) {
	COptimizerParams params;
	params.AddParamItem(5, make_shared<CParamsItem>(1, 90.0, 110.0, false));
	params.AddParamItem(6, make_shared<CParamsItem>(1, 40.0, 60.0, false));
	params.AddParamItem(7, make_shared<CParamsItem>(1, 25.0, 35.0, false));
	EXPECT_DOUBLE_EQ(60.0, params.GetParamItem(6)->maxPrice);
	EXPECT_EQ(1, params.GetAllGroups()->size());
	EXPECT_EQ(3, params.GetAssetsFromGroup(1)->size());

	auto salesHistory = make_shared<CSalesHistory>();
	CDataBase db;
	auto items = db.ReadSales(100);
	for (const auto & item : *items) {
		salesHistory->AddLine(item);
	}
	EXPECT_DOUBLE_EQ(692, salesHistory->Income());

	auto priceList = make_shared<CPriceList>();
	priceList->AddPriceItem(6, 50.0, 20.0);
	priceList->AddPriceItem(5, 100.0, 80.0);
	priceList->AddPriceItem(7, 30.0, 15.0);
	EXPECT_DOUBLE_EQ(100.0, priceList->GetPrice(5));

	COptimizer optimizer(salesHistory, priceList);
	auto assetIds = make_shared<set<int>>();
	assetIds->insert(5); assetIds->insert(6); assetIds->insert(7);
	auto newPriceList = optimizer.CalcExperimentalPriceListInGroup(assetIds, make_shared<COptimizerParams>(params));
	cout << newPriceList->ToString() << endl;
}
TEST(Optimizer, FromBD) {
	COptimizer optimizer(100);
	COptimizerParams params;
	params.AddParamItem(5, make_shared<CParamsItem>(1, 90.0, 110.0, false));
	params.AddParamItem(6, make_shared<CParamsItem>(1, 40.0, 60.0, false));
	params.AddParamItem(7, make_shared<CParamsItem>(1, 50.0, 75.0, false));
	shared_ptr<CPriceList> priceList = optimizer.CalcExperimentalPriceList(params);
	//cout << priceList->ToString() << endl;
}
*/
