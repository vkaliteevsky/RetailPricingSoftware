/*
 * CAnalyzer.cpp
 *
 *  Created on: Feb 8, 2019
 *      Author: kraychik
 */

#include "COptimizer.h"

COptimizer::COptimizer(shared_ptr<CSalesHistory> salesHistory, shared_ptr<CPriceList> curPriceList) {
	this->salesHistory = salesHistory;
	this->curPriceList = curPriceList;
}

COptimizer::COptimizer(int projectId) {
	auto dataBase = CDataBase::GetInstance();
	auto salesList = dataBase->ReadSales(projectId);
	this->salesHistory = make_shared<CSalesHistory>(salesList);
	auto pairAssetsItems = dataBase->ReadPrices(projectId);
	curPriceList = make_shared<CPriceList>(pairAssetsItems.first, pairAssetsItems.second);
}

shared_ptr<CPriceList> COptimizer::CalcExperimentalPriceList(const COptimizerParams & params) const {
	auto resultingPriceList = make_shared<CPriceList>();
	std::shared_ptr<std::vector<int>> groupIds = params.GetAllGroups();
	for (const int groupId : *groupIds) {
		auto assetIds = params.GetAssetsFromGroup(groupId);
		auto filteredParams = params.Filter(*assetIds);
		std::shared_ptr<CPriceList> newPriceList = CalcExperimentalPriceListInGroup(assetIds, filteredParams);
		resultingPriceList->JoinPriceList(newPriceList);
	}
	return resultingPriceList;
}

shared_ptr<CPriceList> COptimizer::CalcExperimentalPriceListInGroup(shared_ptr<set<int>> assetIdsSet, shared_ptr<COptimizerParams> params) const {
	auto priceList = curPriceList->FilterPriceList(assetIdsSet);
	int n = assetIdsSet->size();
	vector<int> assetIds;
	vector<shared_ptr<CPriceItem>> priceItems;
	priceList->FillVectors(assetIds, priceItems);

	if (n != priceList->Size()) {
		throw CSmartException("COptimizer::CalcExperimentalPriceListInGroup: Размерность priceList должна совпадать с размерностью assetId");
	}

	double maxPrice = params->MaxPrice();
	double minPrice = params->MinPrice();
	CPriceGrid priceGrid(minPrice, maxPrice);

	vector<double> w(n, 1.0/n);		// значимость каждого актива
	vector<double> resultingPrices(n, 0.0);
	double effectValue = -1.0;

	for (int i = 0; i < 2000; ++i) {
		vector<double> resPrice(n, 0.0);	//  будет содержать новые цены товаров
		vector<int> inds(n, 0); // индексы [0, n-1], которые заполняются случайным образом
		GenRandomVector(inds);  // инициализируем числами {0, 1, ... n-1} и затем случайным образом перемешиваем
		for (const int j : inds) {
			// анализируем актив с номером j
			const double curPrice = priceItems[j]->price;	// текущая цена товара assets[j]
			// определяем направление, в котором будем изменять цену curPrice
			const double weightRight = priceGrid.WeightRight(curPrice);
			const double weightLeft = priceGrid.WeightLeft(curPrice);
			const double probToGoLeft = ((weightLeft <= 0.00001 && weightRight <= 0.00001) ? 0.5 : weightLeft / (weightLeft + weightRight));
			int randomNumber = Random(1, 10000);
			const bool toGoLeft = (randomNumber <= probToGoLeft * 10000) ? true : false;

			// определяем новую цену
			double newPrice = 0.0;
			const auto priceItem = params->GetParamItem(assetIds[j]);
			if (toGoLeft) {
				vector<double> possiblePrices = priceGrid.GetGridSequence((int)priceItem->minPrice, (int)curPrice);
				vector<double> probs(possiblePrices.size(), 1.0/possiblePrices.size());
				newPrice = GenRandValue(possiblePrices, probs);
			} else {
				vector<double> possiblePrices = priceGrid.GetGridSequence((int)curPrice, (int)priceItem->maxPrice);
				vector<double> probs(possiblePrices.size(), 1.0/possiblePrices.size());
				newPrice = GenRandValue(possiblePrices, probs);
			}

			// сохраняем полученную информацию о структуре эластичности
			priceGrid.AddWeightInfo((int)curPrice, (int)newPrice, w[j]);

			resPrice[j] = newPrice;
		}
		// массив resPrice заполнен новыми ценами
		// рассчитываем информационную эффективность текущего распределения
		double eValue = priceGrid.Effectiveness();
		if (eValue > effectValue) {
			effectValue = eValue;
			resultingPrices = resPrice;
		}
	}
	auto resultingPriceList = priceList->CloneWithItems();
	resultingPriceList->UpdatePrices(assetIds, resultingPrices);
	return resultingPriceList;
}

COptimizer::~COptimizer() {
	// TODO Auto-generated destructor stub
}

int COptimizer::Random(int from, int to) {
	return (rand() % (to-from+1)) + from;
}

void COptimizer::GenRandomVector(vector<int>& arr) {
	iota(arr.begin(), arr.end(), 0);
	const int n = arr.size();
	for (int k = 0; k < n; ++k) {
		int i = Random(0, n-1);
		int j = Random(0, n-1);
		int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
	}
}

shared_ptr<const vector<int> > COptimizer::DetermineIndicatorAssets() {
	if (supplementMatrix == nullptr) {
		this->CalcSupplementMatrix();
	}
	auto columnsMap = supplementMatrix->GetAllColumns();
	const int n = columnsMap->size();
	vector<double> sums(n, 0);
	vector<int> assetIds(n, 0);
	int i = 0;
	for (const auto & mapItem : *columnsMap) {
		assetIds[i] = mapItem.first;
		sums[i] = mapItem.second->Sum();
	}
	auto resultingContainer = std::make_shared<vector<int>>();

	//TODO
	int minPos = distance(sums.begin(), min_element(sums.begin(), sums.end()));
	resultingContainer->push_back(assetIds[minPos]);
	//

	return resultingContainer;
}

shared_ptr<const CSupplementMatrix> COptimizer::CalcSupplementMatrix() {
	CFilter filter;
	CFilterAdvanced aFilter(filter, nullptr);
	supplementMatrix = salesHistory->BuildSupplementMatrix(aFilter);
	return supplementMatrix;
}

double COptimizer::GenRandValue(const vector<double>& values, const vector<double>& probs) {
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
