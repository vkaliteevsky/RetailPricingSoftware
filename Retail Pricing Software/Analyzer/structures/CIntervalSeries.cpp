#include "CIntervalSeries.h"

CIntervalSeries::CIntervalSeries() {}
/*int CIntervalSeries::FindInterval(const std::shared_ptr<CCheck> check, FilterMode filterMode) const
{
	//auto checkTime = check->checkTime;
	for (int i = 0; i < (int)this->size(); ++i) {
		auto interval = this->at(i);
		bool isInInterval = check->IsInInterval(interval.first, interval.second, filterMode);
		if (isInInterval) {
			return i;
		}
	}
	return -1;
}*/

CIntervalSeries::CIntervalSeries(const std::vector<interval>& intervals) {
	this->clear();
	auto l = intervals.size();
	this->reserve(l);
	for (const auto & interval : intervals) {
		this->push_back(interval);
	}
}
CIntervalSeries::CIntervalSeries(const CIntervalSeries& intervals) {
	this->clear();
	auto l = intervals.size();
	this->reserve(l);
	for (const auto & interval : intervals) {
		this->push_back(interval);
	}
}
