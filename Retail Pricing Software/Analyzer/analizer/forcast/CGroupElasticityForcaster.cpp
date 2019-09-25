/*
 * CGroupElasticityForcaster.cpp
 *
 *  Created on: Jun 21, 2019
 *      Author: kraychik
 */

#include "CGroupElasticityForcaster.h"
#include "../algs/CAlgorithms.h"
//#include "../supply/CSmartException.h"
#include <algorithm>
#include <math.h>

CGroupElasticityForcaster::CGroupElasticityForcaster() {
	supMatrix = nullptr;
}

CGroupElasticityForcaster::~CGroupElasticityForcaster() {
	// TODO Auto-generated destructor stub
}

void CGroupElasticityForcaster::TeachModel(const set<int> & assetIdsToTeach, int periodDays, int amountOfDenseAssets) {
	const auto groupIds = params->GetAllGroups();
	//auto groupIds = make_shared<set<int>>();
	//groupIds->insert(2);
	//cout << groupIds->size() << endl;
	{
	shared_ptr<CSalesHistory> sH = const_pointer_cast<CSalesHistory>(salesHistory);
	sH->CalcAmountOfChecks();
	}
	for (const int groupId : *groupIds) {
		cout << "Teaching for Group: " << groupId << endl;
		std::map<int, shared_ptr<vector<double>>> csLocal;
		double aLocal = 0, bLocal = 0;
		TeachModelForGroup(groupId, assetIdsToTeach, periodDays, aLocal, bLocal, csLocal);
		//cout << "Locals for groupId = " << groupId << ": " << aLocal << " " << bLocal << " " << cLocal << endl;
		for (const auto pair : csLocal) {
			const int assetId = pair.first;
			const auto thVectPtr = pair.second;
			theta.insert(make_pair(assetId, make_tuple(aLocal, bLocal, thVectPtr->at(0), thVectPtr->at(1), thVectPtr->at(2))));
		}
	}
	CFilter filter(true);
	filter.SetAssetIds(assetIdsToTeach, false);
	const auto groupsInfo = params->CalcGroupsInfo();
	CFilterAdvanced aFilter(filter, groupsInfo);
	supMatrix = salesHistory->BuildSupplementMatrix(aFilter);
	isTaught = true;
	cout << this->ToString() << endl;
}

CGroupElasticityForcaster::CGroupElasticityForcaster(
		shared_ptr<const CSalesHistory> salesHistory,
		shared_ptr<const CPriceList> curPriceList,
		shared_ptr<const COptimizerParams> params) : IForcaster(salesHistory, curPriceList, params) {
	CFilter filter(true);
	const auto groupsInfo = params->CalcGroupsInfo();
	CFilterAdvanced aFilter(filter, groupsInfo);
	supMatrix = salesHistory->BuildSupplementMatrix(aFilter);
}

shared_ptr<map<int, double> > CGroupElasticityForcaster::ForcastSales(map<int, double>& newPrices, double monthSince, int periodDays) {
	if (!isTaught) {
		throw CSmartException("CGroupElasticityForcaster::ForcastSales: cannot forcast. Model is not taught!");
	}
	auto expectedSales = make_shared<map<int, double>>();
	for (const auto & pair : newPrices) {
		const int assetId = pair.first;
		expectedSales->insert(make_pair(assetId, 0.0));
	}
	for (const auto & pair : newPrices) {
		const int assetId = pair.first;
		const double newPrice = pair.second;
		input_vector priceVect(1, 1);
		priceVect = newPrice;
		const auto it = theta.find(assetId);
		if (it == theta.end()) {
			string str = "";
			str += "(" + to_string(assetId) + ", " + to_string(newPrice) + ") " + ToString();
			throw CSmartException(string("CGroupElasticityForcaster::ForcastSales: theta parameters (a, b, c) not found") + str);
			//cout << ToString() << endl;
		}
		const auto tup = it->second;
		const double a = get<0>(tup);
		const double b = get<1>(tup);
		const double c = get<2>(tup);
		parameter_vector thPars(3, 1);
		thPars = c, a, b;
		const double exSales = Demand(priceVect, thPars);

		input_vector curVect(1, 1);
		curVect = curPriceList->GetPrice(assetId);
		const double curSales = Demand(curVect, thPars);
		const double deltaV = exSales - curSales;

		map<int, double> deltaVolumesSup, deltaVolumeSubst;
		CalcSupplementCrossDemand(assetId, 0.3*deltaV, deltaVolumesSup);
		//CalcSubsituteCrossDemand(assetId, deltaV, deltaVolumeSubst);
		for (const auto & deltaPair : deltaVolumesSup) {
			auto resIt = expectedSales->find(deltaPair.first);
			resIt->second += deltaPair.second;
		}
		//expectedSales->insert(make_pair(assetId, exSales));
	}
	return expectedSales;
}

