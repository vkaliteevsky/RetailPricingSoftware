#include "CSalesHistory.h"
#include "CPriceList.h"

CSalesHistory::CSalesHistory() {
	groupsInfoPtr = make_shared<CGroupsInfo>();
}

CSalesHistory::~CSalesHistory() {
}

void CSalesHistory::AddCheck(shared_ptr<CCheck> check) {
	unique_key key(check->checkId, check->checkTime);
	auto foundItem = checks_map.find(key);
	if (foundItem == checks_map.end()) {
		this->checks.push_back(check);
		this->checks_map.insert(std::pair<unique_key, shared_ptr<CCheck>>(key, check));
	}
	else {
		throw CSmartException(0, "Not implemented", 0);
	}
}

void CSalesHistory::AddLine(int checkId, time_t checkTime, int shopId, int assetId, double price, double volume, double discount)
{
	assetsAll.insert(assetId);
	unique_key key(checkId, checkTime);
	auto foundItem = checks_map.find(key);
	if (foundItem == checks_map.end()) {
		auto check = make_shared<CCheck>(CCheck(checkId, checkTime, shopId));
		check->AddLine(assetId, price, volume, discount);
		this->checks.push_back(check);
		this->checks_map.insert(std::pair<unique_key, shared_ptr<CCheck>>(key, check));
	}
	else {
		shared_ptr<CCheck> check = foundItem->second;
		check->AddLine(assetId, price, volume, discount);
	}
}

void CSalesHistory::DeleteCheck(unique_key checkKey)
{
	for (auto it = checks.begin(); it != checks.end(); it++) {
		auto check = *it;
		if ((check->checkId == checkKey.first) && (check->checkTime == checkKey.second)) {
			RemoveCheckFromInternalStructure(it);
			return;
		}
	}
}

double CSalesHistory::Income() const {
	double income = 0.0;
	for (const auto & check : checks) {
		income += check->Income();
	}
	return income;
}

double CSalesHistory::Volume() const {
	double income = 0.0;
	for (const auto & check : checks) {
		income += check->Volume();
	}
	return income;
}

std::string CSalesHistory::ToString() const
{
	std::string res = "";
	for (auto it = checks.begin(); it != checks.end(); it++) {
		res += (*it)->ToString();
	}
	return res;
}

shared_ptr<set<int>> CSalesHistory::GetAssetsFromGroup(const set<int> &groupIds) const
{
	return groupsInfoPtr->GetAssetsInGroups(groupIds);
}

int CSalesHistory::GetGroupId(int assetId) const
{
	return groupsInfoPtr->GetGroupByAsset(assetId);
}

void CSalesHistory::Clear()
{
	checks_map.clear();
	checks.clear();
	groupsInfoPtr = nullptr;
	assetsAll.clear();
}

void CSalesHistory::AddInfoToCheck(shared_ptr<CCheck> check, int assetId, double price, double volume, double discount)
{
	check->AddLine(assetId, price, volume, discount);
}

void CSalesHistory::AddLine(const CHistoryItem& item) {
	AddLine(item.checkId, item.checkTime, item.shopId, item.assetId, item.price, item.volume, item.discount);
}

void CSalesHistory::CalculateCheckStructure(const CFilter& filter, CStructureMap<int, double>& structureMap, int& amountOfChecks) const {
	CFilterAdvanced advancedFilter(filter, groupsInfoPtr);
	structureMap.Clear();
	amountOfChecks = 0;
	for (const auto & checkPtr : checks) {
		list<shared_ptr<const CCheckItem>> checkItems;
		checkPtr->FilterItems(advancedFilter, checkItems);
		for (const auto & checkItemPtr : checkItems) {
			const int assetId = checkItemPtr->assetId;
			const double volume = checkItemPtr->volume;
			auto it = structureMap.structure.find(assetId);
			if (it == structureMap.structure.end()) {
				structureMap.AddItem(assetId, volume);
			} else {
				it->second += volume;
			}
		}
		if (checkItems.size() > 0) amountOfChecks++;
	}
	for (auto & mapItem : structureMap.structure) {
		mapItem.second /= amountOfChecks;
	}
}

