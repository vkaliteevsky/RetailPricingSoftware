/*
 * CTopAssetsTask.cpp
 *
 *  Created on: Feb 21, 2019
 *      Author: kraychik
 */

#include "CTopAssetsTask.h"

CTopAssetsTask::CTopAssetsTask(int projectId, int calcId, const CFilter & filter, int amount, KPI kpi)
	: CReportTask(projectId, calcId, filter)
{
	if (amount < 0) {
		throw CSmartException("CTopAssetsTask::CTopAssetsTask: amount is negative! Cannot create object.");
	}
	this->amount = amount;
	this->kpi = kpi;
}

CTopAssetsTask::~CTopAssetsTask() {
	// TODO Auto-generated destructor stub
}

int CTopAssetsTask::TypeId() const {
	return 7;
}
