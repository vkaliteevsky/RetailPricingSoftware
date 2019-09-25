/*
 * CElastPriceOptimizer.cpp
 *
 *  Created on: Jun 21, 2019
 *      Author: kraychik
 */

#include "CElastPriceOptimizer.h"
#include "forcast/IRegressionForcaster.h"
#include <algorithm>

CElastPriceOptimizer::CElastPriceOptimizer() : IPriceOptimizer() {
	forcaster = nullptr;
}

CElastPriceOptimizer::CElastPriceOptimizer(int companyId,
		shared_ptr<const CSalesHistory> salesHistory,
		shared_ptr<const CPriceList> curPriceList,
		shared_ptr<COptimizerParams> params, shared_ptr<IForcaster> forcaster)
	: IPriceOptimizer(companyId, salesHistory, curPriceList, params) {
	this->forcaster = forcaster;
	/*
	if (amountOfTopDenseAssets < 0) {
		throw CSmartException("CElastPriceOptimizer::CElastPriceOptimizer: cannot have negative amountOfTopDenseAssets");
	}
	this->amountOfTopDenseAssets = amountOfTopDenseAssets;
	*/
}
shared_ptr<const CPriceList> CElastPriceOptimizer::CalcOptimalPriceList(double monthSince, int periodDays, int amountOfDenseAssets) {
	cout << "Optimization Started: Since Date = " << monthSince << ", Period = " << periodDays << " days, ";
	cout << "Dense Assets = " << amountOfDenseAssets << " ..." << endl;
	bool isDataOk = CheckDataBeforeCalc();
	if (!isDataOk) {
		return nullptr;
	}
	CheckAndSetParams();

	params->ChangeBoxConstraintsForIndicators(curPriceList);
	set<int> assetsToOptimize;
	//params->FillAssetsToOptimize(assetsToOptimize);
	FillAssetsToOptimize(curPriceList, params, assetsToOptimize);
	//const set<int> assetsToTeach = salesHistory->GetAllAssets();
	const set<int> assetsToTeach = assetsToOptimize;
	//set<int> assetsToTeach = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50};
	//set<int> assetsToTeach = salesHistory->GetAllAssets();
	auto thisForcaster = static_pointer_cast<IRegressionForcaster>(forcaster);
	thisForcaster->TeachModel(assetsToTeach, periodDays, amountOfDenseAssets);

	//auto supMatrix = thisForcaster->GetSupplementMatrix();
	set<int> denseAssets, sparseAssets;
	thisForcaster->FillDenseAndSparseAssets(denseAssets, sparseAssets);
	//supMatrix->ChooseTopDenseAssets(amountOfTopDenseAssets, denseAssets);
	//supMatrix->FillAssetIds(allAssetIds);
	//set_difference(assetsToOptimize.begin(), assetsToOptimize.end(), denseAssets.begin(), denseAssets.end(), inserter(sparseAssets, sparseAssets.begin()));

	// denseAssets - содержит кореллированные id, а sparseAssets - некореллированные id

	auto densePriceList = OptimizeDenseAssets(denseAssets, monthSince, periodDays, 50);
	auto sparsePriceList = OptimizeSparseAssets(sparseAssets, monthSince, periodDays, 50);
	if (densePriceList == nullptr) {
		return sparsePriceList;
	} else {
		if (sparsePriceList == nullptr) {
			return densePriceList;
		} else {
			densePriceList->JoinPriceList(sparsePriceList);
			return densePriceList;
		}
	}
}

void CElastPriceOptimizer::NotActive() const {
	return;
}

CElastPriceOptimizer::~CElastPriceOptimizer() {

}

