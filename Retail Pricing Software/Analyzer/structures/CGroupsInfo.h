/*
 * CGroupsInfo.h
 *
 *  Created on: Feb 22, 2019
 *      Author: kraychik
 */

#ifndef STRUCTURES_CGROUPSINFO_H_
#define STRUCTURES_CGROUPSINFO_H_

#include <map>
#include <vector>
#include <list>
#include <set>
#include <memory>
#include <string>

class CGroupsInfo {
public:
	CGroupsInfo();
	CGroupsInfo(const std::vector<int> & assetIds, const std::vector<int> & groupIds);
	void AddInfo(int assetId, int groupId);
	std::shared_ptr<const std::set<int>> GetAssetsInGroup(int groupId) const;
	int GetGroupByAsset(int assetId) const;
	void Clear();
	std::set<int> GetGroupIds() const;

	std::shared_ptr<std::set<int>> GetAllGroups() const;
	void FillAllGroups(std::vector<int> & groupIds) const;
	void FillGroups(const std::set<int> & assetIdsFilter, std::vector<int> & groupIds) const;
	void FillGroups(const std::set<int> & assetIdsFilter, std::set<int> & groupIds) const;

	template <typename Container>
	std::shared_ptr<std::set<int>> GetAssetsInGroups(const Container & groupIds) const;

	virtual ~CGroupsInfo();
private:
	std::map<int, int> mapAssetToGroup;
	std::map<int, std::shared_ptr<std::set<int>>> mapGroupToAssets;
};

#endif /* STRUCTURES_CGROUPSINFO_H_ */