void CGroupElasticityForcaster::TeachModelForGroup(int groupId, const set<int> & assetIdsToTeach, int periodDays, double& a, double& b, std::map<int, shared_ptr<vector<double>>>& cs) {
	const auto assetIdsPtr = params->GetAssetsFromGroup(groupId);
	set<int> assetIdsToWork;
	set_intersection(assetIdsPtr->begin(), assetIdsPtr->end(), assetIdsToTeach.begin(), assetIdsToTeach.end(), inserter(assetIdsToWork, assetIdsToWork.begin()));
	//const int n = (const int)assetIdsPtr->size();		// amount of assets in a group
	set<int> emptySet;
	set<int> groupSet = {groupId};
	vector<interval> emptyVect;
	const auto groupsInfoPtr = params->CalcGroupsInfo();

	const map<time_t, int> amtOfChecks = salesHistory->GetDaysAmountOfChecks();
	CMatrix priceMatrix, volumeMatrix, incomeMatrix;
	//map<time_t, int> amtOfChecks;
	CFilter filter(emptyVect, assetIdsToWork, emptySet, emptySet, true, false, true, true);	// filter by allAssetsToWork
	CFilterAdvanced aFilter(filter, groupsInfoPtr);
	salesHistory->FillDaysStatistics(priceMatrix, volumeMatrix, incomeMatrix, aFilter);

	std::map<int, shared_ptr<vector<double>>> thetas;	// assetId -> (c, a, b)
	for (int assetId : assetIdsToWork) {
		set<int> assetSet = {assetId};
		//ble assetA = 0.0, assetB = 0.0, assetC = 0.0, assetD = 0.0, assetE = 0.0;
		//CMatrix priceMatrix, volumeMatrix, incomeMatrix;
		//CFilter filter(emptyVect, assetSet, emptySet, emptySet, true, false, true, true);	// filter by assetId only
		//CFilterAdvanced aFilter(filter, groupsInfoPtr);
		//salesHistory->FillDaysStatistics(priceMatrix, volumeMatrix, incomeMatrix, aFilter);
		CMatrix localPriceMatrix, localVolumeMatrix, localIncomeMatrix;
		priceMatrix.Filter(assetSet, localPriceMatrix);
		volumeMatrix.Filter(assetSet, localVolumeMatrix);
		incomeMatrix.Filter(assetSet, localIncomeMatrix);
		RestoreMissingPrices(priceMatrix);
		//cout << "Estimating Theta for ID = " << assetId << endl;
		vector<double> modelPars;
		EstimateThetaForOneAsset(localPriceMatrix, localVolumeMatrix, localIncomeMatrix, amtOfChecks, modelPars);
		//cout << "Theta Estimated for ID = " << assetId << endl;
		auto newTheta = make_shared<vector<double>>();
		newTheta->reserve(5);
		newTheta->push_back(modelPars[0]);
		newTheta->push_back(modelPars[1]);
		newTheta->push_back(modelPars[2]);
		newTheta->push_back(modelPars[3]);
		newTheta->push_back(modelPars[4]);
		thetas.insert(make_pair(assetId, newTheta));
	}

	RestoreMissingPrices(priceMatrix);
	//cout << volumeMatrix.ToString() << endl;

	map<int, double> ws;
	CalcAssetWeights(volumeMatrix, ws);

	// рассчитываем и взвешиваем параметры a и b
	a = b = 0;
	for (const auto & pr : ws) {
		const int assetId = pr.first;
		const double w = pr.second;
		const auto it = thetas.find(assetId);
		if (it == thetas.end()) {
			throw CSmartException("CGroupElasticityForcaster::TeachModelForGroup: assetId not found in thetas");
		}
		const auto theta = it->second;
		a += theta->at(0) * w;
		b += theta->at(1) * w;
	}

	for (const auto & pr : thetas) {
		const int assetId = pr.first;
		const auto theta = pr.second;
		const auto newPars = make_shared<vector<double>>();
		newPars->push_back(theta->at(2));
		newPars->push_back(theta->at(3));
		newPars->push_back(theta->at(4));
		cs.insert(make_pair(assetId, newPars));
	}
	/*
	const parameter_vector prs = 10*dlib::randm(n+2,1);
	cout << "inferred parameters: "<< trans(prs) << endl;
	std::vector<std::pair<input_vector, double> > data_samples;
    input_vector input;
    for (int i = 0; i < 1000; ++i) {
    	input = 10*dlib::randm(n,1);
    	const double output = Model(input, prs);
    	data_samples.push_back(make_pair(input, output));
    }
    parameter_vector x(n+2, 1);
    x = 1;
    SolveTech(data_samples, x, n);
    cout << "inferred parameters: "<< trans(x) << endl;
	cout << "solution error:      "<< length(x - prs) << endl;
	cout << endl;
	*/
}
template<int n>
void CGroupElasticityForcaster::SolveLeastSquares(const std::vector<std::pair<input_vector, double> >& data_samples, parameter_vector& x) const {
	map<double, double> dataSample;
	for (const auto & pair : data_samples) {
		const double p = pair.first(0);
		const double v = pair.second;
		dataSample.insert(make_pair(p, v));
	}
	vector<double> aVect = {0, 0, 0}, ths;
	vector<double> bVect = {100, 4, 4};
	CAlgorithms::LeastSquares(dataSample, DemandUnq, aVect, bVect, ths);
	parameter_vector xx(3, 1);
	xx = ths[0], ths[1], ths[2];
	//dlib::matrix<double, n+2, 1> xx;
	//xx = 1;
	//dlib::solve_least_squares_lm(dlib::objective_delta_stop_strategy(1e-7).be_verbose(), Residual, dlib::derivative(Residual), data_samples, xx);
	//dlib::solve_least_squares_lm(dlib::objective_delta_stop_strategy(1e-7).be_verbose(), Residual, ResidualDerivative, data_samples, xx);
	x = xx;
}
// params: { c, a, b }
double CGroupElasticityForcaster::Demand(const input_vector& input, const parameter_vector& params) {
	/*const int n = (int)input.size();
	double sum = 0.0;
	for (int i = 0; i < n; ++i) {
		sum += input(i) * params(i);
	}
	sum /= (params(n) + params(n+1));
	return sum;*/
	const double c = params(2);
	const double a = params(0);
	const double b = params(1);
	const double p = input(0);
	const double expV = exp(-(a+b*p));
	return c * expV / (1 + expV);
}
double CGroupElasticityForcaster::Residual(const std::pair<input_vector, double>& data, const parameter_vector& params) {
	return Demand(data.first, params) - data.second;
}

