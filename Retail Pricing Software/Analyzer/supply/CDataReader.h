/*
 * CDataReader.h
 *
 *  Created on: May 4, 2019
 *      Author: kraychik
 */

#ifndef SUPPLY_CDATAREADER_H_
#define SUPPLY_CDATAREADER_H_

#include <memory>
#include <list>
#include "../structures/CHistoryItem.h"
#include "../structures/CPriceItem.h"

class CDataReader {
public:
	CDataReader();
	virtual std::shared_ptr<std::list<CHistoryItem>> ReadSales(int projectId) = 0;
	virtual std::pair<std::shared_ptr<std::list<int>>, std::shared_ptr<std::list<CPriceItem>>> ReadPrices(int projectId) = 0;
};

#endif /* SUPPLY_CDATAREADER_H_ */
