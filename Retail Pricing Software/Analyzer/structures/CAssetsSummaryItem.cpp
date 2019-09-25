/*
 * CAssetsSummaryItem.cpp
 *
 *  Created on: Feb 21, 2019
 *      Author: kraychik
 */

#include "CAssetsSummaryItem.h"

CAssetsSummaryItem::CAssetsSummaryItem(int assetId, double income, double volume, double incomePer, double amountPer)
	: assetId(assetId), income(income), volume(volume), incomePer(incomePer), amountPer(amountPer)
{
}

CAssetsSummaryItem::CAssetsSummaryItem() {

}

std::string CAssetsSummaryItem::ToJSON() const {
	std::vector<std::string> vect = {
			CJSONCreator::CreateKeyValueItem(std::string("assetId"), assetId),
			CJSONCreator::CreateKeyValueItem(std::string("income"), income),
			CJSONCreator::CreateKeyValueItem(std::string("volume"), volume),
			CJSONCreator::CreateKeyValueItem(std::string("incomePercent"), incomePer),
			CJSONCreator::CreateKeyValueItem(std::string("volumePercent"), amountPer)
	};
	std::string res = CJSONCreator::SeparateObjects(vect);
	return CJSONCreator::MakeObject(res);
}
