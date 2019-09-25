/*
 * IForcaster.cpp
 *
 *  Created on: Jun 21, 2019
 *      Author: kraychik
 */

#include "../forcast/IForcaster.h"

IForcaster::IForcaster() : salesHistory(nullptr), curPriceList(nullptr), params(nullptr), isTaught(false) {

}

IForcaster::IForcaster(shared_ptr<const CSalesHistory> salesHistory, shared_ptr<const CPriceList> curPriceList, shared_ptr<const COptimizerParams> params)
	: salesHistory(salesHistory), curPriceList(curPriceList), params(params), isTaught(false) {
}

IForcaster::~IForcaster() {
}

shared_ptr<const CSalesHistory> IForcaster::GetSalesHistory() const {
	return salesHistory;
}

shared_ptr<const CPriceList> IForcaster::GetPriceList() const {
	return curPriceList;
}

void IForcaster::SaveModel() const {
	return;
}

shared_ptr<const COptimizerParams> IForcaster::GetParams() const {
	return params;
}

/*
double IForcaster::ForcastSalesIndependent(int assetId, double newPrice, int periodDays, double confLevel) {
	map<int, double> newPrices;
	newPrices.insert(make_pair(assetId, newPrice));
	const auto exSalesPtr = ForcastSales(newPrices, periodDays, confLevel);
	return exSalesPtr->begin()->second;
}
*/
