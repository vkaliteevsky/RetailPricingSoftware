/*
 * Check_test.cpp
 *
 *  Created on: Feb 6, 2019
 *      Author: kraychik
 */

#include "gtest/gtest.h"
#include "CTaskManager.h"
#include "CProcessing.h"
#include "CSupplementMatrix.h"
#include "CStructureMap.h"
#include "CFilter.h"
using namespace std;
/*
double sumList(const list<shared_ptr<const CCheckItem>> & lst) {
	double sum = 0.0;
	for (const auto item : lst) {
		sum += ((item->price - item->discount) * item->volume);
	}
	return sum;
}
TEST(CCheck, FilterTest) {
	CCheck check(12345, 248157684, 12);
	check.AddLine(5, 100.0, 0.50, 10.0);
	check.AddLine(6, 120.0, 3.0, 0.0);
	check.AddLine(8, 25.0, 1.0, 0);
	EXPECT_DOUBLE_EQ(430.0, check.Income());

	CFilter filter;
	auto grInfoPtr = make_shared<CGroupsInfo>();
	CFilterAdvanced fAdv(filter, grInfoPtr);
	list<shared_ptr<const CCheckItem>> resList;
	check.FilterItems(fAdv, resList);
	EXPECT_DOUBLE_EQ(430.0, sumList(resList));

	filter.shopIds.insert(13);
	filter.useSupplementShops = true;
	check.FilterItems(fAdv, resList);
	EXPECT_DOUBLE_EQ(430.0, sumList(resList));
}
*/
