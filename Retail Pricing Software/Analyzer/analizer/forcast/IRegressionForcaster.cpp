/*
 * IRegressionForcaster.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: kraychik
 */

#include "IRegressionForcaster.h"
#include "../../supply/CSmartException.h"
#include <list>

IRegressionForcaster::IRegressionForcaster() : IForcaster(), supMatrix(nullptr) {
}

IRegressionForcaster::IRegressionForcaster(shared_ptr<const CSalesHistory> salesHistory, shared_ptr<const CPriceList> curPriceList, shared_ptr<const COptimizerParams> params)
	: IForcaster(salesHistory, curPriceList, params) {
	supMatrix = nullptr;
}

IRegressionForcaster::~IRegressionForcaster() {
}

shared_ptr<vector<double>> IRegressionForcaster::GetTheta(int assetId) const {
	const auto it = thetas.find(assetId);
	if (it == thetas.end()) {
		throw CSmartException("IRegressionForcaster::GetTheta: couldn't find assetId");
	}
	return it->second;
}

void IRegressionForcaster::EstimateThetaForOneAsset(const CMatrix& priceMatrix, const CMatrix& volumeMatrix, const CMatrix& incomeMatrix, const map<time_t, int>& amtOfChecks, vector<double>& params) {
	params.clear();
	const auto dts = priceMatrix.GetTimes();
	const auto assetIds = priceMatrix.GetAssetIds();
	if (assetIds.size() != 1) {
		throw CSmartException("IRegressionForcaster::EstimateThetaForOneAsset: assetIds size not equal to one");
	}

	dlib::matrix<double> xs;
	vector<double> ys;
	FilterAndPrepareDataSet(priceMatrix, volumeMatrix, incomeMatrix, amtOfChecks, xs, ys);	// (xs -> ys) - dataset

	vector<double> aVect = ThetaLowerLimits();
	vector<double> bVect = ThetaUpperLimits();
	function<double(const vector<double> &)> f = [&xs, &ys, this](const vector<double> &thPars) {
		const int n = (int)ys.size();
		double s = 0.0;
		for (int i = 0; i < n; ++i) {
			const double x = this->Demand(xs(0, i), xs(1, i), 1, thPars);
			const double y = ys[i];
			const double tmp = y - x;
			s += tmp * tmp;
		}
		return s;
	};
	CAlgorithms::OptimizeFunctionGlobal(f, aVect, bVect, params, CAlgorithms::TargetObjective::Min, 50);
}

void IRegressionForcaster::TeachModel(const set<int>& assetIds, int periodDays, int amountOfDenseAssets) {
	cout << "Teach Model Started ... " << endl;
	{
	CFilter filter(true);
	filter.SetAssetIds(assetIds, false);	// обучаем только по множеству активов assetIds
	const auto groupsInfo = params->CalcGroupsInfo();
	CFilterAdvanced aFilter(filter, groupsInfo);
	supMatrix = salesHistory->BuildSupplementMatrix(aFilter);
	ChooseTopDenseAssets(amountOfDenseAssets);
	}

	set<int> assets = assetIds;
	const auto groupIds = params->GetAllGroups();
	{
	shared_ptr<CSalesHistory> sH = const_pointer_cast<CSalesHistory>(salesHistory);
	sH->CalcAmountOfChecks();
	}

	const auto groupsInfoPtr = params->CalcGroupsInfo();
	vector<interval> emptyIntervalVector;
	set<int> emptySet;
	const map<time_t, int> amtOfChecks = salesHistory->GetDaysAmountOfChecks();
	CFilter filter(emptyIntervalVector, assets, emptySet, emptySet, true, false, true, true);	// filter by assetIds

	CFilterAdvanced aFilter(filter, groupsInfoPtr);
	CMatrix priceMatrix, volumeMatrix, incomeMatrix;
	salesHistory->FillDaysStatistics(priceMatrix, volumeMatrix, incomeMatrix, aFilter);
	for (const int assetId : assets) {
		cout << "Teaching for assetId = " << assetId << " ..." << endl;
		const set<int> assetSet = { assetId };
		vector<double> thPars;
		CMatrix localPriceMatrix, localVolumeMatrix, localIncomeMatrix;
		priceMatrix.Filter(assetSet, localPriceMatrix);
		volumeMatrix.Filter(assetSet, localVolumeMatrix);
		incomeMatrix.Filter(assetSet, localIncomeMatrix);
		RestoreMissingPrices(localPriceMatrix);
		EstimateThetaForOneAsset(localPriceMatrix, localVolumeMatrix, localIncomeMatrix, amtOfChecks, thPars);
		cout << assetId << ": " << thPars[0] << ", " << thPars[1] << ", " << thPars[2] << ", " << thPars[3] << ", " << thPars[4] << ", " << thPars[5] << ", " << thPars[6] << endl;
		thetas.insert(make_pair(assetId, make_shared<vector<double>>(thPars)));
	}
	// взвешиваем все рассчитанные параметры по группам
	NormalizeThetas(groupsInfoPtr, priceMatrix, volumeMatrix);
	isTaught = true;
	cout << "Teach Model Done." << endl;
}