int CSalesHistory::AmountOfAssets() const {
	return (int)assetsAll.size();
}

shared_ptr<CSupplementMatrix> CSalesHistory::BuildSupplementMatrix(const CFilterAdvanced & filter) const {
	//CFilterAdvanced advancedFilter(filter, groupsInfoPtr);
	//vector<int> assetIds(assetsAll.size());
	//std::copy(assetsAll.begin(), assetsAll.end(), assetIds.begin());
	vector<int> assetIds;
	set<int> allAssets = GetAllAssets();
	filter.FillUsedAssets(allAssets, assetIds);
	auto supplementMatrix = make_shared<CSupplementMatrix>(assetIds, checks, filter);
	return supplementMatrix;
}

set<int> CSalesHistory::GetAllAssets() const {
	return this->assetsAll;
}

shared_ptr<CPriceList> CSalesHistory::GenCurrentPriceList() const {
	return nullptr;
}

void CSalesHistory::FillDaysStatistics(CMatrix& priceMatrix, CMatrix& volumeMatrix, CMatrix& incomeMatrix, const CFilterAdvanced& filter) const {
	priceMatrix.Empty();
	volumeMatrix.Empty();
	incomeMatrix.Empty();
	//amtOfChecks.clear();
	for (const auto & checkPtr : checks) {
		const time_t checkTime = CDate(checkPtr->checkTime).DayOnly();

		/* Добавляем колонку с нулями, если она не существует */
		volumeMatrix.AddNewEmptyCol(checkTime);
		incomeMatrix.AddNewEmptyCol(checkTime);
		priceMatrix.AddNewEmptyCol(checkTime);

		/* Заполняем volumeMatrix и incomeMatrix */
		list<shared_ptr<const CCheckItem>> resultingList;
		checkPtr->FilterItems(filter, resultingList);
		for (const auto checkItemPtr : resultingList) {
			const int assetId = checkItemPtr->assetId;
			volumeMatrix.AddPlusValue(assetId, checkTime, checkItemPtr->volume);
			incomeMatrix.AddPlusValue(assetId, checkTime, checkItemPtr->Income());
		}

		/* Увеличиваем количество чеков на 1 */
		/* if (resultingList.size() > 0) {
			auto it = amtOfChecks.find(checkTime);
			if (it == amtOfChecks.end()) {
				amtOfChecks.insert(make_pair(checkTime, 1));
			} else {
				it->second++;
			}
		} else {
			amtOfChecks.insert(make_pair(checkTime, 0));
		}
		*/
	}
	//cout << "Printint volumeMatrix" << endl;
	//cout << volumeMatrix.ToString() << endl;
	/* Далее заполняем матрицу priceMatrix */
	const auto assetIds = volumeMatrix.GetAssetIds();
	const auto times = volumeMatrix.GetTimes();
	priceMatrix.Init(assetIds, times);
	for (int assetId : assetIds) {
		for (time_t dt : times) {
			const double income = incomeMatrix.GetValue(assetId, dt);
			const double volume = volumeMatrix.GetValue(assetId, dt);
			const double price = (volume > 0) ? (income / volume) : 0.0;
			priceMatrix.SetValue(assetId, dt, price);
		}
	}
}

set<int> CSalesHistory::GetGroupIds() const {
	return groupsInfoPtr->GetGroupIds();
}

shared_ptr<const CGroupsInfo> CSalesHistory::GetGroupsInfo() const {
	return groupsInfoPtr;
}

const map<time_t, int>& CSalesHistory::GetDaysAmountOfChecks() const {
	return amtOfChecks;
}

void CSalesHistory::RemoveCheckFromInternalStructure(list<shared_ptr<CCheck>>::iterator checksIterator)
{
	auto check = *checksIterator;
	checks.erase(checksIterator);
	unique_key ukey(check->checkId, check->checkTime);
	checks_map.erase(ukey);
}

