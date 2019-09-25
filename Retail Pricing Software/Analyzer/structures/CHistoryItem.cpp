#include "CHistoryItem.h"

CHistoryItem::CHistoryItem(int checkId, time_t checkTime, int shopId, int assetId, double price, double volume, double discount)
    : checkId(checkId), checkTime(checkTime), shopId(shopId), assetId(assetId), price(price), volume(volume), discount(discount)
    {

    }
CHistoryItem::~CHistoryItem () {

}
