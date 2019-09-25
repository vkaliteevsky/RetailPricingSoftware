/*
 * CPriceList.cpp
 *
 *  Created on: Feb 8, 2019
 *      Author: kraychik
 */

#include "CPriceList.h"
#include "../http/CJSONCreator.h"
#include "../analizer/COptimizerParams.h"

CPriceList::CPriceList() {
	// TODO Auto-generated constructor stub

}

CPriceList::~CPriceList() {
	// TODO Auto-generated destructor stub
}

void CPriceList::AddPriceItem(int assetId, std::shared_ptr<CPriceItem> item) {
	auto it = priceList.find(assetId);
	if (it == priceList.end()) {
		priceList.insert(std::pair<int, std::shared_ptr<CPriceItem>>(assetId, item));
	} else {
		// обновляем информацию в найденном PriceItem
		auto curItem = it->second;
		curItem->Init(item);
	}
}

void CPriceList::AddPriceItem(int assetId, double price, double cost) {
	auto item = std::make_shared<CPriceItem>(price, cost);
	AddPriceItem(assetId, item);
}

std::shared_ptr<CPriceItem> CPriceList::GetPriceItem(int assetId) const {
	auto it = priceList.find(assetId);
	return (it == priceList.end() ? nullptr : it->second);
}

double CPriceList::GetPrice(int assetId) const {
	auto priceItem = GetPriceItem(assetId);
	return (priceItem == nullptr ? -1.0 : priceItem->price);
}

double CPriceList::GetCost(int assetId) const {
	auto priceItem = GetPriceItem(assetId);
	return (priceItem == nullptr ? -1.0 : priceItem->cost);
}

void CPriceList::SetStartTime(time_t newStartTime) {
	this->startTime = newStartTime;
}

void CPriceList::SetEndTime(time_t newEndTime) {
	this->endTime = newEndTime;
}

inline time_t CPriceList::GetStartTime() const {
	return startTime;
}

inline time_t CPriceList::GetEndTime() const {
	return endTime;
}

CPriceList::CPriceList(const std::shared_ptr<CPriceList> priceList)
	: startTime(priceList->startTime), endTime(priceList->endTime), priceList(priceList->priceList)
{

}

std::shared_ptr<std::vector<int> > CPriceList::GetAllAssets() const {
	auto allAssets = std::make_shared<std::vector<int>>();
	auto allPriceItems = std::make_shared<std::vector<std::shared_ptr<CPriceItem>>>();
	//std::vector<int> vect = *allAssets;
	//std::vector<std::shared_ptr<CPriceItem>> items = *allPriceItems;
	FillVectors(*allAssets, *allPriceItems);
	return allAssets;
}

void CPriceList::FillVectors(std::vector<int>& assetIds, std::vector<std::shared_ptr<CPriceItem> >& priceItems) const {
	assetIds.clear();
	priceItems.clear();
	const int amountOfElements = priceList.size();
	assetIds.reserve(amountOfElements);
	priceItems.reserve(amountOfElements);
	for (const auto & mapItem : priceList) {
		assetIds.push_back(mapItem.first);
		priceItems.push_back(mapItem.second);
	}
}

void CPriceList::JoinPriceList(const std::shared_ptr<CPriceList> priceList) {
	std::vector<int> assetIds;
	std::vector<std::shared_ptr<CPriceItem>> priceItems;
	priceList->FillVectors(assetIds, priceItems);
	for (uint i = 0; i < assetIds.size(); ++i) {
		this->AddPriceItem(assetIds[i], priceItems[i]);
	}
}

std::pair<std::shared_ptr<CPriceList>, std::shared_ptr<CPriceList> > CPriceList::SplitPriceList(const std::set<int>& assetIds) const {
	std::vector<int> assetIdsInternal;
	std::vector<std::shared_ptr<CPriceItem>> priceItems;
	this->FillVectors(assetIdsInternal, priceItems);

	auto newPriceList1 = std::make_shared<CPriceList>();
	auto newPriceList2 = std::make_shared<CPriceList>();
	for (uint i = 0; i < assetIdsInternal.size(); ++i) {
		auto foundIt = assetIds.find(assetIdsInternal[i]);
		if (foundIt != assetIds.end()) {
			// добавляем элемент в первый список
			newPriceList1->AddPriceItem(assetIdsInternal[i], priceItems[i]);
		} else {
			// добавляем элемент во второй список
			newPriceList2->AddPriceItem(assetIdsInternal[i], priceItems[i]);
		}
	}
	return std::pair<std::shared_ptr<CPriceList>, std::shared_ptr<CPriceList>>(newPriceList1, newPriceList2);
}

