/*
 * CAssetsSummary.cpp
 *
 *  Created on: Feb 21, 2019
 *      Author: kraychik
 */

#include "CAssetsSummary.h"

void CAssetsSummary::AddItem(int assetId, double income, double amount) {
	int index = FindItem(assetId);
	if (index == -1) {
		// добавляем новый элемент
		assetIds.push_back(assetId);
		incomes.push_back(income);
		volumes.push_back(amount);
		incomePers.push_back(0.0);
		volumePers.push_back(0.0);
		mapAssetIdToIndex.insert(std::pair<int, int>(assetId, assetIds.size()-1));
	} else {
		// обновляем существующий элемент
		assetIds[index] = assetId;
		incomes[index] = income;
		volumes[index] = amount;
	}
	needsToMakeCalculations = true;
}

CAssetsSummaryItem CAssetsSummary::GetItem(int assetId) {
	if (needsToMakeCalculations) {
		MakeCalculations();
	}
	int i = FindItem(assetId);
	if (i == -1) {
		return CAssetsSummaryItem();
	} else {
		return CAssetsSummaryItem(assetIds[i], incomes[i], volumes[i], incomePers[i], volumePers[i]);
	}
}

std::shared_ptr<CAssetsSummaryItem> CAssetsSummary::GetItemPtr(int assetId) {
	if (needsToMakeCalculations) {
		MakeCalculations();
	}
	int i = FindItem(assetId);
	return (i == -1) ? std::make_shared<CAssetsSummaryItem>() : std::make_shared<CAssetsSummaryItem>(assetIds[i], incomes[i], volumes[i], incomePers[i], volumePers[i]);
}

int CAssetsSummary::Size() const {
	return assetIds.size();
}

std::string CAssetsSummary::ToString() {
	if (needsToMakeCalculations) MakeCalculations();
	std::string res("Assets Summary\n");
	for (uint i = 0; i < assetIds.size(); ++i) {
		res += "AssetId: " + std::to_string(assetIds[i]) + "; Income: " + std::to_string(incomes[i]);
		res += " (" + std::to_string(incomePers[i]) + "); Amounts: " + std::to_string(volumes[i]) + " (" + std::to_string(volumePers[i]) + ")\n";
	}
	return res;
}

void CAssetsSummary::Sort(KPI kpi, bool isDecreasing) {
	std::vector<int> inds(Size());
	for (uint i = 0; i < inds.size(); ++i) {
		inds[i] = i;
	}
	std::vector<double> &refVect = incomes;
	if (kpi == KPI::Income) {
		refVect = incomes;
	} else if (kpi == KPI::Volume) {
		refVect = volumes;
	} else {
		throw CSmartException("CAssetsSummary::Sort: not implemented yet");
	}
	if (isDecreasing) {
		std::sort(std::begin(inds), std::end(inds), [&](int i1, int i2) { return refVect[i1] > refVect[i2]; });
	} else {
		std::sort(std::begin(inds), std::end(inds), [&](int i1, int i2) { return refVect[i1] < refVect[i2]; });
	}
	// получили массив индексов inds, упорядоченный в нужном порядке
	SwapElements<int>(assetIds, inds);
	SwapElements<double>(incomes, inds);
	SwapElements<double>(volumes, inds);
	SwapElements<double>(incomePers, inds);
	SwapElements<double>(volumePers, inds);
	// обновляем map новыми индексами
	for (auto it = mapAssetIdToIndex.begin(); it != mapAssetIdToIndex.end(); it++) {
		it->second = inds[it->second];
	}
}

void CAssetsSummary::MakeCalculations() {
	if (!needsToMakeCalculations) return;
	double incomeSum = 0.0, amountSum = 0.0;
	for (uint i = 0; i < assetIds.size(); ++i) {
		incomeSum += incomes[i];
		amountSum += volumes[i];
	}
	for (uint i = 0; i < assetIds.size(); ++i) {
		incomePers[i] = incomes[i] / incomeSum;
		volumePers[i] = volumes[i] / amountSum;
	}
	needsToMakeCalculations = true;
}

CAssetsSummary::CAssetsSummary() {
}

CAssetsSummary::CAssetsSummary(int size) {
	assetIds.reserve(size);
	incomes.reserve(size);
	volumes.reserve(size);
	incomePers.reserve(size);
	volumePers.reserve(size);
}

std::vector<std::shared_ptr<CAssetsSummaryItem> > CAssetsSummary::GetTopItems(int amount, KPI kpi, bool isDecreasing) {
	std::vector<std::shared_ptr<CAssetsSummaryItem> > resContainer;
	resContainer.reserve(Size());
	if (needsToMakeCalculations) MakeCalculations();
	Sort(kpi, isDecreasing);
	for (int i = 0; i < amount; ++i) {
		resContainer.push_back(GetItemPtr(assetIds[i]));
	}
	return resContainer;
}

int CAssetsSummary::FindItem(int assetId) const {
	auto it = mapAssetIdToIndex.find(assetId);
	if (it != mapAssetIdToIndex.end()) {
		return it->second;
	} else {
		return -1;
	}
}

template<typename T>
void CAssetsSummary::SwapElements(std::vector<T>& vect, const std::vector<int>& inds) {
	const std::vector<T> tmpVect = vect;
	for (uint i = 0; i < inds.size(); ++i) {
		const int index = inds[i];
		vect[i] = tmpVect[index];
	}
}

template void CAssetsSummary::SwapElements(std::vector<int>& vect, const std::vector<int>& inds);
template void CAssetsSummary::SwapElements(std::vector<double>& vect, const std::vector<int>& inds);
