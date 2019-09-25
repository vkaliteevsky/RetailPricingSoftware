#pragma once
#ifndef nofilter
#define nofilter 0
#endif
#include <time.h>
#include <list>
#include "CCheck.h"
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <stdio.h>
#include "CIntervalSeries.h"
#include "CHistoryItem.h"
#include "CSupplementMatrix.h"
#include "CAssetsSummary.h"
#include "CGroupsInfo.h"
#include "CFilterAdvanced.h"
#include "CStructureMap.h"
#include "CPriceList.h"
#include "CMatrix.h"

using namespace std;
typedef std::pair<int, time_t> unique_key;

/// <summary>Является идентификатором типа функции, которую необходимо применять для расчетов</summary>
enum class FunMode { Sum, AvgCheck };

class CPriceList;
class CSalesHistory {
public:
    CSalesHistory();
    CSalesHistory(shared_ptr<list<CHistoryItem>> items);
    ~CSalesHistory();
    void AddCheck(shared_ptr<CCheck> check);
	void AddLine(int checkId, time_t checkTime, int shopId, int assetId, double price, double volume, double discount);
	void AddLine(const CHistoryItem & item);
	void DeleteCheck(unique_key checkKey);
    double Income() const;
    double Volume() const;
	std::string ToString() const;
	int AmountOfAssets() const;

	/// <summary>Осуществляет поиск чека по паре уникальных параметров</summary>
	/// <param name="checkId">Идентификатор чека из системы пользователя (может быть не уникальным)</param>
	/// <param name="checkTime">Время продажи чека (time_t)</param>
	/// <returns>Умный указатель на объект CCheck</returns>
	shared_ptr<CCheck> FindCheck(int checkId, time_t checkTime) const;
	shared_ptr<CCheck> FindCheck(unique_key key) const;

	/// <summary>Возвращает множество assetId, которые принадлежат groupId</summary>
	/// <returns>Возвращает умный указатель на множество. Если ничего не найдено, возвращает указатель на nullptr</returns>
	shared_ptr<const set<int>> GetAssetsFromGroup(int groupId) const;

	/// <summary>Возвращает множество assetId, которые принадлежат хотя бы одной из groupIds</summary>
	/// <returns>Возвращает умный указатель на множество. Если ничего не найдено, возвращает указатель на nullptr</returns>
	shared_ptr<set<int>> GetAssetsFromGroup(const set<int> &groupIds) const;
	void SetGroupsInfo(shared_ptr<const CGroupsInfo> groupsInfo);

	/// <summary>Сопоставляет всякому assetId его группу groupId</summary>
	/// <returns>Возвращает groupId, либо -1, если ничего не найдено</returns>
	int GetGroupId(int assetId) const;
	set<int> GetGroupIds() const;
	shared_ptr<const CGroupsInfo> GetGroupsInfo() const;
	/// <summary>Осуществляет расчет показателя, задаваемого mode, с фильтром по торговым точкам, товарам и группам</summary>
	/// <param = "from">Время начала периода (если не задано, указывать nofilter)</param>
	/// <param = "to">Время конца периода (если не задано, указывать nofilter)</param>
	/// <param = "funMode">Определяет тип функции, которая будет вычисляться в ходе исполнения программы</param>
	/// <returns>Рассчитанное значение. Если фильтрация образовывает пустое множество, возвращается 0</returns>
	vector<double> CalculateDynamics(const CFilter & filter, FunMode funMode) const;
	void CalculateCheckStructure(const CFilter & filter, CStructureMap<int, double> & structureMap, int & amountOfChecks) const;
	void Clear();
	shared_ptr<CSupplementMatrix> BuildSupplementMatrix(const CFilterAdvanced & filter) const;
	set<int> GetAllAssets() const;
	shared_ptr<CAssetsSummary> GetAssetsSummary(const CFilter & filter) const;
	shared_ptr<CPriceList> GenCurrentPriceList() const;
	void FillDaysStatistics(CMatrix &priceMatrix, CMatrix &volumeMatrix, CMatrix &incomeMatrix, const CFilterAdvanced & filter) const;
	const map<time_t, int> & CalcAmountOfChecks();
	const map<time_t, int> & GetDaysAmountOfChecks() const;
private:
	void AddInfoToCheck(shared_ptr<CCheck> check, int assetId, double price, double volume, double discount);
	void RemoveCheckFromInternalStructure(list<shared_ptr<CCheck>>::iterator checksIterator);
	shared_ptr<const CGroupsInfo> groupsInfoPtr;
	map<unique_key, shared_ptr<CCheck>> checks_map; // Определяет сопоставление (assetId, time) -> "указатель на чек"
	list<shared_ptr<CCheck>> checks;
	set<int> assetsAll;	// множество всех активов, входящих в объект
	map<time_t, shared_ptr<CPriceList>> priceLists;
	map<time_t, int> amtOfChecks;
};
