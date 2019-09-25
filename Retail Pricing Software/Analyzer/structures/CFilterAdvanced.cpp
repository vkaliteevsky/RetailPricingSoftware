/*
 * CFilterAdvanced.cpp
 *
 *  Created on: Feb 23, 2019
 *      Author: kraychik
 */

#include "CFilterAdvanced.h"

CFilterAdvanced::CFilterAdvanced(const CFilter& filter, std::shared_ptr<const CGroupsInfo> groupsInfoPtr) : CFilter(filter)
{
	if (groupsInfoPtr == nullptr) {
		throw CSmartException("CFilterAdvanced::CFilterAdvanced: groupsInfoPtr is nullptr!\n");
	}
	this->assetIdsFromGroupsPtr = groupsInfoPtr->GetAssetsInGroups(groupIds);
}

std::shared_ptr<const std::set<int> > CFilterAdvanced::GetAssetIdsFromGroups() const {
	return assetIdsFromGroupsPtr;
}

std::string CFilterAdvanced::ToString() const {
	return std::string("CFilterAdvanced::ToString not implemented yet: ") + CFilter::ToString();
}

CFilterAdvanced::~CFilterAdvanced() {
	// TODO Auto-generated destructor stub
}

void CFilterAdvanced::FillUsedAssets(const std::set<int>& allAssets, std::vector<int>& usedAssets) const {
	usedAssets.clear();
	std::set<int> set1, set2;
	if (useSupplementAssets) {
		FillSupplementSet(assetIds, allAssets, set1);
	} else {
		set1.insert(assetIds.begin(), assetIds.end());
	}
	if (useSupplementGroups) {
		FillSupplementSet(*assetIdsFromGroupsPtr, allAssets, set2);
	} else {
		for (int assetId : *assetIdsFromGroupsPtr) {
			set2.insert(assetId);
		}
	}
	std::set<int> resultingSet;
	for (int assetId : set1) {
		if (set2.find(assetId) != set2.end()) resultingSet.insert(assetId);
	}
	for (int assetId : set2) {
		if (set1.find(assetId) != set1.end()) resultingSet.insert(assetId);
	}
	usedAssets.insert(usedAssets.begin(), resultingSet.begin(), resultingSet.end());
}
