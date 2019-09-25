/*
 * CDate.cpp
 *
 *  Created on: Feb 11, 2019
 *      Author: kraychik
 */

#include "CDate.h"

CDate::~CDate() {
	// TODO Auto-generated destructor stub
}

int CDate::Day() const {
	return time.tm_mday;
}

int CDate::Month() const {
	return time.tm_mon;
}

int CDate::Year() const {
	return time.tm_year;
}

bool CDate::operator ==(const CDate& date) const {
	return (Day() == date.Day()) && (Month() == date.Month()) && (Year() == date.Year());
}

bool CDate::operator <(const CDate& date) const {
	if (Year() < date.Year()) return true;
	else if (Year() > date.Year()) return false;
	else {
		if (Month() < date.Month()) return true;
		else if (Month() > date.Month()) return false;
		else {
			return Day() < date.Day();
		}
	}
}

CDate::CDate(const time_t time) {
	this->time = *gmtime(&time);
}

bool CDate::operator >(const CDate& date) const {
	return !(operator ==(date) && operator <(date));
}

bool CDate::operator <=(const CDate& date) const {
	return operator <(date) || operator ==(date);
}

CDate::CDate(const CDate& date) {
	this->time = date.time;
}

CDate::CDate(std::shared_ptr<const CDate> date) {
	this->time = date->time;
}

bool CDate::operator >=(const CDate& date) const {
	return operator >(date) || operator ==(date);
}

void CDate::AddDays(int deltaDays) {
	time.tm_mday += deltaDays;
}

CDate CDate::operator +(const int deltaDays) const {
	struct tm newTime = time;
	newTime.tm_mday += deltaDays;
	return CDate(newTime);
}

CDate::CDate(const struct tm& date) {
	this->time = date;
}

CDate CDate::operator -(const int deltaDays) const {
	struct tm newTime = time;
	newTime.tm_mday += deltaDays;
	return CDate(newTime);
}

time_t CDate::DayOnly() const {
	struct tm dayOnly = this->time;
	dayOnly.tm_hour = 0;
	dayOnly.tm_min = 0;
	dayOnly.tm_sec = 0;
	return mktime(&dayOnly);
}

time_t CDate::MonthOnly(time_t dt) {
	struct tm monthOnly;
	monthOnly = *gmtime(&dt);
	monthOnly.tm_hour = 0;
	monthOnly.tm_min = 0;
	monthOnly.tm_sec = 0;
	monthOnly.tm_mday = 1;
	return mktime(&monthOnly);
}

int CDate::MonthNumberOnly(time_t dt) {
	struct tm monthOnly;
	monthOnly = *gmtime(&dt);
	return monthOnly.tm_mon + 1;
}
