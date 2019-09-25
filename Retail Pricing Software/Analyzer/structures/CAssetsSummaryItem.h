/*
 * CAssetsSummaryItem.h
 *
 *  Created on: Feb 21, 2019
 *      Author: kraychik
 */

#ifndef STRUCTURES_CASSETSSUMMARYITEM_H_
#define STRUCTURES_CASSETSSUMMARYITEM_H_

#include <string>
#include "../http/CJSONCreator.h"

class CAssetsSummaryItem {
public:
	CAssetsSummaryItem();
	CAssetsSummaryItem(int assetId, double income, double volume, double incomePer, double amountPer);
	std::string ToJSON() const;

	int assetId = 0;
	double income = 0.0;
	double volume = 0.0;
	double incomePer = 0.0;
	double amountPer = 0.0;
};

#endif /* STRUCTURES_CASSETSSUMMARYITEM_H_ */
