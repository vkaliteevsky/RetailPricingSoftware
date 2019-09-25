/*
 * CFilter.h
 *
 *  Created on: Feb 6, 2019
 *      Author: kraychik
 */

#ifndef STRUCTURES_CFILTER_H_
#define STRUCTURES_CFILTER_H_

#include <vector>
#include <set>
#include <string>
#include "Structures.h"
#include "CIntervalSeries.h"
#include "../supply/CTypeConverter.h"

class CFilter {
public:
	CFilter();
	CFilter(std::vector<interval> & intervals, std::set<int> & assetIds, std::set<int> & groupIds, std::set<int> & shopIds, bool useSupplementIntervals = true, bool useSupplementAssets = true, bool useSupplementGroups = true, bool useSupplementShops = true);
	CFilter(bool useAllOk);
	FilterMode IntervalsFilterMode() const;
	FilterMode AssetsFilterMode() const;
	FilterMode GroupsFilterMode() const;
	FilterMode ShopsFilterMode() const;
	void SetAssetIds(const std::set<int> & assetIds, bool toExcludeAssets);
	void SetAssetIds(const std::vector<int> & assetIds, bool toExcludeAssets);
	void SetGroupIds(const std::set<int> & groupIds, bool toExcludeGroups);
	virtual ~CFilter();
	static void FillSupplementSet(const std::set<int> & objectSet, const std::set<int> & allSet, std::set<int> & resultingSet);
	bool IsEmpty() const;
	virtual void Clear();
	virtual std::string ToString() const;
	CIntervalSeries intervals;
	std::set<int> assetIds;
	std::set<int> groupIds;
	std::set<int> shopIds;
	bool useSupplementIntervals = true;
	bool useSupplementAssets = true;
	bool useSupplementGroups = true;
	bool useSupplementShops = true;
};

#endif /* STRUCTURES_CFILTER_H_ */
