/*
 * ResultManager.h
 *
 *  Created on: Jan 20, 2019
 *      Author: kraychik
 */

#ifndef PROCESSING_CRESULTMANAGER_H_
#define PROCESSING_CRESULTMANAGER_H_
#include <memory>
#include <set>
#include <map>
#include <mutex>
#include <ctime>
#include <thread>
#include <functional>
#include <list>
#include "CCalcResult.h"
#include "../supply/CSmartException.h"
#include "CCalcIdsManager.h"

class CResultManager {
public:
	CResultManager();
	static CResultManager *GetInstance();
	void AddResult(std::shared_ptr<const CCalcResult> calcResult);
	std::shared_ptr<const CCalcResult> GetResult(int calcId) const;
	std::shared_ptr<const CCalcResult> EraseResult(int calcId);
	void EraseAll();
	void EraseOldResults();
	virtual ~CResultManager();
	std::string ToString() const;
private:
	void SetTimer();
	bool IsTimeToErase(int calcId) const;
	static CResultManager instance;
	std::map<int, std::shared_ptr<const CCalcResult>> mapCalcResult;
	std::map<int, time_t> mapTimeOfResult;
	mutable std::mutex mutex;
	const int delayBeforeErase = 60 * 1000;
	const int delayTimer = 2000;
};

#endif /* PROCESSING_CRESULTMANAGER_H_ */
