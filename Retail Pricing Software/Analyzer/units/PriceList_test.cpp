/*
 * PriceList_test.cpp
 *
 *  Created on: Feb 8, 2019
 *      Author: kraychik
 */


#include "gtest/gtest.h"
#include "CProcessing.h"
#include "CPriceList.h"
#include <memory>
using namespace std;

/*
TEST(PriceList, Basic) {
	CPriceList priceList;
	priceList.AddPriceItem(5, 100.0, 0.0);
	priceList.AddPriceItem(6, 50.0, 10.0);
	priceList.AddPriceItem(7, 85.0, 0.0);
	priceList.AddPriceItem(4, 110.5, 0.0);

	vector<int> assetIds;
	vector<shared_ptr<CPriceItem>> items;
	priceList.FillVectors(assetIds, items);
	EXPECT_EQ(4, assetIds[0]);
	EXPECT_EQ(5, assetIds[1]);
	EXPECT_EQ(6, assetIds[2]);
	EXPECT_EQ(7, assetIds[3]);
	EXPECT_EQ(4, items.size());
	EXPECT_DOUBLE_EQ(100+50+85+110.5, items[0]->price + items[1]->price + items[2]->price + items[3]->price);

	auto assets = priceList.GetAllAssets();
	EXPECT_EQ(4, assets->at(0)); EXPECT_EQ(5, assets->at(1)); EXPECT_EQ(6, assets->at(2)); EXPECT_EQ(7, assets->at(3));

	EXPECT_DOUBLE_EQ(10.0, priceList.GetCost(6));
	EXPECT_DOUBLE_EQ(110.5, priceList.GetPrice(4));
	EXPECT_LE(priceList.GetPrice(8), 0.0);

	priceList.AddPriceItem(6, 200.0, 5.0);
	EXPECT_DOUBLE_EQ(200.0, priceList.GetPrice(6));
	EXPECT_EQ(4, priceList.Size());
	priceList.FillVectors(assetIds, items);
	EXPECT_DOUBLE_EQ(100+200+85+110.5, items[0]->price + items[1]->price + items[2]->price + items[3]->price);

}

TEST(PriceList, Merge) {
	CPriceList priceList1;
	priceList1.AddPriceItem(5, 100.0, 0.0);
	priceList1.AddPriceItem(6, 50.0, 10.0);
	priceList1.AddPriceItem(7, 85.0, 0.0);
	priceList1.AddPriceItem(4, 110.5, 0.0);

	CPriceList priceList2;
	priceList2.AddPriceItem(10, 20.0, 0.0);
	priceList2.AddPriceItem(11, 30.0, 0.0);

	auto joinedList = CPriceList::Merge(make_shared<CPriceList>(priceList1), make_shared<CPriceList>(priceList2));
	vector<int> assetIds;
	vector<shared_ptr<CPriceItem>> items;
	joinedList->FillVectors(assetIds, items);
	EXPECT_DOUBLE_EQ(100+50+85+110.5+20+30, items[0]->price + items[1]->price + items[2]->price + items[3]->price + items[4]->price + items[5]->price);
	joinedList->AddPriceItem(6, 75.0, 5.0);
	EXPECT_DOUBLE_EQ(5.0, priceList1.GetCost(6));
}
*/
