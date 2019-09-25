/*
 * COptimizerParams.h
 *
 *  Created on: Feb 8, 2019
 *      Author: kraychik
 */

#ifndef ANALIZER_COPTIMIZERPARAMS_H_
#define ANALIZER_COPTIMIZERPARAMS_H_

#include <vector>
#include <map>
#include <memory>
#include <set>

#include "CParamsItem.h"

using namespace std;

class CGroupsInfo;
class CFilter;
class CPriceList;
class COptimizerParams {
public:
	COptimizerParams();
	virtual ~COptimizerParams();
	void AddParamItem(int assetId, const shared_ptr<CParamsItem> paramItem);
	shared_ptr<CParamsItem> GetParamItem(int assetId) const;
	shared_ptr<set<int>> GetAssetsFromGroup(int groupId) const;
	shared_ptr<CParamsItem> operator[](int assetId) const;
	shared_ptr<vector<int>> GetAllGroups() const;
	shared_ptr<COptimizerParams> Filter(set<int> & assetIds) const;
	void ChangeBoxConstraintsForIndicators(const shared_ptr<const CPriceList> curPriceList);
	void FillAssetsToOptimize(set<int> & assetsToOptimize) const;
	double MaxPrice() const;
	double MinPrice() const;
	void SplitByGroups(map<int, shared_ptr<COptimizerParams>> & resultingMap) const;
	shared_ptr<CGroupsInfo> CalcGroupsInfo() const;
	void FillBoxConstraints(map<int, pair<double, double>> & boxes) const;
	void FillBoxConstraints(const vector<int> & assetIdsFilter, vector<double> & a, vector<double> & b) const;
	void FillBoxConstraints(const CFilter & filterAssets, vector<int> & assetIds, vector<double> & a, vector<double> & b) const;
private:
	void AddAssetIntoGroup(int assetId, int groupId);
	map<int, shared_ptr<CParamsItem>> params;  // сопоставляет assetId -> <параметры оптимизации>
	map<int, shared_ptr<set<int>>> assetsInGroup;  // сопоставляет groupId -> <множетсво assetId>
};

#endif /* ANALIZER_COPTIMIZERPARAMS_H_ */
