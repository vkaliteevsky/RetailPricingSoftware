/*
 * CCalcResult.h
 *
 *  Created on: Jan 20, 2019
 *      Author: kraychik
 */

#ifndef PROCESSING_CCALCRESULT_H_
#define PROCESSING_CCALCRESULT_H_
#include <vector>
#include <string>
#include <time.h>

enum class CalcResultStatus { ERROR, WAIT, SUCCESS };
class CCalcResult {
public:
	CCalcResult();
	CCalcResult(int projectId, int calcId);
	CCalcResult(int projectId, int calcId, const std::string & resultString);
	CCalcResult(int projectId, int calcId, const std::string & resultString, CalcResultStatus status);
	virtual ~CCalcResult();
	int calcId = 0;
	int projectId = 0;
	std::string resultString;
	CalcResultStatus status = CalcResultStatus::SUCCESS;
	virtual std::string ToString() const;

};

#endif /* PROCESSING_CCALCRESULT_H_ */
