#pragma once
#include <time.h>
#include <cmath>
#include "../supply/CSmartException.h"
#include <string>

class CCheckItem {
public:
    CCheckItem(int assetId, double price, double volume, double discount);
    double Income() const;
    int assetId;		// идентификатор товара
	double price;		//цена продажи 1 товара
	double volume;		// кол-во проданных штук
	double discount;	//сумма скидки на 1 товара
	void AddInfo(int assetId, double price, double volume, double discount);
	std::string ToString() const;
};