/*
void CGroupElasticityForcaster::EstimateThetaForOneAsset(const CMatrix& priceMatrix,const CMatrix& volumeMatrix, const CMatrix& incomeMatrix, const map<time_t, int>& amtOfChecks, double& a, double& b, double& c) const {
	std::vector<std::pair<input_vector, double>> dataSamples;
	const auto dts = priceMatrix.GetTimes();
	const auto assetIds = priceMatrix.GetAssetIds();
	if (assetIds.size() != 1) {
		throw CSmartException("CGroupElasticityForcaster::EstimateThetaForOneAsset: assetIds size not equal to one");
	}
	const int assetId = *assetIds.begin();
	for (time_t dt : dts) {
		const double price = priceMatrix.GetValue(assetId, dt);
		const double volume = volumeMatrix.GetValue(assetId, dt);
		input_vector pv(1, 1);
		pv = price;
		dataSamples.push_back(std::pair<input_vector, double>(pv, volume));
	}
	parameter_vector theta(3, 1);
	theta = 1.0, 0.3, 0.3;	// c, a, b
	//theta = 1;
	//for (auto pair : dataSamples) {
	//	const double price = pair.first(0);
	//	const double volume = pair.second;
	//	cout << "(" << price << ", " << volume << ") " << endl;
	//}
	FilterDataSamples(dataSamples);		// удаляет все 0
	//cout << "New DataSamples" << endl;
	//for (auto pair : dataSamples) {
	//	const double price = pair.first(0);
	//	const double volume = pair.second;
	//	cout << "(" << price << ", " << volume << ") " << endl;
	//}
	SolveTech(dataSamples, theta, 1);
	c = theta(0); a = theta(1); b = theta(2);
}
*/

