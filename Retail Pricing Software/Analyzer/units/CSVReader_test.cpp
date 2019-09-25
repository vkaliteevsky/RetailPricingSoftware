#include "CCSVReader.h"
#include "gtest/gtest.h"
#include "CTimer.h"

using namespace std;
/*
TEST(CCSVReader, ReadSales) {
	CCSVReader *reader = new CCSVReader();
	auto hist = reader->ReadSales(0);
	EXPECT_EQ((int)hist->size(), 11);
	double income = 0.0;
	for (const auto x : *hist) {
		income += (x.price - x.discount) * (x.volume);
	}
	EXPECT_DOUBLE_EQ(2495.25, income);
	delete reader;
}
*/
