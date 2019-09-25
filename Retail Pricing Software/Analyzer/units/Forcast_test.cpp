/*
 * Forcast_test.cpp
 *
 *  Created on: Jul 9, 2019
 *      Author: kraychik
 */

#include "gtest/gtest.h"
#include "CSalesHistory.h"
#include "CDataBase.h"
#include "CMatrix.h"
#include <numeric>
#include <memory>
#include "forcast/CGroupElasticityForcaster.h"
#include "CElastPriceOptimizer.h"
#include "../storage/CDataManager.h"
#include "algs/CAlgorithms.h"
#include <time.h>

#include <dlib/optimization.h>

using namespace std;
TEST(Dlib, SolveLeastSquares) {

	vector<double> prices =  {0, 1, 2, 3, 4, 5};
	vector<double> volumes = {1, 3, 5, 7, 9, 11};

	/*vector<double> a = {0, 0};
	vector<double> b = {100, 100};
	map<double, double> datS;
	for (int i = 0; i < (int)prices.size(); ++i) {
		datS.insert(make_pair(prices[i], volumes[i]));
	}
	vector<double> theta;
	CAlgorithms::LeastSquares(datS, testF, a, b, theta);
	cout << "Result Theta: " << endl;
	for (int i = 0; i < theta.size(); i++) {
		cout << theta[i] << " ";
	}
	cout << endl;
	*/

	auto dataSamples = CGroupElasticityForcaster::CreateDataSample(prices, volumes);
	auto projectInfo = CDataManager::GetInstance()->GetOrLoadProjectInfo(2);
	auto salesHistory = projectInfo->LoadSalesHistory();
	auto curPriceList = projectInfo->GetCurrentPriceList();
	auto params = projectInfo->GetOrLoadOptimizerParams();
	const auto groupsInfo = params->CalcGroupsInfo();
	auto forcaster = make_shared<CGroupElasticityForcaster>(salesHistory, curPriceList, params);
	CElastPriceOptimizer priceOptimizer(2, salesHistory, curPriceList, params, forcaster);
	//auto forcaster = static_pointer_cast<CGroupElasticityForcaster>(priceOptimizer.GetForcaster());

	const auto optPriceList = priceOptimizer.CalcOptimalPriceList(8.3, 1, 0);
	//cout << "Optimal Price List: " << endl;
	//cout << optPriceList->ToString() << endl;
/*
	parameter_vector x; x = 1;
	cout << "Forcasting ... " << endl;
	//forcaster->SolveLeastSquares<1>(dataSamples, x);
	const auto assetIds = salesHistory->GetAllAssets();
	map<int, double> newPrices;
	for (const int assetId : assetIds) {
		const int groupId = groupsInfo->GetGroupByAsset(assetId);
		if (groupId == 2) {
			newPrices.insert(make_pair(assetId, curPriceList->GetPrice(assetId)+5.0));
		}
	}
	forcaster->TeachModel(1);
	const auto exSales = forcaster->ForcastSales(newPrices, 1, 1.0);
	for (const auto & pr : *exSales) {
		cout << "(" << pr.first << ", " << pr.second << ") ";
	}
	cout << endl;
	//cout << x << endl;
	 *
	 */
}
TEST(Dlib, CommonTest) {
	const int companyId = 2;
	auto projectInfo = CDataManager::GetInstance()->GetOrLoadProjectInfo(companyId);
	auto salesHistory = projectInfo->LoadSalesHistory();
	//salesHistory->SetGroupsInfo(groupsInfo)
	auto curPriceList = projectInfo->GetCurrentPriceList();
	auto params = projectInfo->GetOrLoadOptimizerParams();
	const auto groupsInfoPtr = params->CalcGroupsInfo();
	auto forcaster = make_shared<CGroupElasticityForcaster>(salesHistory, curPriceList, params);
	CElastPriceOptimizer priceOptimizer(companyId, salesHistory, curPriceList, params, forcaster);
	//cout << "Income: " << salesHistory->Income() << endl;

	//auto forcaster = static_pointer_cast<CGroupElasticityForcaster>(priceOptimizer.GetForcaster());

	vector<interval> vect;
	set<int> emptySet;
	set<int> assetSet = {2};
	set<int> groupSet;
	CMatrix priceMatrix, volumeMatrix, incomeMatrix;
	map<time_t, int> amtOfChecks;
	CFilter filter(vect, assetSet, groupSet, emptySet, true, false, true, true);	// filter by groupId only
	CFilterAdvanced aFilter(filter, groupsInfoPtr);
	salesHistory->FillDaysStatistics(priceMatrix, volumeMatrix, incomeMatrix, aFilter);
	//cout << priceMatrix.ToString() << endl;
	//forcaster->RestoreMissingPrices(priceMatrix);
	//cout << priceMatrix.ToString() << endl;
	//forcaster->TeachModel(30);
}
