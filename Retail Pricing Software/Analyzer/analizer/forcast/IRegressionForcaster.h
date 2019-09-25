/*
 * IRegressionForcaster.h
 *
 *  Created on: Aug 6, 2019
 *      Author: kraychik
 */

#ifndef ANALIZER_FORCAST_IREGRESSIONFORCASTER_H_
#define ANALIZER_FORCAST_IREGRESSIONFORCASTER_H_

#include "IForcaster.h"
#include <vector>

#include "../algs/CAlgorithms.h"

class IRegressionForcaster : public IForcaster {
public:
	IRegressionForcaster();
	IRegressionForcaster(shared_ptr<const CSalesHistory> salesHistory, shared_ptr<const CPriceList> curPriceList, shared_ptr<const COptimizerParams> params);
	virtual ~IRegressionForcaster();

	virtual void TeachModel(const set<int> & assetIds, int periodDays, int amountOfDenseAssets) override;
	virtual double ForcastSalesIndependent(int assetId, double newPrice, double monthSince, int periodDays) override;
	virtual shared_ptr<map<int, double>> ForcastSales(map<int, double> & newPrices, double monthSince, int periodDays) override;
	virtual double Demand(const double p, const double monthSince, const int periodDays, const vector<double>& params) const = 0;

	shared_ptr<vector<double>> GetTheta(int assetId) const;
	void FillDenseAndSparseAssets(set<int> & denseIds, set<int> & sparseIds) const;
	set<int> GetDenseAssetIds() const;
	set<int> GetSparseAssetIds() const;

	bool IsAssetDense(int assetId) const;
	bool IsAssetSparse(int assetId) const;
	void MakeMatrix(const vector<int> & assetIds, dlib::matrix<double> & outputMatrix) const;

protected:
	virtual vector<double> ThetaLowerLimits() const = 0;
	virtual vector<double> ThetaUpperLimits() const = 0;

	virtual void NormalizeThetas(shared_ptr<const CGroupsInfo> groupsInfo, const CMatrix &priceMatrix, const CMatrix &volumeMatrix);
	void ChooseTopDenseAssets(int amountOfAssets);
	virtual void EstimateThetaForOneAsset(const CMatrix &priceMatrix, const CMatrix &volumeMatrix, const CMatrix &incomeMatrix, const map<time_t, int> &amtOfChecks, vector<double> & params);
	virtual void FilterAndPrepareDataSet(const CMatrix &priceMatrix, const CMatrix &volumeMatrix, const CMatrix &incomeMatrix, const map<time_t, int> &amtOfChecks, dlib::matrix<double> & xs, vector<double> & ys);
	virtual void RestoreMissingPrices(CMatrix &priceMatrix) const;
	map<int, shared_ptr<vector<double>>> thetas;
	shared_ptr<CSupplementMatrix> supMatrix;

private:
	set<int> denseAssetIds;		// активы, имеющие перекрестную эластичность
	set<int> sparseAssetIds;	// активы, не зависящие от других товаров
};

#endif /* ANALIZER_FORCAST_IREGRESSIONFORCASTER_H_ */
