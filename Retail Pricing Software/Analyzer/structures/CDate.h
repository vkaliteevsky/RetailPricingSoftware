/*
 * CDate.h
 *
 *  Created on: Feb 11, 2019
 *      Author: kraychik
 */

#ifndef STRUCTURES_CDATE_H_
#define STRUCTURES_CDATE_H_

#include <time.h>
#include <ctime>
#include <memory>

class CDate {
public:
	CDate(const time_t time);
	CDate(const CDate & date);
	CDate(const struct tm & date);
	CDate(std::shared_ptr<const CDate> date);
	virtual ~CDate();
	int Day() const;
	int Month() const;
	int Year() const;
	inline bool operator ==(const CDate & date) const;
	inline bool operator <(const CDate & date) const;
	bool operator >(const CDate & date) const;
	bool operator <=(const CDate & date) const;
	bool operator >=(const CDate & date) const;
	inline void AddDays(int deltaDays);
	CDate operator +(const int deltaDays) const;
	CDate operator -(const int deltaDays) const;
	time_t DayOnly() const;
	static time_t MonthOnly(time_t dt);
	static int MonthNumberOnly(time_t dt);
private:
	struct tm time;
};

#endif /* STRUCTURES_CDATE_H_ */
