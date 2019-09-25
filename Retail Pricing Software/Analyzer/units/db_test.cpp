#include "CDataBase.h"
#include "gtest/gtest.h"
#include "CTimer.h"

using namespace std;

/*
TEST(CDataBase, ReadSales) {
	CDataBase *db = CDataBase::GetInstance();
	auto rows = db->ReadSales(100);
	EXPECT_EQ(6, rows->size());
	double income = 0.0;
	for (const auto x : *rows) {
		income += (x.price - x.discount) * (x.volume);
	}
	EXPECT_DOUBLE_EQ(692.0, income);
}
*/
