/*
 * CPriceList.h
 *
 *  Created on: Feb 8, 2019
 *      Author: kraychik
 */

#ifndef STRUCTURES_CPRICELIST_H_
#define STRUCTURES_CPRICELIST_H_
#include <vector>
#include <memory>
#include <map>
#include <set>
#include <string>
#include <time.h>
#include <list>
#include "CPriceItem.h"
#include "Structures.h"

class COptimizerParams;
class CPriceList {
public:
	CPriceList();
	CPriceList(const std::shared_ptr<CPriceList> priceList);
	CPriceList(const std::shared_ptr<std::list<int>> assetIds, const std::shared_ptr<std::list<CPriceItem>> items);
	CPriceList(const std::vector<int> & assetIds, const std::vector<double> & prices, const std::vector<double> & costs);
	virtual ~CPriceList();
	void AddPriceItem(int assetId, std::shared_ptr<CPriceItem> item);
	void AddPriceItem(int assetId, double price, double cost);
	std::shared_ptr<CPriceItem> GetPriceItem(int assetId) const;
	int Size() const;
	double GetPrice(int assetId) const;
	double GetCost(int assetId) const;
	void EraseItem(int assetId);
	void Clear();
	void SetStartTime(time_t newStartTime);
	void SetEndTime(time_t newEndTime);
	void SetPrice(int assetId, double price);
	double MinPrice() const;
	double MaxPrice() const;
	std::shared_ptr<CPriceList> CloneWithItems() const;
	void UpdatePrices(const std::vector<int> & assetIds, const std::vector<double> & newPrices);
	inline time_t GetStartTime() const;
	inline time_t GetEndTime() const;
	std::string ToString() const;
	std::shared_ptr<std::vector<int>> GetAllAssets() const;
	const std::map<int, std::shared_ptr<CPriceItem>> & GetPriceItems() const;
	void FillVectors(std::vector<int> & assetIds, std::vector<std::shared_ptr<CPriceItem>> & priceItems) const;
	void JoinPriceList(const std::shared_ptr<CPriceList> priceList);
	std::pair<std::shared_ptr<CPriceList>, std::shared_ptr<CPriceList>> SplitPriceList(const std::set<int> & assetIds) const;
	std::shared_ptr<CPriceList> FilterPriceList(const std::set<int> & assetIds) const;
	std::shared_ptr<CPriceList> FilterPriceList(const std::shared_ptr<std::set<int>> assetIds) const;
	static std::shared_ptr<CPriceList> Merge(std::shared_ptr<CPriceList> priceList1, std::shared_ptr<CPriceList> priceList2);
	std::string ToJSON() const;
	std::string ToJSON(const std::map<int, bool> & mapIsAssetIndicator) const;
	std::string ToJSON(std::shared_ptr<const COptimizerParams> params) const;
	std::shared_ptr<std::map<int, double>> GetPricesMap() const;
private:
	//int FindIndex(int assetId) const;
	//std::map<int, int> mapIndex;	// assetId -> индекс в массиве priceList
	time_t startTime = 0;
	time_t endTime = 0;
	std::map<int, std::shared_ptr<CPriceItem>> priceList;	// assetId -> CPriceItem
};

#endif /* STRUCTURES_CPRICELIST_H_ */
