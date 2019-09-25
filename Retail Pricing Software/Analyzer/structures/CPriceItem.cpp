#include "CPriceItem.h"

CPriceItem::CPriceItem() {}
CPriceItem::~CPriceItem() {}

CPriceItem::CPriceItem(double price, double cost) : price(price), cost(cost)
{
}

void CPriceItem::Init(std::shared_ptr<CPriceItem> item) {
	this->price = item->price;
	this->cost = item->cost;
}

void CPriceItem::SetPrice(double price) {
	this->price = price;
}

void CPriceItem::SetCost(double cost) {
	this->cost = cost;
}

std::string CPriceItem::ToString() const {
	std::string result = std::string("Price: ") + std::to_string(price) + ", Cost: " + std::to_string(cost) + "\n";
	return result;
}

double CPriceItem::GetPrice() const {
	return price;
}

double CPriceItem::GetCost() const {
	return cost;
}
