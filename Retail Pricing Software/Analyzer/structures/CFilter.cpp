/*
 * CFilter.cpp
 *
 *  Created on: Feb 6, 2019
 *      Author: kraychik
 */

#include "CFilter.h"

CFilter::CFilter() {
	// TODO Auto-generated constructor stub

}

CFilter::~CFilter() {
	// TODO Auto-generated destructor stub
}

CFilter::CFilter(std::vector<interval> & intervals, std::set<int>& assetIds,
		std::set<int>& groupIds, std::set<int>& shopIds,
		bool useSupplementIntervals, bool useSupplementAssets,
		bool useSupplementGroups, bool useSupplementShops)
		: intervals(intervals), assetIds(assetIds), groupIds(groupIds), shopIds(shopIds),
		  useSupplementIntervals(useSupplementIntervals), useSupplementAssets(useSupplementAssets), useSupplementGroups(useSupplementGroups), useSupplementShops(useSupplementShops)
{

}

bool CFilter::IsEmpty() const {
	return (intervals.empty() && assetIds.empty() && groupIds.empty() && shopIds.empty()
			&& useSupplementAssets && useSupplementGroups && useSupplementIntervals && useSupplementShops);
}

void CFilter::Clear() {
	intervals.clear();
	shopIds.clear();
	assetIds.clear();
	groupIds.clear();
	useSupplementAssets = true; useSupplementGroups = true; useSupplementIntervals = true; useSupplementShops = true;
}

std::string CFilter::ToString() const {
	std::string res = "CFilter:\n";
	res += std::string("Intervals (UseSupplement = ") + std::to_string(useSupplementIntervals) + "): ";
	for (const auto & interval : intervals) {
		res += std::string("[") + std::to_string(interval.first) + ", " + std::to_string(interval.second) + "], ";
	}
	res += std::string("\nAssets (UseSupplement = ") + std::to_string(useSupplementAssets) + "): ";
	for (const auto & asset : assetIds) {
		res += std::to_string(asset) + ", ";
	}
	res += std::string("\nGroups (UseSupplement = ") + std::to_string(useSupplementGroups) + "): ";
	for (const auto & asset : groupIds) {
		res += std::to_string(asset) + ", ";
	}
	res += std::string("\nShops (UseSupplement = ") + std::to_string(useSupplementShops) + "): ";
	for (const auto & shop : shopIds) {
		res += std::to_string(shop) + ", ";
	}
	res += "\n";
	return res;
}

FilterMode CFilter::IntervalsFilterMode() const {
	return CTypeConverter::BoolToFilterMode(useSupplementIntervals);
}

FilterMode CFilter::AssetsFilterMode() const {
	return CTypeConverter::BoolToFilterMode(useSupplementAssets);
}

FilterMode CFilter::GroupsFilterMode() const {
	return CTypeConverter::BoolToFilterMode(useSupplementGroups);
}

FilterMode CFilter::ShopsFilterMode() const {
	return CTypeConverter::BoolToFilterMode(useSupplementShops);
}

CFilter::CFilter(bool useAllOk) {
	std::vector<interval> emptyIntervals;
	std::set<int> emptySet;
	if (useAllOk) {   // все элементы включаются
		CFilter(emptyIntervals, emptySet, emptySet, emptySet, true, true, true, true);
	} else {
		CFilter(emptyIntervals, emptySet, emptySet, emptySet, false, false, false, false);
	}
}

void CFilter::FillSupplementSet(const std::set<int>& objectSet, const std::set<int>& allSet, std::set<int>& resultingSet) {
	resultingSet.clear();
	for (int objectId : allSet) {
		const auto it = objectSet.find(objectId);
		if (it == objectSet.end()) {
			resultingSet.insert(objectId);
		}
	}
}

void CFilter::SetAssetIds(const std::set<int>& assetIds, bool toExcludeAssets) {
	this->assetIds = assetIds;
	this->useSupplementAssets = toExcludeAssets;
}

void CFilter::SetGroupIds(const std::set<int>& groupIds, bool toExcludeGroups) {
	this->groupIds = groupIds;
	this->useSupplementGroups = toExcludeGroups;
}

void CFilter::SetAssetIds(const std::vector<int>& assetIds, bool toExcludeAssets) {
	this->assetIds.clear();
	this->assetIds.insert(assetIds.begin(), assetIds.end());
	this->useSupplementAssets = toExcludeAssets;
}
