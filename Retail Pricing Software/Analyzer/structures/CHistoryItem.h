#pragma once
#include <time.h>
#include "CCheckItem.h"
#include <memory>

class CHistoryItem {
public:
	//CHistoryItem ();
	CHistoryItem(int checkId, time_t checkTime, int shopId, int assetId, double price, double volume, double discount);
	~CHistoryItem ();

	int checkId;		//transaction id
	time_t checkTime;	//время продажи (unix timestamp)
	int shopId;			// идентификатор торговой точки
	int assetId;
	double price;
	double volume;
	double discount;
	//std::list<std::unique_ptr<CCheckStructure>> checkStructure; // указатель на строку чека (assetId, price, volume, discount)
};
