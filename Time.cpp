//
// Created by julie on 15/04/17.
//

#include "Time.h"

Time::Time(DAY day_, unsigned short hour_, vector<Classroom> classrooms_) :
    day(day_), hour(hour_), classrooms(classrooms_), courses_number(0) {}

const Time Time::null(SUNDAY, 0, {});

bool Time::operator==(const Time &time_) const{
    return this->day == time_.day && this->hour == time_.hour;
}

bool Time::operator!=(const Time &time_) const{
    return !(this->day == time_.day && this->hour == time_.hour);
}

bool Time::operator<(const Time &time_) const{
    return this->day < time_.day || (this->day == time_.day && this->hour < time_.hour);
}

Time::operator bool() const{
    return *this != Time::null;
}