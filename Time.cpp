//
// Created by julie on 15/04/17.
//

#include "Time.h"

Time::Time(DAY day_, unsigned short hour_, vector<Classroom> classrooms_) :
    day(day_), hour(hour_), classrooms(classrooms_), courses_number(0) {}

TimeAccessor::TimeAccessor(DAY day_, unsigned short hour_) : day(day_), hour(hour_) {}


bool Time::operator==(const Time &time_) const{
    return this->day == time_.day && this->hour == time_.hour;
}

bool TimeAccessor::operator==(const TimeAccessor &time_) const{
    return this->day == time_.day && this->hour == time_.hour;
}

bool Time::operator!=(const Time &time_) const{
    return !(this->day == time_.day && this->hour == time_.hour);
}

bool Time::operator<(const Time &time_) const{
    return this->day < time_.day || (this->day == time_.day && this->hour < time_.hour);
}

bool TimeAccessor::operator<(const TimeAccessor &time_) const{
    return this->day < time_.day || (this->day == time_.day && this->hour < time_.hour);
}

TimeAccessor::operator bool() const{
    return this->day != SUNDAY || this->hour != 0;
}