void CPriceList::EraseItem(int assetId) {
	auto it = priceList.find(assetId);
	if (it != priceList.end()) {
		priceList.erase(it);
	}
}

std::shared_ptr<CPriceList> CPriceList::FilterPriceList(const std::set<int>& assetIds) const {
	std::shared_ptr<CPriceList> newPriceList = std::make_shared<CPriceList>();
	std::vector<int> assetIdsInternal;
	std::vector<std::shared_ptr<CPriceItem>> priceItems;
	this->FillVectors(assetIdsInternal, priceItems);
	for (uint i = 0; i < assetIdsInternal.size(); ++i) {
		if (assetIds.find(assetIdsInternal[i]) != assetIds.end()) {
			newPriceList->AddPriceItem(assetIdsInternal[i], priceItems[i]);
		}
	}
	return newPriceList;
}

std::shared_ptr<CPriceList> CPriceList::FilterPriceList(const std::shared_ptr<std::set<int> > assetIds) const {
	return FilterPriceList(*assetIds);
}

std::shared_ptr<CPriceList> CPriceList::Merge(std::shared_ptr<CPriceList> priceList1, std::shared_ptr<CPriceList> priceList2) {
	std::shared_ptr<CPriceList> joinedPriceList(priceList1);
	std::vector<int> assetIds;
	std::vector<std::shared_ptr<CPriceItem>> priceItems;
	priceList2->FillVectors(assetIds, priceItems);
	for (uint i = 0; i < assetIds.size(); ++i) {
		joinedPriceList->AddPriceItem(assetIds[i], priceItems[i]);
	}
	return joinedPriceList;
}

int CPriceList::Size() const {
	return priceList.size();
}

double CPriceList::MinPrice() const {
	double minPrice = 100000000.0;
	for (const auto & mapItem : priceList) {
		double price = mapItem.second->price;
		if (price < minPrice) minPrice = price;
	}
	return minPrice;
}

double CPriceList::MaxPrice() const {
	double maxPrice = -10000.0;
	for (const auto & mapItem : priceList) {
		double price = mapItem.second->price;
		if (price > maxPrice) maxPrice = price;
	}
	return maxPrice;
}

std::shared_ptr<CPriceList> CPriceList::CloneWithItems() const {
	auto newPriceList = std::make_shared<CPriceList>();
	for (const auto & mapItem : priceList) {
		newPriceList->AddPriceItem(mapItem.first, mapItem.second->price, mapItem.second->cost);
	}
	newPriceList->SetStartTime(startTime);
	newPriceList->SetEndTime(endTime);
	return newPriceList;
}

void CPriceList::UpdatePrices(const std::vector<int>& assetIds, const std::vector<double>& newPrices) {
	const uint n = assetIds.size();
	for (uint i = 0; i < n; ++i) {
		auto priceItem = GetPriceItem(assetIds[i]);
		priceItem->SetPrice(newPrices[i]);
	}
}

std::string CPriceList::ToString() const {
	std::string result("PriceList\n");
	for (const auto & mapItem : priceList) {
		result += std::to_string(mapItem.first) + ": " + mapItem.second->ToString();
	}
	return result;
}

CPriceList::CPriceList(const std::shared_ptr<std::list<int>> assetIds, const std::shared_ptr<std::list<CPriceItem>> items) {
	auto it1 = assetIds->begin();
	auto it2 = items->begin();
	while (it1 != assetIds->end()) {
		const int assetId = *it1;
		const double price = it2->price;
		const double cost = it2->cost;
		this->AddPriceItem(assetId, price, cost);
		it1++; it2++;
	}
}

void CPriceList::Clear() {
	priceList.clear();
	startTime = 0;
	endTime = 0;
}

const std::map<int, std::shared_ptr<CPriceItem> >& CPriceList::GetPriceItems() const {
	return priceList;
}

std::string CPriceList::ToJSON() const {
	std::vector<std::string> jsons;
	for (const auto & pairItem : priceList) {
		const int assetId = pairItem.first;
		const auto priceItemPtr = pairItem.second;
		const auto priceJSON = CJSONCreator::CreateKeyValueItem("price", priceItemPtr->GetPrice());
		const auto costJSON = CJSONCreator::CreateKeyValueItem("cost", priceItemPtr->GetCost());
		const auto priceCostJSON = CJSONCreator::SeparateObjects(priceJSON, costJSON);
		const auto assetJSON = CJSONCreator::CreateKeyValueItem("assetId", assetId);
		const auto item = CJSONCreator::SeparateObjects(assetJSON, priceCostJSON);
		jsons.push_back(CJSONCreator::MakeObject(item));
		//mapResult.insert(std::pair<int, std::string>(assetId, priceCost));
	}
	//const auto response = CJSONCreator::MakeKeyValueArray<int, std::string>(mapResult, std::string("assetId"), std::string("priceItem"));
	const auto response = CJSONCreator::MakeArray(jsons);
	return response;
}

