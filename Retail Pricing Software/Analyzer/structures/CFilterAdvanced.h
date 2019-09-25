/*
 * CFilterAdvanced.h
 *
 *  Created on: Feb 23, 2019
 *      Author: kraychik
 */

#ifndef STRUCTURES_CFILTERADVANCED_H_
#define STRUCTURES_CFILTERADVANCED_H_

#include "CFilter.h"
#include "CGroupsInfo.h"

class CFilterAdvanced: public CFilter {
public:
	CFilterAdvanced(const CFilter & filter, std::shared_ptr<const CGroupsInfo> groupsInfoPtr);

	void FillUsedAssets(const std::set<int> & allAssets, std::vector<int> & usedAssets) const;
	std::shared_ptr<const std::set<int>> GetAssetIdsFromGroups() const;
	virtual std::string ToString() const;
	virtual ~CFilterAdvanced();
private:
	std::shared_ptr<const std::set<int>> assetIdsFromGroupsPtr = nullptr;
};

#endif /* STRUCTURES_CFILTERADVANCED_H_ */