void CGroupElasticityForcaster::EstimateThetaForOneAsset(const CMatrix& priceMatrix,const CMatrix& volumeMatrix, const CMatrix& incomeMatrix, const map<time_t, int>& amtOfChecks, vector<double> & modelPars) const {
	const auto dts = priceMatrix.GetTimes();
	const auto assetIds = priceMatrix.GetAssetIds();
	if (assetIds.size() != 1) {
		throw CSmartException("CGroupElasticityForcaster::EstimateThetaForOneAsset: assetIds size not equal to one");
	}
	const int assetId = *assetIds.begin();
	const int n = dts.size();
	vector<double> prices(n, 0.0), ysTmp(n, 0.0);
	vector<int> dtMonthNumbers(n, 0);
	int i = 0;
	for (time_t dt : dts) {
		const double price = priceMatrix.GetValue(assetId, dt);
		const double volume = volumeMatrix.GetValue(assetId, dt);
		const auto itChecks = amtOfChecks.find(dt);
		if (itChecks == amtOfChecks.end()) {
			throw CSmartException("CGroupElasticityForcaster::EstimateThetaForOneAsset: itCheck not found in amtOfChecks");
		}
		const int checksAmt = itChecks->second;
		if (checksAmt == 0) {
			throw CSmartException("CGroupElasticityForcaster::EstimateThetaForOneAsset: checksAmt equals to zero");
		}
		int dtMonthNumber = CDate::MonthNumberOnly(dt);
		//xs[i] = price;
		//dtMonthNumbers[i] = dtMonthNumber;
		if (abs(price) > 0.001) {
			prices[i] = price;
			dtMonthNumbers[i] = dtMonthNumber;
			ysTmp[i] = volume / checksAmt;
			i++;
		}
	}
	const int m = i;
	dlib::matrix<double> xs(2, m);
	vector<double> ys(m);
	for (int i = 0; i < m; ++i) {
		xs(0, i) = prices[i];
		xs(1, i) = static_cast<double>(dtMonthNumbers[i]);
		ys[i] = ysTmp[i];
	}
	//FilterDataSamples(xs, ys);
	//bool hasErrorInPrices = any_of(xs.begin(), xs.end(), [](double p) { return abs(p) <= 0.001; } );
	for (int i = 0; i < m; ++i) {
		if (abs(xs(0, i) <= 0.001)) {
			throw CSmartException("CGroupElasticityForcaster::EstimateThetaForOneAsset: prices should't contain zeros!");
		}
	}
	vector<double> aVect = {0.0, 0.0, 0.00, 0.0, 0.00};
	vector<double> bVect = {3.0, 3.0, 3.0, 1.0, 1.0/6.0};
	vector<double> resTheta;
	function<double(const vector<double> &)> f = [&xs, &ys, this](const vector<double> &thPars) {
		const int n = (int)ys.size();
		double s = 0.0;
		for (int i = 0; i < n; ++i) {
			const double x = DemandPair(xs(0, i), xs(1, i), thPars);
			const double y = ys[i];
			const double tmp = y - x;
			s += tmp * tmp;
		}
		return s;
	};
	CAlgorithms::OptimizeFunctionGlobal(f, aVect, bVect, resTheta, CAlgorithms::TargetObjective::Min, 200);
	//CAlgorithms::LeastSquaresDerivative(xs, ys, DemandVect, aVect, bVect, resTheta, 50);
	//a = resTheta[0]; b = resTheta[1]; c = resTheta[2]; d = resTheta[3]; e = resTheta[4];
	modelPars.clear();
	modelPars = resTheta;
}

