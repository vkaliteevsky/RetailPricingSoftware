/*
 * IPriceOptimizer.cpp
 *
 *  Created on: Jun 20, 2019
 *      Author: kraychik
 */

#include "IPriceOptimizer.h"

IPriceOptimizer::IPriceOptimizer() {
	salesHistory = nullptr;
	curPriceList = nullptr;
	optimalPrices = nullptr;
	params = nullptr;
	companyId = 0;
}

IPriceOptimizer::~IPriceOptimizer() {

}

/*
IPriceOptimizer::IPriceOptimizer(int projectId) {
	auto dataBase = CDataBase::GetInstance();
	auto salesList = dataBase->ReadSales(projectId);
	auto salesHistory = make_shared<const CSalesHistory>(salesList);
	auto pairAssetsItems = dataBase->ReadPrices(projectId);
	auto curPriceList = make_shared<CPriceList>(pairAssetsItems.first, pairAssetsItems.second);
	IPriceOptimizer(salesHistory, curPriceList);
}
*/

void IPriceOptimizer::SetSalesHistory(shared_ptr<CSalesHistory> salesHistory) {
	this->salesHistory = salesHistory;
}

void IPriceOptimizer::SetCurrentPriceList(shared_ptr<CPriceList> priceList) {
	curPriceList = priceList;
}

void IPriceOptimizer::SetParams(shared_ptr<COptimizerParams> params) {
	this->params = params;
}

shared_ptr<const CPriceList> IPriceOptimizer::GetOptimalPrices() const {
	return optimalPrices;
}

shared_ptr<const CSalesHistory> IPriceOptimizer::GetSalesHistory() const {
	return salesHistory;
}

shared_ptr<const CPriceList> IPriceOptimizer::GetCurrentPrices() const {
	return curPriceList;
}

IPriceOptimizer::IPriceOptimizer(int companyId, shared_ptr<const CSalesHistory> salesHistory, shared_ptr<const CPriceList> curPriceList, shared_ptr<COptimizerParams> params)
	: salesHistory(salesHistory), curPriceList(curPriceList), companyId(companyId), params(params) {
}

shared_ptr<const COptimizerParams> IPriceOptimizer::GetParams() const {
	return params;
}

bool IPriceOptimizer::CheckDataBeforeCalc() const {
	return ((salesHistory != nullptr) && (curPriceList != nullptr));
}

double IPriceOptimizer::DefaultSpread() const {
	return 0.10;
}

void IPriceOptimizer::SetDefaultParams() {
	const double delta = DefaultSpread();	// +- 5% (10%)
	const auto curPrices = static_pointer_cast<const CPriceList>(curPriceList);
	const auto assetsPtr = curPrices->GetAllAssets();
	params = make_shared<COptimizerParams>();
	for (int assetId : *assetsPtr) {
		const auto priceItem = curPrices->GetPriceItem(assetId);
		const double curPrice = priceItem->price;
		const double minPrice = ceil(curPrice * (1 - delta));
		const double maxPrice = floor(curPrice * (1 + delta));
		auto newParamItemPtr = make_shared<CParamsItem>(-1, minPrice, maxPrice, false);
		params->AddParamItem(assetId, newParamItemPtr);
	}
}
int IPriceOptimizer::Random(int from, int to) {
	return (rand() % (to-from+1)) + from;
}

void IPriceOptimizer::GenRandomVector(vector<int>& arr) {
	iota(arr.begin(), arr.end(), 0);
	const int n = arr.size();
	for (int k = 0; k < n; ++k) {
		int i = Random(0, n-1);
		int j = Random(0, n-1);
		int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
	}
}
double IPriceOptimizer::GenRandValue(const vector<double>& values, const vector<double>& probs) {
	const int maxInt = 10000;
	const int n = probs.size();
	vector<double> cumProbs(n, 0);
	double cumS = 0.0;
	for (int i = 0; i < n; ++i) {
		cumS += probs[i];
		cumProbs[i] = cumS;
	}
	int randNumber = rand() % maxInt;
	if (randNumber < cumProbs[0] * maxInt) return values[0];
	for (int i = 1; i < n; ++i) {
		if (randNumber < cumProbs[i] * maxInt) return values[i];
	}
	// следующие строчки никогда не должны исполниться
	return values[n-1];
}

