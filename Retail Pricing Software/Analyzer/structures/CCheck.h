#pragma once
#include "CCheckItem.h"
#include "Structures.h"
#include "CIntervalSeries.h"
#include "CFilter.h"
#include "CFilterAdvanced.h"
#include "CDate.h"
#include "../supply/CTypeConverter.h"
#include <list>
#include <memory>
#include <map>
#include <string>
#include <set>
#include <vector>

//class CIntervalSeries;
class CCheck {
public:
    CCheck();
    ~CCheck();
    CCheck(int checkId, time_t checkTime, int shopId);
	CCheck(int checkId, time_t checkTime, int shopId, std::list<std::shared_ptr<CCheckItem>> &checkStructure);
	///<summary>Добавляет строку к чеку</summary>
    void AddLine(int assetId, double price, double volume, double discount);
    double Income() const;
	/// <summary>Рассчитывает выручку чека с учетом фильтрации товаров</summary>
	/// <param = "assetIds1">Массив идентификаторов, которые должны учитываться при расчете выручки</param>
	/// <returns>Выручка отфильтрованного чека по двум критериям. Если пусто, будет возвращен 0</returns>
	double Income(const std::set<int> &assetIds1, FilterMode assetFilter1, const std::set<int> &assetIds2, FilterMode assetFilter2) const;
	
	double Income(const std::set<int> &assetIds, FilterMode filterMode = FilterMode::Standard) const;

	/// <summary>Определяет, попадает ли чек в интервал [from, to]</summary>
	/// <param ="from">Если нижней границы нет, подавать значение nofilter</param>
	/// <param ="to">Если верхней границы нет, подавать значение nofilter</param>
	bool IsInInterval(time_t from, time_t to, FilterMode filterMode) const;
	int FindInterval(const CIntervalSeries & intervals, FilterMode filterMode) const;
	bool IsInIntervals(const CIntervalSeries & intervals, FilterMode filterMode) const;

	/// <summary>Определяет, продан ли чек магазине из множества shopIds</summary>
	bool IsInShops(const std::set<int> & shopIds, FilterMode filterMode) const;

    double Volume() const;
	/// <summary>Возвращает количество CCheckItem в чеке</summary>
	int Size() const;
	std::string ToString() const;

	///<returns>Возвращает ссылку на внутренний объект CheckStructure</returns>
	const std::list<std::shared_ptr<CCheckItem>> & GetCheckStructure();

	/// <summary>Удаляет указатели на все CCheckItems, у которых assetId == assetId. Данные не очищаются</summary>
	/// <param = "deleteEqual">True, если нужно удалять только те CCheckItems, у которых совпали assetId</param>
	void DeleteCheckItems(int assetId, bool deleteEqual = true);

	/// <summary>Удаляет указатели на все CCheckItems, у которых assetId \in assetIds. Данные не очищаются</summary>
	/// <param = "deleteEqual">True, если нужно удалять только те CCheckItems, у которых совпали assetId</param>
	void DeleteCheckItems(std::set<int> &assetIds, bool deleteEqual = true);

	/// <summary>Копирует все указатели на CheckItem. Копирование CheckItem не осущетствляется</summary>
	/// <returns>Указатель на новый объект CCheck</returns>
	std::shared_ptr<CCheck> Clone();

	std::unique_ptr<std::map<int, double>> GetSoldAmounts() const;

	double AssetVolume(int assetId) const;
	bool HasAsset(int assetId) const;
	void FilterItems(const CFilterAdvanced & filter, std::list<std::shared_ptr<const CCheckItem>> & resultingList) const;
	CDate GetDate() const;
	int Day() const;
	int Month() const;
	int Year() const;

    int checkId = 0;		//transaction id
	time_t checkTime = 0;	//время продажи (unix timestamp)
	int shopId = 0;			// идентификатор торговой точки
private:
	/// <summary>Находит элемент CCheckItem в структуре чека</summary>
	/// <returns>Умный указатель на CCheckItem, либо на nullptr</return>
	std::shared_ptr<CCheckItem> FindItemToAdd(int assetId, double price, double volume, double discount);
	//void FilterItems(const CIntervalSeries & intervals, FilterMode filterIntervals, const std::set<int> & shopIds, FilterMode filterShops, const std::set<int> & assetIds, FilterMode filterAssets, const std::set<int> & assetIdsFromGroups, FilterMode filterGroups, std::list<std::shared_ptr<CCheckItem>> & resultingList) const;
    static FilterMode BoolToFilterMode(bool useSupplement);
	std::list<std::shared_ptr<CCheckItem>> checkStructure;
	//double income;
	//double volume;
};
