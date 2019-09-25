/*
 * ResultManager.cpp
 *
 *  Created on: Jan 20, 2019
 *      Author: kraychik
 */

#include "CResultManager.h"

//CResultManager instance;

CResultManager::CResultManager() {
	SetTimer();
	// TODO Auto-generated constructor stub

}

CResultManager *CResultManager::GetInstance() {
	static CResultManager instance;
	return (& instance);
}

void CResultManager::AddResult(std::shared_ptr<const CCalcResult> calcResult) {
	mutex.lock();
	const int calcId = calcResult->calcId;

	CCalcIdsManager *calcIdsManager = CCalcIdsManager::GetInstance();
	auto idStatus = calcIdsManager->GetStatus(calcId);
	if (idStatus == CCalcIdsManager::IdStatus::NotExists) {
		calcIdsManager->AddCalcId(calcId, CCalcIdsManager::IdStatus::Executed);
	}
	idStatus = calcIdsManager->GetStatus(calcId);
	if (idStatus != CCalcIdsManager::IdStatus::Executed) {
		throw CSmartException(std::string("CResultManager::AddResult: wrong IdStatus found\n") + calcResult->ToString());
	}
	calcIdsManager->MakeExecuted(calcId);

	auto foundResult = mapCalcResult.find(calcId);
	if (foundResult != mapCalcResult.end()) {
		mutex.unlock();
		throw CSmartException(std::string("CResultManager::AddResult: foundResult was found!\n") + calcResult->ToString());
	}

	mapCalcResult.insert(std::pair<int, std::shared_ptr<const CCalcResult>>(calcId, calcResult));
	mapTimeOfResult.insert(std::pair<int, time_t>(calcId, time(NULL)));
	mutex.unlock();
}

std::shared_ptr<const CCalcResult> CResultManager::GetResult(int calcId) const {
	mutex.lock();
	auto it = mapCalcResult.find(calcId);
	if (it != mapCalcResult.end()) {
		// результат найден и подготовлен
		auto result = it->second;
		mutex.unlock();
		return result;
	} else {
		// результат не найден
		// проверяем, есть ли о нем информация в taskIdsUnderCalculation
		std::shared_ptr<CCalcResult> calcResult;
		auto calcIdsManager = CCalcIdsManager::GetInstance();
		const auto idStatus = calcIdsManager->GetStatus(calcId);
		if ((idStatus == CCalcIdsManager::IdStatus::NotExecutedYet) || (idStatus == CCalcIdsManager::IdStatus::UnderExecution)) {
			calcResult = std::make_shared<CCalcResult>(0, calcId, std::string(""), CalcResultStatus::WAIT);
		} else {
			calcResult = std::make_shared<CCalcResult>(0, calcId, std::string(""), CalcResultStatus::ERROR);
		}
		/*if (idStatus != CCalcIdsManager::IdStatus::Executed) {
			// информация о calcId не найдена. Выдаем ошибку
			calcResult = std::make_shared<CCalcResult>(0, calcId, std::string(""), CalcResultStatus::ERROR);
		} else {
			// информация о calcId найдена. Вернуть WAIT
			calcResult = std::make_shared<CCalcResult>(0, calcId, std::string(""), CalcResultStatus::WAIT);
		}*/
		mutex.unlock();
		return calcResult;
		//std::string response = std::string("");
		//return (std::make_shared<CCalcResult>(0, calcId, response, CalcResultStatus::WAIT));
	}
}

CResultManager::~CResultManager() {
	// TODO Auto-generated destructor stub
}

void CResultManager::EraseAll() {
	mutex.lock();
	mapCalcResult.clear();
	mapTimeOfResult.clear();
	mutex.unlock();
}

std::shared_ptr<const CCalcResult> CResultManager::EraseResult(int calcId) {
	mutex.lock();
	std::shared_ptr<const CCalcResult> erasingResult = nullptr;
	auto itMap = mapCalcResult.find(calcId);
	if (itMap != mapCalcResult.end()) {
		erasingResult = itMap->second;
	}
	mapCalcResult.erase(itMap);
	mapTimeOfResult.erase(calcId);
	auto manager = CCalcIdsManager::GetInstance();
	manager->RemoveCalcId(calcId);
	mutex.unlock();
	return erasingResult;
}

void CResultManager::EraseOldResults() {
	std::list<int> calcIdsToErase;
	for (const auto & it : mapCalcResult) {
		int calcId = it.second->calcId;
		if (IsTimeToErase(calcId)) {
			calcIdsToErase.push_back(calcId);
		}
	}
	for (const auto & calcId : calcIdsToErase) {
		EraseResult(calcId);
	}
}

std::string CResultManager::ToString() const {
	std::string res;
	for (const auto x : mapCalcResult) {
		res += x.second->ToString();
	}
	return res;
}

void CResultManager::SetTimer() {
	std::thread t([=]() {
		while(true) {
			//if(this->clear) return;
			std::this_thread::sleep_for(std::chrono::milliseconds(delayTimer));
			//if(this->clear) return;
			EraseOldResults();
		}
	});
	t.detach();
}

bool CResultManager::IsTimeToErase(int calcId) const {
	auto itTime = mapTimeOfResult.find(calcId);
	if (itTime == mapTimeOfResult.end()) {
		return true;
	}
	double deltaMs = 1000.0 * difftime(time(NULL), itTime->second);
	return (deltaMs > delayBeforeErase);
}
