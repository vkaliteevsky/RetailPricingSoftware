/*
 * groupsInfo_test.cpp
 *
 *  Created on: Feb 22, 2019
 *      Author: kraychik
 */

#include "gtest/gtest.h"
#include "CSalesHistory.h"
#include "CGroupsInfo.h"
#include "CDataBase.h"

using namespace std;
/*
TEST(GroupsInfo, All) {
	CGroupsInfo gInfo({5, 6, 7, 8, 9}, {1, 1, 2, 3, 3});
	auto set1 = gInfo.GetAssetsInGroup(1);
	auto set2 = gInfo.GetAssetsInGroup(2);
	auto set3 = gInfo.GetAssetsInGroup(3);
	EXPECT_EQ(2, (int)set1->size());
	EXPECT_EQ(1, (int)set2->size());
	EXPECT_EQ(2, (int)set3->size());

	EXPECT_EQ(1, gInfo.GetGroupByAsset(6));
	EXPECT_EQ(3, gInfo.GetGroupByAsset(8));
	EXPECT_EQ(3, gInfo.GetGroupByAsset(9));

	std::vector<int> groupIds = { 2, 3 };
	auto joinedSet = gInfo.GetAssetsInGroups(groupIds);
	EXPECT_EQ(true, joinedSet->find(7) != joinedSet->end());
	EXPECT_EQ(true, joinedSet->find(8) != joinedSet->end());
	EXPECT_EQ(true, joinedSet->find(9) != joinedSet->end());
	EXPECT_EQ(3, (int)joinedSet->size());

	std::set<int> groupIdsSet = { 1, 2 };
	joinedSet = gInfo.GetAssetsInGroups(groupIdsSet);
	EXPECT_EQ(true, joinedSet->find(5) != joinedSet->end());
	EXPECT_EQ(true, joinedSet->find(6) != joinedSet->end());
	EXPECT_EQ(true, joinedSet->find(7) != joinedSet->end());
	EXPECT_EQ(3, (int)joinedSet->size());

	shared_ptr<vector<int>> allGroups = gInfo.GetAllGroups<vector<int>>();
	std::sort(allGroups->begin(), allGroups->end());
	EXPECT_EQ(1, allGroups->at(0));
	EXPECT_EQ(2, allGroups->at(1));
	EXPECT_EQ(3, allGroups->at(2));

	auto allGroupsSet = gInfo.GetAllGroups<set<int>>();
	EXPECT_EQ(true, allGroupsSet->find(1) != allGroupsSet->end());
	EXPECT_EQ(true, allGroupsSet->find(2) != allGroupsSet->end());
	EXPECT_EQ(true, allGroupsSet->find(3) != allGroupsSet->end());
	EXPECT_EQ(3, (int)allGroupsSet->size());

	gInfo.Clear();
	EXPECT_EQ(0, (int)gInfo.GetAllGroups<list<int>>()->size());
}
*/
