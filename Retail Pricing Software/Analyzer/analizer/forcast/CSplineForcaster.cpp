/*
 * CSplineForcaster.cpp
 *
 *  Created on: Aug 6, 2019
 *      Author: kraychik
 */

#include "CSplineForcaster.h"

CSplineForcaster::CSplineForcaster() : IRegressionForcaster() {
}

CSplineForcaster::CSplineForcaster(shared_ptr<const CSalesHistory> salesHistory,
		shared_ptr<const CPriceList> curPriceList,
		shared_ptr<const COptimizerParams> params)
	: IRegressionForcaster(salesHistory, curPriceList, params) {
}

CSplineForcaster::~CSplineForcaster() {
}

double CSplineForcaster::Demand(const double p, const double monthSince, const int periodDays, const vector<double>& params) const {
	const double a = params[0];
	const double b = params[1];
	const double c = params[2];
	const double expV = exp(-(a+b*p));
	const double priceEffect = c * expV / (1 + expV);

	const vector<double> splineX = { 1.5, 4.5, 7.5, 10.5, 13.5 };
	const vector<double> splineY = { params[3], params[4], params[5], params[6], params[3] };
	tk::spline sp;
	sp.set_points(splineX, splineY);

	double s = 0.0;
	for (int i = 1; i <= periodDays; ++i) {
		const double delta = ((double)i) / 30.0;
		double mn = monthSince + delta;
		if (mn >= 13.5) mn -= 12.0;
		const double seasonEffect = sp(mn);
		s += seasonEffect;
	}
	if (s <= 0.0) s = 1.0;
	return s * priceEffect;
}

vector<double> CSplineForcaster::ThetaLowerLimits() const {
	vector<double> pars = {0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5 };
	return pars;
}

vector<double> CSplineForcaster::ThetaUpperLimits() const {
	vector<double> pars = {3.0, 3.0, 10.0, 1.5, 1.5, 1.5, 1.5 };
	return pars;
}
