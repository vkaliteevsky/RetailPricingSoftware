/*
 * CGroupElasticityForcaster.h
 *
 *  Created on: Jun 21, 2019
 *      Author: kraychik
 */

#ifndef ANALIZER_FORCAST_CGROUPELASTICITYFORCASTER_H_
#define ANALIZER_FORCAST_CGROUPELASTICITYFORCASTER_H_

#include "IForcaster.h"
#include <tuple>

#include <dlib/optimization.h>
//using namespace dlib;
typedef dlib::matrix<double> input_vector;
typedef dlib::matrix<double> parameter_vector;

class CGroupElasticityForcaster: public IForcaster {
public:
	CGroupElasticityForcaster();
	CGroupElasticityForcaster(shared_ptr<const CSalesHistory> salesHistory, shared_ptr<const CPriceList> curPriceList, shared_ptr<const COptimizerParams> params);
	virtual ~CGroupElasticityForcaster();

	virtual void TeachModel(const set<int> & assetIds, int periodDays, int amountOfDenseAssets) override;
	virtual shared_ptr<map<int, double>> ForcastSales(map<int, double> & newPrices, double monthSince, int periodDays) override;
	virtual string ToString() const;
	void RestoreMissingPrices(CMatrix & priceMatrix) const;
	void RestoreMissingPricesForAsset(CMatrix & priceMatrix, int assetId, int maxZeros = 20) const;
	template<int n>
	void SolveLeastSquares(const std::vector<std::pair<input_vector, double> >& data_samples, parameter_vector& x) const;
	static vector<pair<input_vector, double>> CreateDataSample(const vector<double> & prices, const vector<double> & volumes);
	virtual void SaveModel() const override;
	shared_ptr<CSupplementMatrix> GetSupplementMatrix() const;
	virtual double ForcastSalesIndependent(int assetId, double newPrice, double month, int periodDays) override;
protected:
	void TeachModelForGroup(int groupId, const set<int> & assetIdsToTeach, int periodDays, double &a, double &b, map<int, shared_ptr<vector<double>>>& cs);
	void EstimateThetaForOneAsset(const CMatrix &priceMatrix, const CMatrix &volumeMatrix, const CMatrix &incomeMatrix, const map<time_t, int> &amtOfChecks, vector<double> & modelPars) const;
	void CalcSupplementCrossDemand(int assetId, double deltaV, map<int, double> & deltaVs) const;
private:
	static double Demand(const input_vector& input, const parameter_vector& params);
	static double DemandUnq(const double p, const vector<double> & params);
	static double DemandVect(const vector<double> & inputVect, const vector<double>& params);
	static double DemandPair(const double p, const double month, const vector<double>& params);
	static double Residual(const std::pair<input_vector, double>& data, const parameter_vector& params);
	static parameter_vector ResidualDerivative(const std::pair<input_vector, double>& data, const parameter_vector& params);
	void SolveTech(const std::vector<std::pair<input_vector, double> >& data_samples, parameter_vector& x, int n) const;
	void CalcAssetWeights(const CMatrix &volumeMatrix, map<int, double> & ws) const;
	void FilterDataSamples(std::vector<std::pair<input_vector, double>> & dataSamples) const;
	void FilterDataSamples(vector<double> &xs, vector<int> & dtMonths, vector<double> &ys) const;

	map<int, tuple<double, double, double, double, double>> theta;	 // assetId -> (a, b, c)
	shared_ptr<CSupplementMatrix> supMatrix;
};

#endif /* ANALIZER_FORCAST_CGROUPELASTICITYFORCASTER_H_ */
