/*
 * Structures.h
 *
 *  Created on: Feb 5, 2019
 *      Author: kraychik
 */

#ifndef STRUCTURES_STRUCTURES_H_
#define STRUCTURES_STRUCTURES_H_

#ifndef nofilter
#define nofilter 0
#endif

typedef std::pair<time_t, time_t> interval;
enum class FilterMode { Standard, UseSupplementSet, NotImplemented };
enum class PeriodMode { Day, Week, Month, Quarter, Half, Year, NotImplemented };
enum class KPI { Income, Volume, Gain, NotImplemented };
enum class DynamicsTaskType { IncomeDynamics, AvgCheckDynamics, NotImplemented };
enum class ResponseFormat { ArrayOfStructures, ArrayOfArrays, NotImplemented };


#endif /* STRUCTURES_STRUCTURES_H_ */
