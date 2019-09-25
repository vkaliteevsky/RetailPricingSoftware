#include "CCheck.h"
#include "CCheckItem.h"
#include <algorithm>

CCheck::CCheck() {
    //checkStructure = new std::list<CCheckItem *>();
    //checkStructure = std::list<std::shared_ptr<CCheckItem>>();
	//income = 0.0;
	//volume = 0.0;
}
CCheck::CCheck(int checkId, time_t checkTime, int shopId) : CCheck() {
    this->checkId = checkId;
	this->checkTime = checkTime;
	this->shopId = shopId;
}

CCheck::CCheck(int checkId, time_t checkTime, int shopId, std::list<std::shared_ptr<CCheckItem>>& checkStructure) : CCheck(checkId, checkTime, shopId)
{
	this->checkStructure = checkStructure;
}

CCheck::~CCheck() {
    
}

void CCheck::AddLine(int assetId, double price, double volume, double discount) {
	std::shared_ptr<CCheckItem> checkItem;
	checkItem = FindItemToAdd(assetId, price, volume, discount);
	if (!checkItem) {
		checkItem = std::make_shared<CCheckItem>(CCheckItem(assetId, price, volume, discount));
		this->checkStructure.push_back(checkItem);
	}
	else {
		checkItem->AddInfo(assetId, price, volume, discount);
	}
}

double CCheck::Income() const {
    double s = 0.0;
    for (auto it = checkStructure.begin(); it != checkStructure.end(); it++) {
        s += ((*it)->price - (*it)->discount) * (*it)->volume;
    }
    return (s);
}

double CCheck::Income(const std::set<int> &assetIds1, FilterMode assetFilter1, const std::set<int> &assetIds2, FilterMode assetFilter2) const
{
	double result = 0.0;
	for (const auto &item : checkStructure) {
		auto assetId = item->assetId;
		bool inAssetIds1 = (assetIds1.find(assetId) != assetIds1.end());
		bool inAssetIds2 = (assetIds2.find(assetId) != assetIds2.end());
		if (assetFilter1 == FilterMode::UseSupplementSet) inAssetIds1 = !inAssetIds1;
		if (assetFilter2 == FilterMode::UseSupplementSet) inAssetIds2 = !inAssetIds2;
		if (inAssetIds1 && inAssetIds2) {
			result += (item->price - item->discount) * item->volume;
		}
	}
	return result;
}

double CCheck::Income(const std::set<int>& assetIds, FilterMode filterMode) const
{
	return Income(assetIds, filterMode, std::set<int>(), FilterMode::UseSupplementSet);
}

bool CCheck::IsInInterval(time_t from, time_t to, FilterMode filterMode) const
{
	bool result = false;
	if ((to == nofilter) && (from == nofilter)) result = true;
	else if (to != nofilter && from == nofilter) result = (checkTime >= to);
	else if (to == nofilter && from != nofilter) result = (checkTime <= from);
	else result = (checkTime >= from) && (checkTime <= to);
	if (filterMode == FilterMode::UseSupplementSet) result = !result;
	return result;
}

bool CCheck::IsInShops(const std::set<int>& shopIds, FilterMode filterMode) const
{
	bool isFound = (shopIds.find(this->shopId) != shopIds.end());
	if (filterMode == FilterMode::UseSupplementSet) isFound = !isFound;
	return isFound;
}

double CCheck::Volume() const {
    double s = 0.0;
    for (auto it = checkStructure.begin(); it != checkStructure.end(); it++) {
        s += (*it)->volume;
    }
    return (s);
}

int CCheck::Size() const
{
	return checkStructure.size();
}

std::string CCheck::ToString() const
{
	std::string res = ("CheckId: " + std::to_string(checkId));
	res += "; ShopId: ";
	res += std::to_string(shopId);
	res += "\n";
	for (auto it = checkStructure.begin(); it != checkStructure.end(); it++) {
		res += (*it)->ToString();
	}
	return (res);
	return res;
}

const std::list<std::shared_ptr<CCheckItem>> &CCheck::GetCheckStructure()
{
	return checkStructure;
}

void CCheck::DeleteCheckItems(int assetId, bool deleteEqual)
{
	std::list<std::shared_ptr<CCheckItem>>::iterator it = checkStructure.begin();
	while (it != checkStructure.end()) {
		bool toDeleteItem = ((*it)->assetId == assetId);
		if (!deleteEqual) toDeleteItem = !toDeleteItem;
		if (toDeleteItem) {
			it = checkStructure.erase(it);
		}
		else {
			++it;
		}
	}
}

