/*
 * IPriceOptimizer.h
 *
 *  Created on: Jun 20, 2019
 *      Author: kraychik
 */

#ifndef ANALIZER_IPRICEOPTIMIZER_H_
#define ANALIZER_IPRICEOPTIMIZER_H_

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
#include "forcast/IForcaster.h"
#include "algs/CAlgorithms.h"

class IPriceOptimizer {
public:
	IPriceOptimizer();
	IPriceOptimizer(int companyId, shared_ptr<const CSalesHistory> salesHistory, shared_ptr<const CPriceList> curPriceList, shared_ptr<COptimizerParams> params);
	virtual shared_ptr<const CPriceList> CalcOptimalPriceList(double monthSince, int periodDays, int amountOfDenseAssets);
	virtual void NotActive() const = 0;
	virtual ~IPriceOptimizer();

	void SetSalesHistory(shared_ptr<CSalesHistory> salesHistory);
	void SetCurrentPriceList(shared_ptr<CPriceList> priceList);
	void SetParams(shared_ptr<COptimizerParams> params);

	shared_ptr<const CPriceList> GetOptimalPrices() const;
	shared_ptr<const CSalesHistory> GetSalesHistory() const;
	shared_ptr<const CPriceList> GetCurrentPrices() const;
	shared_ptr<const COptimizerParams> GetParams() const;
	shared_ptr<IForcaster> GetForcaster() const;

	static int Random(int from, int to);
	static void GenRandomVector(vector<int> & arr);
	static double GenRandValue(const vector<double> & values, const vector<double> & probs);
protected:
	virtual shared_ptr<CPriceList> CalcOptimalPriceListForGroup(int groupId, int periodDays) const;
	bool CheckDataBeforeCalc() const;
	void CheckAndSetParams();
	void SetDefaultParams();
	shared_ptr<const CSalesHistory> salesHistory;
	shared_ptr<const CPriceList> curPriceList;
	shared_ptr<COptimizerParams> params;
	shared_ptr<CPriceList> optimalPrices;
	virtual double DefaultSpread() const;
	shared_ptr<IForcaster> forcaster;
	int companyId;
private:
	shared_ptr<map<int, double>> CalcRandomPrices() const;
	shared_ptr<map<int, double>> CalcRandomPrices(shared_ptr<std::set<int>> assetIds) const;
	double EvaluateOutcome(map<int, double> & sales, map<int, double> & prices) const;
	int ChooseBestOutcome(const vector<double> & incomes) const;
};

#endif /* ANALIZER_IPRICEOPTIMIZER_H_ */