std::string CPriceList::ToJSON(const std::map<int, bool> & mapIsAssetIndicator) const {
	std::vector<std::string> jsons;
	for (const auto & pairItem : priceList) {
		const int assetId = pairItem.first;
		const auto priceItemPtr = pairItem.second;
		const auto priceJSON = CJSONCreator::CreateKeyValueItem("price", priceItemPtr->GetPrice());
		const auto costJSON = CJSONCreator::CreateKeyValueItem("cost", priceItemPtr->GetCost());
		const auto priceCostJSON = CJSONCreator::SeparateObjects(priceJSON, costJSON);
		const auto assetJSON = CJSONCreator::CreateKeyValueItem("assetId", assetId);
		auto item = CJSONCreator::SeparateObjects(assetJSON, priceCostJSON);
		const auto it = mapIsAssetIndicator.find(assetId);
		bool isIndicator;
		if (it != mapIsAssetIndicator.end()) {
			isIndicator = it->second;
		} else {
			isIndicator = false;
		}
		const auto indicatorJSON = CJSONCreator::CreateKeyValueItem("isIndicator", isIndicator);
		item = CJSONCreator::SeparateObjects(item, indicatorJSON);
		jsons.push_back(CJSONCreator::MakeObject(item));
		//mapResult.insert(std::pair<int, std::string>(assetId, priceCost));
	}
	//const auto response = CJSONCreator::MakeKeyValueArray<int, std::string>(mapResult, std::string("assetId"), std::string("priceItem"));
	const auto response = CJSONCreator::MakeArray(jsons);
	return response;
}

void CPriceList::SetPrice(int assetId, double price) {
	auto priceItem = this->GetPriceItem(assetId);
	if (priceItem == nullptr) {
		this->AddPriceItem(assetId, price, 0.0);
	} else {
		priceItem->SetPrice(price);
	}
}

std::string CPriceList::ToJSON(std::shared_ptr<const COptimizerParams> params) const {
	std::vector<std::string> jsons;
	for (const auto & pairItem : priceList) {
		int assetId = pairItem.first;
		const auto priceItemPtr = pairItem.second;
		const auto priceJSON = CJSONCreator::CreateKeyValueItem("price", priceItemPtr->GetPrice());
		const auto costJSON = CJSONCreator::CreateKeyValueItem("cost", priceItemPtr->GetCost());
		const auto priceCostJSON = CJSONCreator::SeparateObjects(priceJSON, costJSON);
		const auto assetJSON = CJSONCreator::CreateKeyValueItem("assetId", assetId);
		auto item = CJSONCreator::SeparateObjects(assetJSON, priceCostJSON);
		const auto priceItem = params->GetParamItem(assetId);
		const bool isIndicator = (priceItem == nullptr) ? false : priceItem->roleId;
		const auto indicatorJSON = CJSONCreator::CreateKeyValueItem("isIndicator", isIndicator);
		item = CJSONCreator::SeparateObjects(item, indicatorJSON);
		const int groupId = priceItem->groupId;
		const auto groupJSON = CJSONCreator::CreateKeyValueItem("groupId", groupId);
		item = CJSONCreator::SeparateObjects(item, groupJSON);
		const double minPrice = priceItem->minPrice;
		const auto minPriceJSON = CJSONCreator::CreateKeyValueItem("minPrice", minPrice);
		item = CJSONCreator::SeparateObjects(item, minPriceJSON);
		const double maxPrice = priceItem->maxPrice;
		const auto maxPriceJSON = CJSONCreator::CreateKeyValueItem("maxPrice", maxPrice);
		item = CJSONCreator::SeparateObjects(item, maxPriceJSON);
		jsons.push_back(CJSONCreator::MakeObject(item));
	}
	const auto response = CJSONCreator::MakeArray(jsons);
	return response;
}

CPriceList::CPriceList(const std::vector<int>& assetIds, const std::vector<double>& prices, const std::vector<double>& costs) {
	const int n = (int)assetIds.size();
	for (int i = 0; i < n; ++i) {
		this->AddPriceItem(assetIds[i], prices[i], costs[i]);
	}
}

std::shared_ptr<std::map<int, double> > CPriceList::GetPricesMap() const {
	auto mapResult = std::make_shared<std::map<int, double>>();
	for (const auto & pair : priceList) {
		const int assetId = pair.first;
		const auto priceItem = pair.second;
		mapResult->insert(std::pair<int, double>(assetId, priceItem->price));
	}
	return mapResult;
}