void CGroupElasticityForcaster::SolveTech(const std::vector<std::pair<input_vector, double> >& data_samples, parameter_vector& x, int n) const {
	switch (n) {
	case 1: SolveLeastSquares<1>(data_samples, x); break;
	/*case 2: SolveLeastSquares<2>(data_samples, x); break;
	case 3: SolveLeastSquares<3>(data_samples, x); break;
	case 4: SolveLeastSquares<4>(data_samples, x); break;
	case 5: SolveLeastSquares<5>(data_samples, x); break;
	case 6: SolveLeastSquares<6>(data_samples, x); break;
	case 7: SolveLeastSquares<7>(data_samples, x); break;
	case 8: SolveLeastSquares<8>(data_samples, x); break;
	case 9: SolveLeastSquares<9>(data_samples, x); break;
	case 10: SolveLeastSquares<10>(data_samples, x); break;
	case 11: SolveLeastSquares<11>(data_samples, x); break;
	case 12: SolveLeastSquares<12>(data_samples, x); break;
	case 13: SolveLeastSquares<13>(data_samples, x); break;
	case 14: SolveLeastSquares<14>(data_samples, x); break;
	case 15: SolveLeastSquares<15>(data_samples, x); break;
	case 16: SolveLeastSquares<16>(data_samples, x); break;
	case 17: SolveLeastSquares<17>(data_samples, x); break;
	case 18: SolveLeastSquares<18>(data_samples, x); break;
	case 19: SolveLeastSquares<19>(data_samples, x); break;
	case 20: SolveLeastSquares<20>(data_samples, x); break;
	case 21: SolveLeastSquares<21>(data_samples, x); break;
	case 22: SolveLeastSquares<22>(data_samples, x); break;
	case 23: SolveLeastSquares<23>(data_samples, x); break;
	case 24: SolveLeastSquares<24>(data_samples, x); break;
	case 25: SolveLeastSquares<25>(data_samples, x); break;
	case 26: SolveLeastSquares<26>(data_samples, x); break;
	case 27: SolveLeastSquares<27>(data_samples, x); break;
	case 28: SolveLeastSquares<28>(data_samples, x); break;
	case 29: SolveLeastSquares<29>(data_samples, x); break;
	case 30: SolveLeastSquares<30>(data_samples, x); break;
	case 31: SolveLeastSquares<31>(data_samples, x); break;
	case 32: SolveLeastSquares<32>(data_samples, x); break;
	case 33: SolveLeastSquares<33>(data_samples, x); break;
	case 34: SolveLeastSquares<34>(data_samples, x); break;
	case 35: SolveLeastSquares<35>(data_samples, x); break;
	case 36: SolveLeastSquares<36>(data_samples, x); break;
	case 37: SolveLeastSquares<37>(data_samples, x); break;
	case 38: SolveLeastSquares<38>(data_samples, x); break;
	case 39: SolveLeastSquares<39>(data_samples, x); break;
	case 40: SolveLeastSquares<40>(data_samples, x); break;*/
	default: throw CSmartException("CGroupElasticityForcaster::SolveTech: no case defined for n > 40");
	}
}

