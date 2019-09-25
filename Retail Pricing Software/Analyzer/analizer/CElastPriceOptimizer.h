/*
 * CElastPriceOptimizer.h
 *
 *  Created on: Jun 21, 2019
 *      Author: kraychik
 */

#ifndef ANALIZER_CELASTPRICEOPTIMIZER_H_
#define ANALIZER_CELASTPRICEOPTIMIZER_H_

#include "IPriceOptimizer.h"

class CElastPriceOptimizer: public IPriceOptimizer {
public:
	CElastPriceOptimizer();
	CElastPriceOptimizer(int companyId, shared_ptr<const CSalesHistory> salesHistory, shared_ptr<const CPriceList> curPriceList, shared_ptr<COptimizerParams> params, shared_ptr<IForcaster> forcaster);
	virtual shared_ptr<const CPriceList> CalcOptimalPriceList(double monthSince, int periodDays, int amountOfDenseAssets) override;
	virtual void NotActive() const override;
	virtual ~CElastPriceOptimizer();

protected:
	shared_ptr<CPriceList> OptimizeDenseAssets(const set<int> & denseAssetIds, double monthSince, int periodDays, int iters) const;
	shared_ptr<CPriceList> OptimizeSparseAssets(const set<int> & sparseAssetIds, double monthSince, int periodDays, int iters) const;
	void FillAssetsToOptimize(shared_ptr<const CPriceList> curPriceList, shared_ptr<const COptimizerParams> params, set<int> & assetsToOptimize) const;
};

#endif /* ANALIZER_CELASTPRICEOPTIMIZER_H_ */
