/*
 * CAssetsSummary.h
 *
 *  Created on: Feb 21, 2019
 *      Author: kraychik
 */

#ifndef STRUCTURES_CASSETSSUMMARY_H_
#define STRUCTURES_CASSETSSUMMARY_H_

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <memory>
#include "Structures.h"
#include "CAssetsSummaryItem.h"
#include "../supply/CSmartException.h"

class CAssetsSummary {
public:
	CAssetsSummary();
	CAssetsSummary(int size);
	void AddItem(int assetId, double income, double amount);
	CAssetsSummaryItem GetItem(int assetId);
	std::shared_ptr<CAssetsSummaryItem> GetItemPtr(int assetId);
	std::vector<std::shared_ptr<CAssetsSummaryItem>> GetTopItems(int amount, KPI kpi, bool isDecreasing = true);
	int Size() const;
	std::string ToString();
private:
	void Sort(KPI kpi, bool isDecreasing = true);
	void MakeCalculations();
	int FindItem(int assetId) const;

	template <typename T>
	void SwapElements(std::vector<T> & vect, const std::vector<int> & inds);

	std::vector<int> assetIds;
	std::vector<double> incomes;
	std::vector<double> volumes;
	std::vector<double> incomePers;
	std::vector<double> volumePers;
	std::map<int, int> mapAssetIdToIndex;
	bool needsToMakeCalculations = true;	// если True, то необходимо вычислить все значения массивов incomePers и amountPers
};

#endif /* STRUCTURES_CASSETSSUMMARY_H_ */
