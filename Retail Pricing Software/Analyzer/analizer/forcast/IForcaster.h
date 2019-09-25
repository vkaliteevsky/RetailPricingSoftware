/*
 * IForcaster.h
 *
 *  Created on: Jun 21, 2019
 *      Author: kraychik
 */

#ifndef ANALIZER_FORCAST_IFORCASTER_H_
#define ANALIZER_FORCAST_IFORCASTER_H_

#include "../../structures/CSalesHistory.h"
#include "../../structures/CPriceList.h"
#include "../COptimizerParams.h"
#include <vector>
#include <map>

using namespace std;

class IForcaster {
public:
	IForcaster();
	IForcaster(shared_ptr<const CSalesHistory> salesHistory, shared_ptr<const CPriceList> curPriceList, shared_ptr<const COptimizerParams> params);
	virtual ~IForcaster();

	virtual void TeachModel(const set<int> & assetIds, int periodDays, int amountOfDenseAssets) = 0;
	virtual double ForcastSalesIndependent(int assetId, double newPrice, double month, int periodDays) = 0;
	virtual shared_ptr<map<int, double>> ForcastSales(map<int, double> & newPrices, double monthSince, int periodDays) = 0;
	virtual void SaveModel() const;

	shared_ptr<const CSalesHistory> GetSalesHistory() const;
	shared_ptr<const CPriceList> GetPriceList() const;
	shared_ptr<const COptimizerParams> GetParams() const;

protected:
	shared_ptr<const CSalesHistory> salesHistory;
	shared_ptr<const CPriceList> curPriceList;
	shared_ptr<const COptimizerParams> params;
	bool isTaught;
};

#endif /* ANALIZER_FORCAST_IFORCASTER_H_ */
