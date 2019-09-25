#include "CCheckItem.h"

CCheckItem::CCheckItem(int assetId, double price, double volume, double discount)
    : assetId(assetId), price(price), volume(volume), discount(discount) {}

void CCheckItem::AddInfo(int assetId, double price, double volume, double discount)
{
	if (this->assetId != assetId) {
		throw CSmartException("����������� ������");
		return;
	}
	if (abs(this->price - price) > 0.001) {
		// � ����� ���� ��� ������ ����� assetId �� ������ �����
		// ����� ������� ������ �� ��������; ������ ������
		throw CSmartException("����������� ������");
		return;
	}
	double new_volume = this->volume + volume;
	double new_discount = (this->discount * this->volume + discount * volume) / (this->volume + volume);
	this->volume = new_volume;
	this->discount = new_discount;
}

std::string CCheckItem::ToString() const
{
	std::string res = "AssetId: ";
	res += std::to_string(assetId);
	res += "; Price: ";
	res += std::to_string(price);
	res += "; Volume: ";
	res += std::to_string(volume);
	res += "; Discount: ";
	res += std::to_string(discount);
	res += "\n";
	return (res);
}

double CCheckItem::Income() const {
	return (price - discount) * volume;
}
