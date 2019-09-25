/*
 * CParamsItem.h
 *
 *  Created on: Feb 8, 2019
 *      Author: kraychik
 */

#ifndef ANALIZER_CPARAMSITEM_H_
#define ANALIZER_CPARAMSITEM_H_

class CParamsItem {
public:
	CParamsItem();
	CParamsItem(int groupId, double minPrice, double maxPrice, int roleId);
	virtual ~CParamsItem();
	int groupId = -1;
	double minPrice = 0.0;
	double maxPrice = 100000000000.0;
	int roleId = 0;
};

#endif /* ANALIZER_CPARAMSITEM_H_ */
