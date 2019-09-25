/*
 * CRandomPriceOptimizer.cpp
 *
 *  Created on: Jun 20, 2019
 *      Author: kraychik
 */

#include "CRandomPriceOptimizer.h"

CRandomPriceOptimizer::CRandomPriceOptimizer() : IPriceOptimizer() {

}

CRandomPriceOptimizer::~CRandomPriceOptimizer() {
}

CRandomPriceOptimizer::CRandomPriceOptimizer(int companyId, shared_ptr<const CSalesHistory> salesHistory, shared_ptr<const CPriceList> curPriceList, shared_ptr<COptimizerParams> params)
	: IPriceOptimizer(companyId, salesHistory, curPriceList, params) {
}

shared_ptr<const CPriceList> CRandomPriceOptimizer::CalcOptimalPriceList(double monthSince, int periodDays, int amountOfDenseAssets) {
	const bool isDataOk = CheckDataBeforeCalc();
	if (!isDataOk) {
		optimalPrices = nullptr;
		return nullptr;
	}
	if (params == nullptr) {
		SetDefaultParams();
	}
	optimalPrices = curPriceList->CloneWithItems();

	auto priceItems = optimalPrices->GetPriceItems();
	for (const auto & mapPair : priceItems) {
		const int assetId = mapPair.first;
		const auto priceItemPtr = mapPair.second;
		//const double curPrice = priceItemPtr->price;
		const auto paramItemPtr = params->GetParamItem(assetId);
		const double newPrice = Random(ceil(paramItemPtr->minPrice), floor(paramItemPtr->maxPrice));
		priceItemPtr->SetPrice(newPrice);
	}
	return optimalPrices;
}
