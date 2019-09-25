/*
 * CAnalyzer.h
 *
 *  Created on: Feb 8, 2019
 *      Author: kraychik
 */

#ifndef ANALIZER_COPTIMIZER_H_
#define ANALIZER_COPTIMIZER_H_
#include <memory>
#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include <numeric>
#include "../structures/CSalesHistory.h"
#include "../structures/CPriceList.h"
#include "../structures/CSupplementMatrix.h"
#include "CParamsItem.h"
#include "COptimizerParams.h"
#include "CPriceGrid.h"
#include "../supply/CDataBase.h"

using namespace std;

class COptimizer {
public:
	COptimizer(shared_ptr<CSalesHistory> salesHistory, shared_ptr<CPriceList> curPriceList);
	COptimizer(int projectId);
	shared_ptr<CPriceList> CalcExperimentalPriceList(const COptimizerParams & params) const;
	shared_ptr<CPriceList> CalcExperimentalPriceListInGroup(shared_ptr<set<int>> assetIds, shared_ptr<COptimizerParams> params) const;
	shared_ptr<const vector<int>> DetermineIndicatorAssets();
	shared_ptr<const CSupplementMatrix> CalcSupplementMatrix();
	virtual ~COptimizer();
private:
	static int Random(int from, int to);
	static void GenRandomVector(vector<int> & arr);
	static double GenRandValue(const vector<double> & values, const vector<double> & probs);
	shared_ptr<CSalesHistory> salesHistory = nullptr;
	shared_ptr<CPriceList> curPriceList = nullptr;
	shared_ptr<CSupplementMatrix> supplementMatrix = nullptr;
	shared_ptr<vector<int>> indicatorAssets = nullptr;
};

#endif /* ANALIZER_COPTIMIZER_H_ */
