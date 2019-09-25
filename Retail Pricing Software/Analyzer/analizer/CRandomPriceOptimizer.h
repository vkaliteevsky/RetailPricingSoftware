/*
 * CRandomPriceOptimizer.h
 *
 *  Created on: Jun 20, 2019
 *      Author: kraychik
 */

#ifndef ANALIZER_CRANDOMPRICEOPTIMIZER_H_
#define ANALIZER_CRANDOMPRICEOPTIMIZER_H_

#include "IPriceOptimizer.h"

class CRandomPriceOptimizer: public IPriceOptimizer {
public:
	CRandomPriceOptimizer();
	CRandomPriceOptimizer(int companyId, shared_ptr<const CSalesHistory> salesHistory, shared_ptr<const CPriceList> curPriceList, shared_ptr<COptimizerParams> params);
	virtual ~CRandomPriceOptimizer();
	virtual shared_ptr<const CPriceList> CalcOptimalPriceList(double monthSince, int periodDays, int amountOfDenseAssets) override;
};

#endif /* ANALIZER_CRANDOMPRICEOPTIMIZER_H_ */
