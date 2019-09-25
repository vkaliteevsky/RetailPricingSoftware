#pragma once
#include <time.h>
#include <vector>
#include <memory>
//#include "CCheck.h"
#include "Structures.h"
#ifndef nofilter
#define nofilter 0
#endif

//class CCheck;
class CIntervalSeries : public std::vector<interval>
{
public:
	CIntervalSeries();
	CIntervalSeries(const std::vector<interval> & intervals);
	CIntervalSeries(const CIntervalSeries& intervals);
	//int FindInterval(const std::shared_ptr<CCheck> check, FilterMode filterMode = FilterMode::Standard) const;
	//int FindInterval(const CCheck * check, FilterMode filterMode = FilterMode::Standard) const;
};
