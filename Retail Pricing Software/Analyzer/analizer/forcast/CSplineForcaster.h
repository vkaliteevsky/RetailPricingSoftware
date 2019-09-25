/*
 * CSplineForcaster.h
 *
 *  Created on: Aug 6, 2019
 *      Author: kraychik
 */

#ifndef ANALIZER_FORCAST_CSPLINEFORCASTER_H_
#define ANALIZER_FORCAST_CSPLINEFORCASTER_H_

#include "IRegressionForcaster.h"
#include "../algs/spline.h"

class CSplineForcaster: public IRegressionForcaster {
public:
	CSplineForcaster();
	CSplineForcaster(shared_ptr<const CSalesHistory> salesHistory, shared_ptr<const CPriceList> curPriceList, shared_ptr<const COptimizerParams> params);
	virtual ~CSplineForcaster();

	virtual double Demand(const double p, const double monthSince, const int periodDays, const vector<double>& params) const override;
protected:
	virtual vector<double> ThetaLowerLimits() const override;
	virtual vector<double> ThetaUpperLimits() const override;
};

#endif /* ANALIZER_FORCAST_CSPLINEFORCASTER_H_ */
