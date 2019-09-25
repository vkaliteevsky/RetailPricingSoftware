/*
 * CCalcResult.cpp
 *
 *  Created on: Jan 20, 2019
 *      Author: kraychik
 */

#include "CCalcResult.h"

CCalcResult::CCalcResult() {
	// TODO Auto-generated constructor stub

}

CCalcResult::CCalcResult(int projectId, int calcId) {
	this->projectId = projectId;
	this->calcId = calcId;
}

CCalcResult::CCalcResult(int projectId, int calcId, const std::string & resultString)
 : CCalcResult (projectId, calcId) {
	this->resultString = resultString;
}

CCalcResult::~CCalcResult() {
	// TODO Auto-generated destructor stub
}

CCalcResult::CCalcResult(int projectId, int calcId, const std::string& resultString, CalcResultStatus status)
	: CCalcResult(projectId, calcId, resultString)
{
	this->status = status;
}

std::string CCalcResult::ToString() const {
	std::string res = "ProjectId: " + std::to_string(projectId) + "; ";
	res += "CalcId: " + std::to_string(calcId) + "\n";
	res += "Response: " + resultString + "\n";
	return res;
}
