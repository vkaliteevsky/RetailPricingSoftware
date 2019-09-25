/*
 * CPriceGrid.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: kraychik
 */

#include "CPriceGrid.h"

CPriceGrid::CPriceGrid(int minPrice, int maxPrice) : minPrice(minPrice), maxPrice(maxPrice), infoWs(abs(maxPrice-minPrice+1), 0)  {
	if (minPrice > maxPrice) {
		throw CSmartException("CPriceGrid::CPriceGrid: minPrice must be less than maxPrice");
	}
	if (minPrice < 0) {
		throw CSmartException("CPriceGrid::CPriceGrid: minPrice must be positive");
	}
	//weight = weights;
}

CPriceGrid::~CPriceGrid() {
	// TODO Auto-generated destructor stub
}

double CPriceGrid::Weight(int price) const {
	int index = FindIndex(price);
	return infoWs[index];
}

double CPriceGrid::WeightRight(int price) const {
	int index = FindIndex(price);
	if (price >= maxPrice) return 0.0;
	else return infoWs[index+1];
}

double CPriceGrid::WeightLeft(int price) const {
	int index = FindIndex(price);
	if (price <= minPrice) return 0.0;
	else return infoWs[index-1];
}

void CPriceGrid::AddWeightInfo(int priceOld, int priceNew, double weight) {
	const int price1 = std::min(priceOld, priceNew);
	const int price2 = std::max(priceOld, priceNew);
	const int n = price2 - price1;
	weight /= n;
	const int index1 = FindIndex(price1);
	const int index2 = FindIndex(price2);
	for (int i = index1; i <= index2; ++i) {
		infoWs[i] += weight;
	}
}

int CPriceGrid::FindIndex(int price) const {
	if (price < minPrice || price > maxPrice) {
		throw CSmartException("CPriceGrid::FindIndex: price вне диапазона [minPrice, maxPrice]");
	}
	return (price - minPrice);
}

double CPriceGrid::Effectiveness() const {
	double eValue = 0.0;
	int amt = 0;
	for (const double v : infoWs) {
		eValue += v;
		amt += (v > 0 ? 1 : 0);
	}
	return eValue * ((double)amt / (double)infoWs.size());
}

std::vector<double> CPriceGrid::GetGridSequence(int price1, int price2) const {
	if (price1 > price2) {
		int tmp = price1; price1 = price2; price2 = tmp;
	}
	std::vector<double> res(price2 - price1 + 1);
	for (uint i = 0; i < res.size(); ++i) {
		res[i] = price1 + i;
	}
	return res;
}
