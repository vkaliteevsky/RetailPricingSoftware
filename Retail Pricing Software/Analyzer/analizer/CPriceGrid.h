/*
 * CPriceGrid.h
 *
 *  Created on: Feb 10, 2019
 *      Author: kraychik
 */

#ifndef ANALIZER_CPRICEGRID_H_
#define ANALIZER_CPRICEGRID_H_

#include <vector>
#include <map>
#include <set>
#include <string>
#include <memory>
#include "../supply/CSmartException.h"

class CPriceGrid {
public:
	CPriceGrid(int minPrice, int maxPrice);
	virtual ~CPriceGrid();
	double Weight(int price) const;
	double WeightRight(int price) const;
	double WeightLeft(int price) const;
	void AddWeightInfo(int priceOld, int priceNew, double weight);
	double Effectiveness() const;
	std::vector<double> GetGridSequence(int price1, int price2) const;
	//bool IsGoodToMoveRight(int price) const;
private:
	int FindIndex(int price) const;
	int minPrice;
	int maxPrice;
	std::vector<double> infoWs;	// итоговая мера полезности для каждой цены price
	//std::vector<double> weight; 	// значимость каждого актива
};

#endif /* ANALIZER_CPRICEGRID_H_ */
