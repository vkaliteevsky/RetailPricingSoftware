/*
 * CGroupsInfo.cpp
 *
 *  Created on: Feb 22, 2019
 *      Author: kraychik
 */

#include "CGroupsInfo.h"

CGroupsInfo::CGroupsInfo(const std::vector<int>& assetIds, const std::vector<int>& groupIds) {
	for (uint i = 0; i < assetIds.size(); ++i) {
		AddInfo(assetIds[i], groupIds[i]);
	}
}

void CGroupsInfo::AddInfo(int assetId, int groupId) {
	mapAssetToGroup.insert(std::pair<int, int>(assetId, groupId));
	auto it = mapGroupToAssets.find(groupId);
	if (it == mapGroupToAssets.end()) {
		// создаем новое множество (элемент)
		auto newSet = std::make_shared<std::set<int>>();
		newSet->insert(assetId);
		mapGroupToAssets.insert(std::pair<int, std::shared_ptr<std::set<int>>>(groupId, newSet));
	} else {
		// добавляем элемент к существующему множеству
		 it->second->insert(assetId);
	}
}

std::shared_ptr<const std::set<int> > CGroupsInfo::GetAssetsInGroup(int groupId) const {
	auto it = mapGroupToAssets.find(groupId);
	if (it == mapGroupToAssets.end()) {
		return nullptr;
	}
	return it->second;
}

int CGroupsInfo::GetGroupByAsset(int assetId) const {
	auto it = mapAssetToGroup.find(assetId);
	if (it == mapAssetToGroup.end()) {
		return -1;
	}
	return it->second;
}

void CGroupsInfo::Clear() {
	mapAssetToGroup.clear();
	mapGroupToAssets.clear();
}

void CGroupsInfo::FillAllGroups(std::vector<int>& groupIds) const {
	groupIds.clear();
	for (const auto & pr : mapGroupToAssets) {
		const int groupId = pr.first;
		groupIds.push_back(groupId);
	}
}

void CGroupsInfo::FillGroups(const std::set<int>& assetIdsFilter, std::vector<int>& groupIds) const {
	groupIds.clear();
	for (const int assetId : assetIdsFilter) {
		const int groupId = GetGroupByAsset(assetId);
		groupIds.push_back(groupId);
	}
}

void CGroupsInfo::FillGroups(const std::set<int>& assetIdsFilter, std::set<int>& groupIds) const {
	groupIds.clear();
	for (const int assetId : assetIdsFilter) {
		const int groupId = GetGroupByAsset(assetId);
		groupIds.insert(groupId);
	}
}

CGroupsInfo::~CGroupsInfo() {
	// TODO Auto-generated destructor stub
}

template<typename Container>
std::shared_ptr<std::set<int> > CGroupsInfo::GetAssetsInGroups(const Container& groupIds) const {
	auto resultingSet = std::make_shared<std::set<int>>();
	for (const int groupId : groupIds) {
		const auto mapIt = mapGroupToAssets.find(groupId);
		if (mapIt != mapGroupToAssets.end()) {
			resultingSet->insert(mapIt->second->begin(), mapIt->second->end());
		}
	}
	return resultingSet;
}

/*template<typename Container>
std::shared_ptr<Container> CGroupsInfo::GetAllGroups() const {
	auto resultingContainer = std::make_shared<Container>();
	for (const auto & mapItem : mapGroupToAssets) {
		resultingContainer->push_back(mapItem.first);
	}
	return resultingContainer;
}*/

std::shared_ptr<std::set<int>> CGroupsInfo::GetAllGroups() const {
	auto resultingContainer = std::make_shared<std::set<int>>();
	for (const auto & mapItem : mapGroupToAssets) {
		resultingContainer->insert(mapItem.first);
	}
	return resultingContainer;
}


template std::shared_ptr<std::set<int> > CGroupsInfo::GetAssetsInGroups(const std::vector<int> & groupIds) const;
template std::shared_ptr<std::set<int> > CGroupsInfo::GetAssetsInGroups(const std::set<int> & groupIds) const;
template std::shared_ptr<std::set<int> > CGroupsInfo::GetAssetsInGroups(const std::list<int> & groupIds) const;

//template std::shared_ptr<std::vector<int>> CGroupsInfo::GetAllGroups() const;
//template std::shared_ptr<std::list<int>> CGroupsInfo::GetAllGroups() const;

CGroupsInfo::CGroupsInfo() {
}

std::set<int> CGroupsInfo::GetGroupIds() const {
	std::set<int> groupIds;
	for (const auto pair : mapGroupToAssets) {
		groupIds.insert(pair.first);
	}
	return groupIds;
}