string CGroupElasticityForcaster::ToString() const {
	string res = "";
	for (const auto & pair : theta) {
		const int assetId = pair.first;
		const auto tup = pair.second;
		const double a = get<0>(tup);
		const double b = get<1>(tup);
		const double c = get<2>(tup);
		const double d = get<3>(tup);
		const double e = get<4>(tup);
		res += to_string(assetId) + ": " + to_string(a) + ", " + to_string(b) + ", " + to_string(c) + ", "
				+ to_string(d) + ", " + to_string(e) + "\n";
	}
	return res;
}

// params: { c, a, b }
parameter_vector CGroupElasticityForcaster::ResidualDerivative(const std::pair<input_vector, double>& data, const parameter_vector& params) {
	parameter_vector der(3, 1);
	const double c = params(0);
	const double a = params(1);
	const double b = params(2);
	const double p = data.first(0);
	const double expV = exp(a+b*p);
	const double expVNeg = 1 / expV;
	der(0) = expV / (1+expV);	// derivative by c
	const double znam = (expVNeg+1)*(expVNeg+1);
	der(1) = c * exp(a-1)*(expVNeg+a+1) / (znam);	// derivative by a
	der(2) = -c * exp(a-1)*(b*p*expVNeg-expVNeg-1) / (znam);	// derivative by b
	return der;
}

void CGroupElasticityForcaster::RestoreMissingPrices(CMatrix& priceMatrix) const {
	const auto assetIds = priceMatrix.GetAssetIds();
	for (const int assetId : assetIds) {
		RestoreMissingPricesForAsset(priceMatrix, assetId);
	}
}