shared_ptr<CPriceList> CElastPriceOptimizer::OptimizeDenseAssets(const set<int>& denseAssetIds, double monthSince, int periodDays, int iters) const {
	cout << "Optimizing Dense Assets ..." << endl;
	const int n = (int)denseAssetIds.size();
	if (n <= 0) {
		return nullptr;
	}
	auto thisForcaster = static_pointer_cast<IRegressionForcaster>(forcaster);
	//auto supMatrix = thisForcaster->GetSupplementMatrix();
	dlib::matrix<double> M, Q;
	vector<int> assetIds(n, 0);	// содержит последовательность активов в матрице Q
	std::copy(denseAssetIds.begin(), denseAssetIds.end(), assetIds.begin());

	//CAlgorithms::MakeMatrix(supMatrix, assetIds, M);
	thisForcaster->MakeMatrix(assetIds, M);
	Q = dlib::trans(M);

	dlib::matrix<double> curVolumes(1, n);	// содержит текущие средние значения продаж в штуках
	dlib::matrix<double> curPrices(1, n);
	for (int i = 0; i < n; ++i) {
		const int assetId = assetIds[i];
		const double curPrice = curPriceList->GetPrice(assetId);
		curPrices(0, i) = curPrice;
		curVolumes(0, i) = forcaster->ForcastSalesIndependent(assetId, curPrice, monthSince, periodDays);
	}

	function<double(const vector<double> &)> f = [thisForcaster, & assetIds, monthSince, periodDays, &Q, &curVolumes, &curPrices](const vector<double> & p) {
		const int n = (int)p.size();
		dlib::matrix<double> exVolumes(1, n);	// ожидаемые значения продаж в штуках
		for (int i = 0; i < n; ++i) {
			const int assetId = assetIds[i];
			const double newP = p[i];
			exVolumes(0, i) = thisForcaster->ForcastSalesIndependent(assetId, newP, monthSince, periodDays);
		}
		const double curIncome = (curVolumes * Q) * trans(curVolumes);
		const double exIncome = (exVolumes * Q) * trans(exVolumes);
		return exIncome - curIncome;
	};

	vector<double> a, b;
	CFilter filterAssets(true);
	filterAssets.SetAssetIds(assetIds, false);
	params->FillBoxConstraints(assetIds, a, b);

	vector<double> optPrices, costs(n, 0.0);
	CAlgorithms::OptimizeFunctionGlobal(f, a, b, optPrices, CAlgorithms::TargetObjective::Max, iters);

	//for (int i = 0; i < n; ++i) {
	//	cout << assetIds[i] << ": " << a[i] << " " << optPrices[i] << " " << b[i] << endl;
	//}
	return make_shared<CPriceList>(assetIds, optPrices, costs);
}



shared_ptr<CPriceList> CElastPriceOptimizer::OptimizeSparseAssets(const set<int>& sparseAssetIds, double monthSince, int periodDays, int iters) const {
	const int n = (int)sparseAssetIds.size();
	if (n <= 0) {
		 return nullptr;
	}
	vector<int> assetIds(n, 0);
	vector<double> optPrices(n, 0.0), costs(n, 0.0);
	auto thisForcaster = static_pointer_cast<IRegressionForcaster>(forcaster);

	int i = 0;
	cout << "Optimizing Sparse Assets ..." << endl;
	for (const int assetId : sparseAssetIds) {
		//cout << "Optimizing Sparse Asset: ID = " << assetId << endl;
		const double curPrice = curPriceList->GetPrice(assetId);
		const double curVolume = thisForcaster->ForcastSalesIndependent(assetId, curPrice, monthSince, periodDays);
		const double curIncome = curVolume * curPrice;

		function<double(const vector<double> &)> f = [thisForcaster, assetId, curIncome, monthSince, periodDays](const vector<double> & p) {
			const double exVolume = thisForcaster->ForcastSalesIndependent(assetId, p[0], monthSince, periodDays);
			const double exIncome = exVolume * p[0];
			return exIncome - curIncome;
		};

		vector<double> a, b, localOptPrices;
		vector<int> localAssetIds = { assetId };
		CFilter filterAssets(true);
		filterAssets.SetAssetIds(localAssetIds, false);
		params->FillBoxConstraints(localAssetIds, a, b);

		CAlgorithms::OptimizeFunctionGlobal(f, a, b, localOptPrices, CAlgorithms::TargetObjective::Max, iters);

		assetIds[i] = assetId;
		optPrices[i] = localOptPrices[0];
		//cout << assetId << ": " << a[0] << " " << localOptPrices[0] << " " << b[0] << endl;
		i++;
	}
	return make_shared<CPriceList>(assetIds, optPrices, costs);
}

void CElastPriceOptimizer::FillAssetsToOptimize(shared_ptr<const CPriceList> curPriceList, shared_ptr<const COptimizerParams> params, set<int>& assetsToOptimize) const {
	assetsToOptimize.clear();
	set<int> idsInParams;
	params->FillAssetsToOptimize(idsInParams);
	auto idsInPrices = curPriceList->GetAllAssets();
	for (const int assetId : *idsInPrices) {
		if (idsInParams.find(assetId) != idsInParams.end()) {
			assetsToOptimize.insert(assetId);
		}
	}
	//set_intersection(idsInParams.begin(), idsInParams.end(), idsInPrices->begin(), idsInPrices->end(), assetsToOptimize.begin());
}
