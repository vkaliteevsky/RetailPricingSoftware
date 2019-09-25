/*
 * CParamsItem.cpp
 *
 *  Created on: Feb 8, 2019
 *      Author: kraychik
 */

#include "CParamsItem.h"

CParamsItem::CParamsItem() {
	// TODO Auto-generated constructor stub

}

CParamsItem::CParamsItem(int groupId, double minPrice, double maxPrice, int roleId)
	: groupId(groupId), minPrice(minPrice), maxPrice(maxPrice), roleId(roleId) {
}

CParamsItem::~CParamsItem() {
	// TODO Auto-generated destructor stub
}

