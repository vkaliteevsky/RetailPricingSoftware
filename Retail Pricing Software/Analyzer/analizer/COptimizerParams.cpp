/*
 * COptimizerParams.cpp
 *
 *  Created on: Feb 8, 2019
 *      Author: kraychik
 */

#include "COptimizerParams.h"
#include "../structures/CGroupsInfo.h"
#include "../structures/CFilter.h"
#include "../structures/CPriceList.h"

COptimizerParams::~COptimizerParams() {
	// TODO Auto-generated destructor stub
}

void COptimizerParams::AddParamItem(int assetId, const shared_ptr<CParamsItem> paramItem) {
	params.insert(pair<int, shared_ptr<CParamsItem>>(assetId, paramItem));
	AddAssetIntoGroup(assetId, paramItem->groupId);
}

shared_ptr<CParamsItem> COptimizerParams::GetParamItem(int assetId) const {
	auto it = params.find(assetId);
	if (it == params.end()) {
		return nullptr;
	} else {
		return it->second;
	}
}

shared_ptr<CParamsItem> COptimizerParams::operator [](int assetId) const {
	return GetParamItem(assetId);
}

shared_ptr<set<int> > COptimizerParams::GetAssetsFromGroup(int groupId) const {
	auto it = assetsInGroup.find(groupId);
	if (it == assetsInGroup.end()) {
		return nullptr;
	} else {
		return it->second;
	}
}

void COptimizerParams::FillBoxConstraints(map<int, pair<double, double> >& boxes) const {
	boxes.clear();
	for (const auto & item : params) {
		const int assetId = item.first;
		const double a = item.second->minPrice;
		const double b = item.second->maxPrice;
		boxes.insert(make_pair(assetId, make_pair(a, b)));
	}
}

void COptimizerParams::FillBoxConstraints(const CFilter & filterAssets, vector<int>& assetIds, vector<double>& a, vector<double>& b) const {
	assetIds.clear(); a.clear(); b.clear();
	const int n = (int)filterAssets.assetIds.size();
	assetIds.resize(n); a.resize(n); b.resize(n);
	int i = 0;
	for (const auto & item : params) {
		const int assetId = item.first;
		if (filterAssets.assetIds.find(assetId) != filterAssets.assetIds.end()) {
			assetIds[i] = assetId;
			a[i] = item.second->minPrice;
			b[i] = item.second->maxPrice;
			i++;
		}
	}
}

void COptimizerParams::FillBoxConstraints(const vector<int>& assetIdsFilter, vector<double>& a, vector<double>& b) const {
	a.clear(); b.clear();
	const int n = (int)assetIdsFilter.size();
	a.resize(n); b.resize(n);
	for (int i = 0; i < n; ++i) {
		const int assetId = assetIdsFilter[i];
		const auto it = params.find(assetId);
		if (it == params.end()) {
			throw CSmartException("COptimizerParams::FillBoxConstraints: assetId was not found in params");
		}
		const auto paramsItem = it->second;
		a[i] = paramsItem->minPrice;
		b[i] = paramsItem->maxPrice;
	}
}

void COptimizerParams::ChangeBoxConstraintsForIndicators(const shared_ptr<const CPriceList> curPriceList) {
	for (auto pr : params) {
		const int assetId = pr.first;
		const auto priceItemPtr = curPriceList->GetPriceItem(assetId);
		if (priceItemPtr == nullptr) {
			throw CSmartException("COptimizerParams::FillAssetsToOptimize: cannot find assetId in curPriceList");
		}
		const double curPrice = priceItemPtr->price;
		const int roleId = pr.second->roleId;
		if (roleId == 2) {
			pr.second->minPrice = curPrice;
			pr.second->maxPrice = curPrice;
		}
	}
}

void COptimizerParams::FillAssetsToOptimize(set<int>& assetsToOptimize) const {
	assetsToOptimize.clear();
	for (auto pr : params) {
		const int assetId = pr.first;
		const int roleId = pr.second->roleId;
		if ((roleId == 1) || (roleId == 2)) {
			assetsToOptimize.insert(assetId);
		}
	}
}

void COptimizerParams::AddAssetIntoGroup(int assetId, int groupId) {
	const auto it = assetsInGroup.find(groupId);
	if (it == assetsInGroup.end()) {
		auto newSet = make_shared<set<int>>();
		newSet->insert(assetId);
		assetsInGroup[groupId] = newSet;
	} else {
		it->second->insert(assetId);
	}
}

void COptimizerParams::SplitByGroups(map<int, shared_ptr<COptimizerParams>> & resultingMap) const {
	resultingMap.clear();
	for (const auto & mapItem : assetsInGroup) {
		auto newOptimizerParams = make_shared<COptimizerParams>();
		for (const int assetId : *mapItem.second) {
			auto paramItem = GetParamItem(assetId);
			newOptimizerParams->AddParamItem(assetId, paramItem);
		}
		resultingMap.insert(pair<int, shared_ptr<COptimizerParams>>(mapItem.first, newOptimizerParams));
	}
}

COptimizerParams::COptimizerParams() {
}

shared_ptr<vector<int> > COptimizerParams::GetAllGroups() const {
	auto resultingVector = make_shared<vector<int>>();
	resultingVector->reserve(assetsInGroup.size());
	for (const auto & mapItem : assetsInGroup) {
		resultingVector->push_back(mapItem.first);
	}
	return resultingVector;
}

shared_ptr<COptimizerParams> COptimizerParams::Filter(set<int>& assetIds) const {
	auto newParams = make_shared<COptimizerParams>();
	for (const int assetId : assetIds) {
		auto it = params.find(assetId);
		if (it != params.end()) {
			newParams->AddParamItem(assetId, it->second);
		}
	}
	return newParams;
}

double COptimizerParams::MaxPrice() const {
	double maxPrice = -1.0;
	for (const auto & mapItem : params) {
		const double cp = mapItem.second->maxPrice;
		if (cp > maxPrice) maxPrice = cp;
	}
	return maxPrice;
}

double COptimizerParams::MinPrice() const {
	double minPrice = 100000000.0;
	for (const auto & mapItem : params) {
		const double cp = mapItem.second->minPrice;
		if (cp < minPrice) minPrice = cp;
	}
	return minPrice;
}

shared_ptr<CGroupsInfo> COptimizerParams::CalcGroupsInfo() const {
	vector<int> assetIds, groupIds;
	for (const auto pair : assetsInGroup) {
		const int groupId = pair.first;
		const auto setPtr = pair.second;
		for (const int assetId : *setPtr) {
			assetIds.push_back(assetId);
			groupIds.push_back(groupId);
		}
	}
	return make_shared<CGroupsInfo>(assetIds, groupIds);
}