void IRegressionForcaster::FilterAndPrepareDataSet(const CMatrix &priceMatrix, const CMatrix &volumeMatrix, const CMatrix &incomeMatrix, const map<time_t, int> &amtOfChecks, dlib::matrix<double> & xs, vector<double> & ys) {
	const int assetId = *priceMatrix.GetAssetIds().begin();
	const auto dts = priceMatrix.GetTimes();
	list<tuple<double, time_t, double>> listDataSet;

	for (const time_t dt : dts) {
		const double price = priceMatrix.GetValue(assetId, dt);
		if (abs(price) >= 0.01) {
			listDataSet.push_back(tuple<double, time_t, double>(price, dt, volumeMatrix.GetValue(assetId, dt)));
		}
	}

	const int n = (const int)listDataSet.size();
	xs.set_size(2, n);
	ys.resize(n);
	int i = 0;
	for (auto it = begin(listDataSet); it != end(listDataSet); it++) {
		const double price = get<0>(*it);
		const time_t dt = get<1>(*it);
		const double volume = get<2>(*it);
		const auto amtIt = amtOfChecks.find(dt);
		if (amtIt == amtOfChecks.end()) {
			throw CSmartException("IRegressionForcaster::FilterAndPrepareDataSet: cannot find dt");
		}
		const double amtC = (double)amtIt->second;
		xs(0, i) = price; xs(1, i) = (double)dt;
		ys[i] = volume / amtC;
		i++;
	}
}

void IRegressionForcaster::NormalizeThetas(shared_ptr<const CGroupsInfo> groupsInfo, const CMatrix & priceMatrix, const CMatrix & volumeMatrix) {
	const auto groupIds = groupsInfo->GetAllGroups();

	set<int> assetsToTeach;
	for (const auto pr : thetas) {
		assetsToTeach.insert(pr.first);
	}

	for (const int groupId : *groupIds) {
		const auto assetIdsPtr = groupsInfo->GetAssetsInGroup(groupId);	// assets in current group

		map<int, double> ws;
		double sumVolGlob = 0.0;
		bool wasIn = false;
		for (const int assetId : *assetIdsPtr) {
			// учитываем вес только при условии, что assetId \in assetsToTeach
			if (assetsToTeach.find(assetId) != assetsToTeach.end()) {
				const double sumV = volumeMatrix.SumByAssetId(assetId);
				ws.insert(make_pair(assetId, sumV));
				sumVolGlob += sumV;
				wasIn = true;
			}
		}
		if (!wasIn) continue;
		if (sumVolGlob <= 0) {
			throw CSmartException("IRegressionForcaster::NormalizeThetas: cannot normalize - sumGlob is zero");
		}

		/*for (const int assetId : *assetIdsPtr) {
			auto it = ws.find(assetId);
			it->second /= sumVolGlob;
		}*/
		for (auto pr : ws) {
			pr.second /= sumVolGlob;
		}

		for (auto wIt = ws.begin(); wIt != ws.end(); wIt++) {
			const int assetId = wIt->first;
			const double w = wIt->second;
			auto thIt = thetas.find(assetId);
			if (thIt == thetas.end()) {
				throw CSmartException("IRegressionForcaster::NormalizeThetas: couldn't find assetId");
			}
			auto thPtr = thIt->second;
			thPtr->at(0) = thPtr->at(0) * w;
			thPtr->at(1) = thPtr->at(1) * w;
		}
	}
}

double IRegressionForcaster::ForcastSalesIndependent(int assetId, double newPrice, double monthSince, int periodDays) {
	if (!isTaught) {
		throw CSmartException("IRegressionForcaster::ForcastSalesIndependent: model cannot forcast without being taught beforehand");
	}
	const auto it = thetas.find(assetId);
	if (it == thetas.end()) {
		throw CSmartException("IRegressionForcaster::ForcastSalesIndependent: assetId was not found");
	}
	const vector<double> thPars = *(it->second);
	//const time_t curTimeT = time(nullptr);
	//CDate curDt(curTimeT);
	//const double month = (double)curDt.Month() + ((double)curDt.Day() / 30.0);
	const double exVolume = Demand(newPrice, monthSince, periodDays, thPars);
	return exVolume;
}

//shared_ptr<CSupplementMatrix> IRegressionForcaster::GetSupplementMatrix() const {
//	return supMatrix;
//}

