#pragma once
#include <memory>
#include <string>
class CPriceItem {
public:
	CPriceItem ();
	CPriceItem(double price, double cost);
	~CPriceItem ();
	void Init(std::shared_ptr<CPriceItem> item);
	void SetPrice(double price);
	void SetCost(double cost);
	double GetPrice() const;
	double GetCost() const;
	std::string ToString() const;
	//int assetId = 0;
	double price = 0.0;	//цена продукта
	double cost = 0.0;	//себестоимость
};