void CCheck::DeleteCheckItems(std::set<int>& assetIds, bool deleteEqual)
{
	for (auto it = assetIds.begin(); it != assetIds.end(); it++) {
		auto assetId = *it;
		DeleteCheckItems(assetId, deleteEqual);
	}
}

std::shared_ptr<CCheck> CCheck::Clone()
{
	return std::make_shared<CCheck>(checkId, checkTime, shopId, checkStructure);
}

std::shared_ptr<CCheckItem> CCheck::FindItemToAdd(int assetId, double price, double volume, double discount)
{
	for (auto it = checkStructure.begin(); it != checkStructure.end(); it++) {
		if ((assetId == (*it)->assetId) && (abs((*it)->price - price) <= 0.001)) {
			return (*it);
		}
	}
	return std::shared_ptr<CCheckItem>(nullptr);
}

std::unique_ptr<std::map<int, double> > CCheck::GetSoldAmounts() const {
	auto resultingMap = std::make_unique<std::map<int, double>>();
	for (const auto & item : checkStructure) {
		resultingMap->insert(std::pair<int, double>(item->assetId, item->volume));
	}
	return resultingMap;
}

double CCheck::AssetVolume(int assetId) const {
	double result = 0.0;
	for (const auto item : checkStructure) {
		result += (item->assetId == assetId ? item->volume : 0.0);
	}
	return result;
}

bool CCheck::HasAsset(int assetId) const {
	return AssetVolume(assetId) <= 0.00001;
}

int CCheck::FindInterval(const CIntervalSeries& intervals, FilterMode filterMode) const {
	for (int i = 0; i < (int)intervals.size(); ++i) {
		auto interval = intervals.at(i);
		bool isInInterval = this->IsInInterval(interval.first, interval.second, filterMode);
		if (isInInterval) {
			return i;
		}
	}
	return -1;
}

bool CCheck::IsInIntervals(const CIntervalSeries& intervals, FilterMode filterMode) const {
	if (intervals.size() == 0) {
		if (filterMode == FilterMode::Standard) return false;
		else return true;
	}
	return FindInterval(intervals, filterMode) >= 0;
}
void CCheck::FilterItems(const CFilterAdvanced & filter, std::list<std::shared_ptr<const CCheckItem>> & resultingList) const {
	resultingList.clear();
	FilterMode filterIntervals = CTypeConverter::BoolToFilterMode(filter.useSupplementIntervals);
	FilterMode filterShops = CTypeConverter::BoolToFilterMode(filter.useSupplementShops);
	FilterMode filterAssets = CTypeConverter::BoolToFilterMode(filter.useSupplementAssets);
	FilterMode filterGroups = CTypeConverter::BoolToFilterMode(filter.useSupplementGroups);
	bool inIntervals = IsInIntervals(filter.intervals, filterIntervals);
	if (filter.intervals.size() == 0) {
		inIntervals = filter.useSupplementIntervals;
	}
	bool inShops = IsInShops(filter.shopIds, filterShops);
	if (!(inIntervals && inShops)) {
		// чек не попадает в фильтр по времени и магазину. Возвращает пустой список
		return;
	}
	auto assetIdsFromGroupsPtr = filter.GetAssetIdsFromGroups();
	if (assetIdsFromGroupsPtr == nullptr) {
		throw CSmartException("CCheck::FilterItems: CFilterAdvanced returned nullptr\n");
	}
	for (const auto & item : checkStructure) {
		bool inAssets = (filter.assetIds.find(item->assetId) != filter.assetIds.end());
		inAssets = (filterAssets == FilterMode::Standard ? inAssets : !inAssets);
		bool inAssetsFromGroups = (assetIdsFromGroupsPtr->find(item->assetId) != assetIdsFromGroupsPtr->end());
		inAssetsFromGroups = (filterGroups == FilterMode::Standard ? inAssetsFromGroups : !inAssetsFromGroups);
		if (inAssets && inAssetsFromGroups) {
			resultingList.push_back(item);
		}
	}
}

CDate CCheck::GetDate() const {
	return CDate(checkTime);
}

int CCheck::Day() const {
	return GetDate().Day();
}

int CCheck::Month() const {
	return GetDate().Month();
}

int CCheck::Year() const {
	return GetDate().Year();
}