shared_ptr<map<int, double> > IRegressionForcaster::ForcastSales(map<int, double> & newPrices, double monthSince, int periodDays) {
	if (!isTaught) {
		throw CSmartException("IRegressionForcaster::ForcastSales: model cannot forecast without being taught beforehand");
	}
	auto exSales = make_shared<map<int, double>>();

	/*
	set<int> denseIds, sparseIds;
	for (const auto pr : newPrices) {
		const int assetId = pr.first;
		if (IsAssetDense(assetId)) denseIds.insert(assetId);
		else if (IsAssetSparse(assetId)) {
			sparseIds.insert(assetId);
			const double newPrice = pr.second;
			const double exVol = ForcastSalesIndependent(assetId, newPrice, monthSince, periodDays);
			exSales->insert(make_pair(assetId, exVol));
		}
		else throw CSmartException("IRegressionForcaster::ForcastSales: assetId is not dense and not sparse");
	}

	// sparse уже обработаны; далее обрабатываются dense
	const int n = (int)denseIds.size();
	dlib::matrix<double> M, Q;
	vector<int> assetIds(n, 0);	// содержит последовательность активов в матрице Q
	std::copy(denseIds.begin(), denseIds.end(), assetIds.begin());

	CAlgorithms::MakeMatrix(supMatrix, assetIds, M);
	Q = dlib::trans(M);

	dlib::matrix<double> curVolumes(1, n), curPrices(1, n);
	for (int i = 0; i < n; ++i) {
		const int assetId = assetIds[i];
		const double curPrice = curPriceList->GetPrice(assetId);
		curPrices(0, i) = curPrice;
		curVolumes(0, i) = this->ForcastSalesIndependent(assetId, curPrice, periodDays, 1.0);
	}
	*/
	for (const auto pr : newPrices) {
		const int assetId = pr.first;
		const double newPrice = pr.second;
		const double exVol = ForcastSalesIndependent(assetId, newPrice, monthSince, periodDays);
		exSales->insert(make_pair(assetId, exVol));
	}
	return exSales;
}

set<int> IRegressionForcaster::GetDenseAssetIds() const {
	return denseAssetIds;
}

set<int> IRegressionForcaster::GetSparseAssetIds() const {
	return sparseAssetIds;
}

void IRegressionForcaster::ChooseTopDenseAssets(int amountOfAssets) {
	if (supMatrix == nullptr) {
		throw CSmartException("IRegressionForcaster::ChooseTopDenseAssets: cannot chose dense assets as supMatrix is null");
	}
	supMatrix->ChooseTopDenseAssets(amountOfAssets, denseAssetIds);
	set<int> allAssetIds;
	supMatrix->FillAssetIds(allAssetIds);
	for (const int assetId : allAssetIds) {
		if (denseAssetIds.find(assetId) == denseAssetIds.end()) {
			sparseAssetIds.insert(assetId);
		}
	}
}

void IRegressionForcaster::RestoreMissingPrices(CMatrix& priceMatrix) const {
	const set<int> assetIds = priceMatrix.GetAssetIds();
	if (assetIds.size() != 1) {
		throw CSmartException("IRegressionForcaster::RestoreMissingPrices: amount of assets must be equal to one!");
	}
	const int assetId = *assetIds.begin();
	const int maxZeros = 30;
	shared_ptr<vector<double>> prs = const_pointer_cast<vector<double>>(priceMatrix.GetRow(assetId));
	if (prs == nullptr) {
		throw CSmartException("IRegressionForcaster::RestoreMissingPrices: assetId not found");
	}
	const int n = prs->size();
	int i = -1, lastI = -1;
	while (++i < n) {
		int zerosAmt = 0;
		while (abs(prs->at(i)) <= 0.0001) {
			zerosAmt++;
			i++;
			if (i >= n) break;
		}
		// i-ый элемент указывает не на ноль, количество подряд идущих нулей zerosAmt, последний не ноль - lastI;
		if ((zerosAmt <= maxZeros) and (zerosAmt > 0) and (lastI >= 0)) {
			const double lastPrice = prs->at(lastI);
			for (int j = lastI+1; j < i; j++) {
				prs->at(j) = lastPrice;
			}
		}
		lastI = i;
		//if (zerosAmt == 0) i++;
	}
}

void IRegressionForcaster::FillDenseAndSparseAssets(set<int>& denseIds, set<int>& sparseIds) const {
	denseIds.clear();
	sparseIds.clear();
	if (supMatrix == nullptr) {
		throw CSmartException("IRegressionForcaster::FillDenseAndSparseAssets: supMatrix is null, cannot fill dense and sparse assets");
	}
	if (denseAssetIds.size() + sparseAssetIds.size() == 0) {
		throw CSmartException("IRegressionForcaster::FillDenseAndSparseAssets: size of dense and sparse are null");
	}
	denseIds = denseAssetIds;
	sparseIds = sparseAssetIds;
}

bool IRegressionForcaster::IsAssetDense(int assetId) const {
	const auto it = denseAssetIds.find(assetId);
	return (it != denseAssetIds.end());
}

bool IRegressionForcaster::IsAssetSparse(int assetId) const {
	const auto it = sparseAssetIds.find(assetId);
	return (it != sparseAssetIds.end());
}

void IRegressionForcaster::MakeMatrix(const vector<int>& assetIds, dlib::matrix<double>& outputMatrix) const {
	CAlgorithms::MakeMatrix(supMatrix, assetIds, outputMatrix);
}
