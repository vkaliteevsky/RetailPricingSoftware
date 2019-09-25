/*
 * CMatrix_test.cpp
 *
 *  Created on: Jul 9, 2019
 *      Author: kraychik
 */

#include "gtest/gtest.h"
#include "CSalesHistory.h"
#include "CDataBase.h"
#include "CMatrix.h"
#include <numeric>
#include <memory>

using namespace std;

TEST(CMatrix, AddingTest) {
	CMatrix mtx;
	mtx.AddPlusValue(2, 100000, 20.0);
	mtx.AddPlusValue(5, 200000, 4.5);
	mtx.AddPlusValue(3, 50000, -2.5);
	mtx.AddPlusValue(5, 50000, 10.0);
	mtx.AddPlusValue(5, 200000, 9.0);

	EXPECT_DOUBLE_EQ(mtx.GetValue(2, 100000), 20.0);
	EXPECT_DOUBLE_EQ(mtx.GetValue(5, 200000), 13.5);
	EXPECT_DOUBLE_EQ(mtx.GetValue(5, 50000), 10.0);
	EXPECT_DOUBLE_EQ(mtx.GetValue(3, 50000), -2.5);
	EXPECT_EQ(mtx.GetAmountOfRows(), 3);
	EXPECT_EQ(mtx.GetAmountOfColumns(), 3);
	EXPECT_DOUBLE_EQ(mtx.GetValue(2, 50000), 0.0);

	mtx.SetValue(5, 200000, -3.2);
	mtx.SetValue(6, 50000, 30.0);
	EXPECT_DOUBLE_EQ(mtx.GetValue(5, 200000), -3.2);
	EXPECT_DOUBLE_EQ(mtx.GetValue(6, 50000), 30.0);
	EXPECT_DOUBLE_EQ(mtx.GetValue(6, 100000), 0.0);
	mtx.AddPlusValue(2, 100000, 4.5);
	EXPECT_DOUBLE_EQ(mtx.GetValue(2, 100000), 24.5);

	CMatrix mx = mtx.Clone();
	EXPECT_DOUBLE_EQ(mtx.GetValue(2, 100000), 24.5);
	EXPECT_DOUBLE_EQ(mtx.GetValue(6, 100000), 0.0);
	EXPECT_DOUBLE_EQ(mtx.GetValue(5, 200000), -3.2);
}
