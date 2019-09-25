/*
 * CCalcIdsManager.cpp
 *
 *  Created on: Feb 25, 2019
 *      Author: kraychik
 */

#include "CCalcIdsManager.h"

int CCalcIdsManager::GenerateCalcId() const {
	mLock.lock();
	int k = 0;
	int calcId;
	bool isFound = false;
	while (k++ < 10000) {
		calcId = rand();
		if (reservedIds.find(calcId) == reservedIds.end()) {
			isFound = true; break;
		}
	}
	if (isFound) {
		mLock.unlock();
		return calcId;
	}
	mLock.unlock();
	throw CSmartException("CCalcIdsManager::GenerateCalcId(): Couldn't generate CalcId");
}

void CCalcIdsManager::Reset() {
	mLock.lock();
	reservedIds.clear();
	mLock.unlock();
}

CCalcIdsManager* CCalcIdsManager::GetInstance() {
	if (instance == nullptr) {
		instance = new CCalcIdsManager();
	}
	return instance;
}

void CCalcIdsManager::RemoveCalcId(int calcId) {
	mLock.lock();
	reservedIds.erase(calcId);
	mapIdStatus.erase(calcId);
	mLock.unlock();
}

void CCalcIdsManager::AddCalcId(int calcId, IdStatus idStatus) {
	mLock.lock();
	reservedIds.insert(calcId);
	mapIdStatus[calcId] = idStatus;
	mLock.unlock();
}

CCalcIdsManager::IdStatus CCalcIdsManager::GetStatus(int calcId) const {
	mLock.lock();
	IdStatus idStatus;
	const auto it = mapIdStatus.find(calcId);
	if (it == mapIdStatus.end()) {
		idStatus = IdStatus::NotExists;
	} else {
		idStatus = it->second;
	}
	mLock.unlock();
	return idStatus;
}

void CCalcIdsManager::SetStatus(int calcId, IdStatus idStatus) {
	mLock.lock();
	const auto itSet = reservedIds.find(calcId);
	if (itSet == reservedIds.end()) {
		if (idStatus != IdStatus::NotExists) {
			reservedIds.insert(calcId);
			mapIdStatus[calcId] = idStatus;
		} else {
			// ничего не делаем
		}
	} else {
		mapIdStatus[calcId] = idStatus;
	}
	mLock.unlock();
}

std::string CCalcIdsManager::IdStatusToString(CCalcIdsManager::IdStatus idStatus) {
	switch (idStatus) {
	case IdStatus::Executed:
		return std::string("Executed");
	case IdStatus::NotExecutedYet:
		return std::string("NotExecutedYet");
	case IdStatus::UnderExecution:
		return std::string("UnderExecution");
	default:
		throw CSmartException("CCalcIdsManager::IdStatusToString: not implemented yet");
	}
}

CCalcIdsManager::CCalcIdsManager() {

}

CCalcIdsManager::~CCalcIdsManager() {

}

void CCalcIdsManager::MakeExecuted(int calcId) {
	SetStatus(calcId, IdStatus::Executed);
}

void CCalcIdsManager::MakeUnderExecution(int calcId) {
	SetStatus(calcId, IdStatus::UnderExecution);
}

void CCalcIdsManager::MakeNotExecutedYet(int calcId) {
	SetStatus(calcId, IdStatus::NotExecutedYet);
}

CCalcIdsManager * CCalcIdsManager::instance = nullptr;

std::string CCalcIdsManager::ToString() const {
	std::string result("CCalcIdsManager::ToString()\n");
	for (const int calcId : reservedIds) {
		const auto it = mapIdStatus.find(calcId);
		if (it == mapIdStatus.end()) {
			throw CSmartException("CCalcIdsManager::ToString(): not found calcId = " + std::to_string(calcId) + "n");
		}
		const auto idStatus = it->second;
		result += std::to_string(calcId) + ": " + IdStatusToString(idStatus) + ", ";
	}
	return result;
}