shared_ptr<CCheck> CSalesHistory::FindCheck(int checkId, time_t checkTime) const
{
	shared_ptr<CCheck> check;
	unique_key key(checkId, checkTime);
	auto it = checks_map.find(key);
	return it->second;
}

shared_ptr<CCheck> CSalesHistory::FindCheck(unique_key key) const
{
	return FindCheck(key.first, key.second);
}

shared_ptr<const set<int>> CSalesHistory::GetAssetsFromGroup(int groupId) const
{
	return groupsInfoPtr->GetAssetsInGroup(groupId);
}

CSalesHistory::CSalesHistory(shared_ptr<list<CHistoryItem> > items) : CSalesHistory() {
	for (const auto & item : *items) {
		this->AddLine(item);
	}
	this->CalcAmountOfChecks();
}

shared_ptr<CAssetsSummary> CSalesHistory::GetAssetsSummary(const CFilter& filter) const {
	CFilterAdvanced filterAdvanced(filter, groupsInfoPtr);
	auto assetsSummary = make_shared<CAssetsSummary>();
	set<int> assetIds;
	map<int, double> incomes;
	map<int, double> amounts;
	for (const auto check : checks) {
		list<shared_ptr<const CCheckItem>> checkItems;
		check->FilterItems(filterAdvanced, checkItems);
		for (const auto checkItem : checkItems) {
			const int assetId = checkItem->assetId;
			const double curIncome = checkItem->Income();
			const double curVolume = checkItem->volume;
			const auto it = assetIds.find(assetId);
			if (it == assetIds.end()) {
				assetIds.insert(assetId);
				incomes.insert(pair<int, double>(assetId, curIncome));
				amounts.insert(pair<int, double>(assetId, curVolume));
			} else {
				incomes[assetId] += curIncome;
				amounts[assetId] += curVolume;
			}
		}
	}
	for (const int assetId : assetIds) {
		assetsSummary->AddItem(assetId, incomes[assetId], amounts[assetId]);
	}
	return assetsSummary;
}

void CSalesHistory::SetGroupsInfo(shared_ptr<const CGroupsInfo> groupsInfo) {
	this->groupsInfoPtr = groupsInfo;
}

vector<double> CSalesHistory::CalculateDynamics(const CFilter& filter, FunMode funMode) const {
	CFilterAdvanced newFilter(filter, groupsInfoPtr);
	const int len = (newFilter.intervals.size() <= 0 ? 1 : newFilter.intervals.size());	// кол-во элементов в итоговом массиве
	vector<double> resultingContainer(len, 0.0);
	vector<double> resultingAmounts(len, 0.0);
	for (const auto & checkPtr : checks) {
		list<shared_ptr<const CCheckItem>> checkItems;
		checkPtr->FilterItems(newFilter, checkItems);	// заполняет список checkItems
		int checkIndex = checkPtr->FindInterval(newFilter.intervals, newFilter.IntervalsFilterMode());
		const int foundIndex = (len == 1) ? 0 : checkIndex;
		double curIncome = 0.0;
		for (const auto & checkItemPtr : checkItems) {
			curIncome += checkItemPtr->Income();
		}
		resultingContainer[foundIndex] += curIncome;
		if (checkItems.size() > 0) {
			resultingAmounts[foundIndex]++;
		}
	}
	if (funMode == FunMode::AvgCheck) {
		for (int i = 0; i < len; ++i) {
			if (resultingAmounts[i] != 0) {
				resultingContainer[i] /= resultingAmounts[i];
			}
		}
	}
	return resultingContainer;
}

const map<time_t, int> & CSalesHistory::CalcAmountOfChecks() {
	amtOfChecks.clear();
	for (const auto & checkPtr : checks) {
		const time_t checkTime = CDate(checkPtr->checkTime).DayOnly();
		auto it = amtOfChecks.find(checkTime);
		if (it == amtOfChecks.end()) {
			amtOfChecks.insert(make_pair(checkTime, 1));
		} else {
			it->second++;
		}
	}
	return amtOfChecks;
}