void CGroupElasticityForcaster::RestoreMissingPricesForAsset(CMatrix& priceMatrix, int assetId, int maxZeros) const {
	shared_ptr<vector<double>> prs = const_pointer_cast<vector<double>>(priceMatrix.GetRow(assetId));
	if (prs == nullptr) {
		throw CSmartException("CGroupElasticityForcaster::RestoreMissingPricesForAsset: assetId not found");
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

void CGroupElasticityForcaster::CalcAssetWeights(const CMatrix& volumeMatrix, map<int, double>& ws) const {
	ws.empty();
	const auto assetIds = volumeMatrix.GetAssetIds();
	for (const int assetId : assetIds) {
		const double s = volumeMatrix.SumByAssetId(assetId);
		ws.insert(make_pair(assetId, s));
	}
	double s = 0.0;
	for (const auto & pr : ws) {
		s += pr.second;
	}
	for (auto it = ws.begin(); it != ws.end(); it++) {
		it->second /= s;
	}
}

vector<pair<input_vector, double> > CGroupElasticityForcaster::CreateDataSample(const vector<double>& prices, const vector<double>& volumes) {
	if (prices.size() != volumes.size()) {
		throw CSmartException("CGroupElasticityForcaster::CreateDataSample: prices size not equal to volume size");
	}
	vector<pair<input_vector, double> > dataSamples;
	for (int i = 0; i < (int)prices.size(); i++) {
		input_vector priceVect;
		priceVect = prices[i];
		dataSamples.push_back(make_pair(priceVect, volumes[i]));
	}
	return dataSamples;
}

double CGroupElasticityForcaster::DemandUnq(const double p, const vector<double>& params) {
	input_vector pVect(1, 1);
	pVect = p;
	parameter_vector ths(3, 1);
	ths = params[0], params[1], params[2];
	return Demand(pVect, ths);
}
double CGroupElasticityForcaster::DemandVect(const vector<double> & inputVar, const vector<double>& params) {
	const double a = params[0];
	const double b = params[1];
	const double c = params[2];
	const double p = inputVar[0];
	const double month = inputVar[1];
	const double tmp = exp(-(a+b*p));
	return c * tmp / (1 + tmp);
}
double CGroupElasticityForcaster::DemandPair(const double p, const double month, const vector<double>& params) {
	const double a = params[0];
	const double b = params[1];
	const double c = params[2];
	const double d = params[3];
	const double e = params[4];
	const double tmp = exp(-(a+b*p));
	const double oldRes = c * tmp / (1 + tmp);
	const double sn = sin(d * 3.14159265 + e * 3.14159265 * month);
	return oldRes * (sn * sn + 0.8);
}
void CGroupElasticityForcaster::CalcSupplementCrossDemand(int assetId, double deltaV, map<int, double>& deltaVs) const {
	supMatrix->MultValueByRowAsset(assetId, deltaV, deltaVs);
}

void CGroupElasticityForcaster::SaveModel() const {

}

void CGroupElasticityForcaster::FilterDataSamples(std::vector<std::pair<input_vector, double> >& dataSamples) const {
	if (dataSamples.size() <= 0) return;
	const int n = dataSamples[0].first.nr();
	if (n != 1) {
		throw CSmartException("CGroupElasticityForcaster::FilterDataSamples: n not equals to 1 - not implemented yet");
	}
	vector<int> idsToDelete;
	const int m = dataSamples.size();
	idsToDelete.reserve(m);
	for (int i = 0; i < m; ++i) {
		const auto pair = dataSamples[i];
		const double price = pair.first(0);
		if (abs(price) <= 0.001) {
			idsToDelete.push_back(i);
		}
	}
	for (auto it = idsToDelete.rbegin(); it != idsToDelete.rend(); it++) {
		const int index = *it;
		//cout << "Deleting item: Price = " << dataSamples[index].first(0) << "; Volume = " << dataSamples[index].second << endl;
		dataSamples.erase(dataSamples.begin() + index);
	}
}

shared_ptr<CSupplementMatrix> CGroupElasticityForcaster::GetSupplementMatrix() const {
	return supMatrix;
}

void CGroupElasticityForcaster::FilterDataSamples(vector<double>& xs, vector<int> & dtMonths, vector<double>& ys) const {
	const vector<double> copyXs = xs, copyYs = ys;
	const vector<int> copyDtMonths = dtMonths;
	xs.clear(); ys.clear(); dtMonths.clear();
	const int n = (int)copyXs.size();
	for (int i = 0; i < n; ++i) {
		if (abs(copyXs[i]) <= 0.001) continue;
		xs.push_back(copyXs[i]);
		ys.push_back(copyYs[i]);
		dtMonths.push_back(copyDtMonths[i]);
	}
}

double CGroupElasticityForcaster::ForcastSalesIndependent(int assetId, double newPrice, double month, int periodDays) {
	const auto it = theta.find(assetId);
	if (it == theta.end()) {
		throw CSmartException("CGroupElasticityForcaster::ForcastSalesIndependent: cannot forcast - assetId not found");
	}
	const double a = get<0>(it->second);
	const double b = get<1>(it->second);
	const double c = get<2>(it->second);
	const double d = get<3>(it->second);
	const double e = get<4>(it->second);
	const vector<double> thParams = {a, b, c, d, e};
	//const double curPrice = curPriceList->GetPrice(assetId);
	//const double curVolume = DemandUnq(curPrice, thParams);
	//const double exVolume = DemandUnq(newPrice, thParams);
	month = 8.0;
	const double exVolume = DemandPair(newPrice, month, thParams);
	//return exVolume - curVolume;
	return exVolume;
}