shared_ptr<const CPriceList> IPriceOptimizer::CalcOptimalPriceList(double monthSince, int periodDays, int amountOfDenseAssets) {
	const double startMonth = 8.2;
	bool isDataOk = CheckDataBeforeCalc();
	if (!isDataOk) {
		return nullptr;
	}
	CheckAndSetParams();

	set<int> assetsToTeach = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50};
	forcaster->TeachModel(assetsToTeach, periodDays, amountOfDenseAssets);

	vector<int> assetIds;
	vector<double> a, b;
	CFilter filterAssets(true);
	filterAssets.SetAssetIds(assetsToTeach, false);
	params->FillBoxConstraints(filterAssets, assetIds, a, b);

	function<double(const vector<double> &)> f = [this, & assetIds, periodDays, startMonth](const vector<double> & p) {
		map<int, double> newPrices;
		const int n = (int)assetIds.size();
		for (int i = 0; i < n; ++i) {
			newPrices.insert(make_pair(assetIds[i], p[i]));
		}
		const auto exSalesPtr = forcaster->ForcastSales(newPrices, startMonth, periodDays);
		const double exIncome = EvaluateOutcome(*exSalesPtr, newPrices);
		return exIncome;
	};

	const auto startPoint = CAlgorithms::RandomDoubleVector(a, b);
	vector<double> optimalPrices;
	CAlgorithms::OptimizeFunctionDerivative(f, a, b, *startPoint, optimalPrices, CAlgorithms::TargetObjective::Max);

	const vector<double> costs(assetIds.size(), 0.0);
	const auto optPriceList = make_shared<CPriceList>(assetIds, optimalPrices, costs);
	return optPriceList;
	/*
	auto groupIds = params->GetAllGroups();
	auto groupPriceLists = vector<shared_ptr<CPriceList>>();
	groupPriceLists.reserve(groupIds->size());
	for (const auto groupId : *groupIds) {
		auto tmpPriceList = CalcOptimalPriceListForGroup(groupId, periodDays);
		groupPriceLists.push_back(tmpPriceList);
	}

	auto optimalPriceList = groupPriceLists[0];
	for (uint i = 1; i < groupPriceLists.size(); i++) {
		optimalPriceList->JoinPriceList(groupPriceLists[i]);
	}

	if (optimalPriceList->Size() != curPriceList->Size()) {
		throw CSmartException("IPriceOptimizer::CalcOptimalPriceList: final price list length not equal to current one");
	}
	return optimalPriceList;
	*/
}

shared_ptr<map<int, double>> IPriceOptimizer::CalcRandomPrices() const {
	const auto curPrices = curPriceList->GetPricesMap();
	auto resPtr = make_shared<map<int, double>>();
	for (const auto & pair : *curPrices) {
		const int assetId = pair.first;
		const auto paramItem = params->GetParamItem(assetId);
		const double nPrice = Random(ceil(paramItem->minPrice), floor(paramItem->maxPrice));
		resPtr->insert(std::pair<int, double>(assetId, nPrice));
	}
	return resPtr;
}

double IPriceOptimizer::EvaluateOutcome(map<int, double>& sales, map<int, double>& prices) const {
	double income = 0.0;
	for (const auto & pairSale : sales) {
		const int assetId = pairSale.first;
		const auto it = prices.find(assetId);
		if (it == prices.end()) {
			CSmartException("IPriceOptimizer::EvaluateOutcome: could't find assetId in prices");
		}
		const double price = it->second;
		income += (pairSale.second * price);
	}
	return income;
}

void IPriceOptimizer::CheckAndSetParams() {
	if (params == nullptr) {
		CDataManager *manager = CDataManager::GetInstance();
		auto projectInfo = manager->GetOrLoadProjectInfo(companyId);
		params = projectInfo->GetOrLoadOptimizerParams();
		if (params == nullptr) {
			SetDefaultParams();
		}
	}
}

shared_ptr<CPriceList> IPriceOptimizer::CalcOptimalPriceListForGroup(int groupId, int periodDays) const {
	const double startMonth = 8.2;
	auto assetIds = params->GetAssetsFromGroup(groupId);
	auto groupPriceList = curPriceList->FilterPriceList(assetIds);

	const int n = 10000;
	vector<double> expectedIncomes(n);
	vector<shared_ptr<map<int, double>>> prices(n);

	for (int i = 0; i < n; ++i) {
		auto newPrices = CalcRandomPrices(assetIds);
		const auto expectedSales = forcaster->ForcastSales(*newPrices, startMonth, periodDays);
		const double eIncome = EvaluateOutcome(*expectedSales, *newPrices);
		expectedIncomes[i] = eIncome;
		prices[i] = newPrices;
	}

	int bestInd = ChooseBestOutcome(expectedIncomes);
	auto bestPrices = prices[bestInd];

	//auto groupPriceList = make_shared<CPriceList>();
	for (const auto & pair : *bestPrices) {
		const int assetId = pair.first;
		const double price = pair.second;
		groupPriceList->GetPriceItem(assetId)->SetPrice(price);
	}
	return groupPriceList;
}

shared_ptr<map<int, double> > IPriceOptimizer::CalcRandomPrices(shared_ptr<std::set<int>> assetIds) const {
	const auto curPrices = curPriceList->FilterPriceList(assetIds)->GetPricesMap();
	auto resPtr = make_shared<map<int, double>>();
	for (const auto & pair : *curPrices) {
		const int assetId = pair.first;
		const auto paramItem = params->GetParamItem(assetId);
		const double nPrice = Random(ceil(paramItem->minPrice), floor(paramItem->maxPrice));
		resPtr->insert(std::pair<int, double>(assetId, nPrice));
	}
	return resPtr;
}

shared_ptr<IForcaster> IPriceOptimizer::GetForcaster() const {
	return forcaster;
}

int IPriceOptimizer::ChooseBestOutcome(const vector<double> & incomes) const {
	auto copyIncomes = incomes;
	const int n = (int)incomes.size();
	std::sort(copyIncomes.begin(), copyIncomes.end());
	const double resIncome = copyIncomes[ceil(n * 0.85)];

	int bestInd = -1;
	for (int i = 0; i < n; ++i) {
		if (abs(resIncome - incomes[i]) <= 0.001) {
			bestInd = i;
		}
	}
	if (bestInd == -1) {
		throw CSmartException("IPriceOptimizer::ChooseBestOutcome: didn't find bestInd");
	}
	return bestInd;
}
