/*
 * supplementMatrix_test.cpp
 *
 *  Created on: Feb 5, 2019
 *      Author: kraychik
 */

/*
#include "gtest/gtest.h"
#include "CTaskManager.h"
#include "CProcessing.h"
#include "CSupplementMatrix.h"
#include "CStructureMap.h"
#include "CFilter.h"
using namespace std;

TEST(SupplementMatrix, initTest) {
	cout << CCalcIdsManager::GetInstance()->ToString() << endl;
	CSalesHistory sales;
	CDataBase db;
	auto items = db.ReadSales(100);
	for (const auto & item : *items) {
		sales.AddLine(item);
	}
	EXPECT_DOUBLE_EQ(692.0, sales.Income());

	CFilter filter;
	shared_ptr<CSupplementMatrix> matrix = sales.BuildSupplementMatrix(filter);
	cout << matrix->ToString() << endl;
	list<shared_ptr<CStructureMap<int, double>>> resList;
	matrix->SliceAndBuildStructureMap({5, 7}, resList);
	EXPECT_EQ(2, (int)resList.size());
	cout << "TEST" << endl;
	for (const auto & item : resList) {
		cout << item->ToJSON("assetId", "value")<< endl;
	}
	cout << "END" << endl;
}
*/
