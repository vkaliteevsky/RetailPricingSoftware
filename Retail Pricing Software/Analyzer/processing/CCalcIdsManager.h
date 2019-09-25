/*
 * CCalcIdsManager.h
 *
 *  Created on: Feb 25, 2019
 *      Author: kraychik
 */

#ifndef PROCESSING_CCALCIDSMANAGER_H_
#define PROCESSING_CCALCIDSMANAGER_H_

#include <set>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include "../supply/CSmartException.h"

class CCalcIdsManager {
public:
	enum class IdStatus { NotExecutedYet, UnderExecution, Executed, NotExists };
	CCalcIdsManager(const CCalcIdsManager &) = delete;
	CCalcIdsManager operator =(const CCalcIdsManager &) = delete;
	int GenerateCalcId() const;
	void RemoveCalcId(int calcId);
	void Reset();
	CCalcIdsManager::IdStatus GetStatus(int calcId) const;
	void SetStatus(int calcId, IdStatus idStatus);
	static CCalcIdsManager *GetInstance();
	virtual ~CCalcIdsManager();
	void AddCalcId(int calcId, IdStatus idStatus);
	void MakeExecuted(int calcId);
	void MakeUnderExecution(int calcId);
	void MakeNotExecutedYet(int calcId);
	std::string ToString() const;
	static std::string IdStatusToString(CCalcIdsManager::IdStatus idStatus);
private:

	static CCalcIdsManager * instance;
	CCalcIdsManager();
	std::set<int> reservedIds;
	std::map<int, IdStatus> mapIdStatus;
	mutable std::mutex mLock;
};

#endif /* PROCESSING_CCALCIDSMANAGER_H_ */
