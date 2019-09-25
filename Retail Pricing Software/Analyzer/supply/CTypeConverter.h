/*
 * CTypeConverter.h
 *
 *  Created on: Feb 21, 2019
 *      Author: kraychik
 */

#ifndef SUPPLY_CTYPECONVERTER_H_
#define SUPPLY_CTYPECONVERTER_H_

#include <string>
#include "../structures/Structures.h"
#include "CSmartException.h"

class CTypeConverter {
public:
	static KPI ConvertKPI(const std::string & kpiStr);
	static std::string ConvertKPI(KPI kpi);
	static DynamicsTaskType ConvertDynamicsType(const std::string & dynamicsType);
	static std::string ConvertDynamicsType(DynamicsTaskType dynamicsTaskType);
	static FilterMode BoolToFilterMode(bool useSupplement);
	static ResponseFormat ConvertResponseFormat(const std::string & responseFormat);

};

#endif /* SUPPLY_CTYPECONVERTER_H_ */
