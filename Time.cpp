//
// Created by julie on 15/04/17.
//

#include "Time.h"

Time::Time(DAY day_, unsigned short hour_, set<Classroom> classrooms_) :
    day(day_), hour(hour_), classrooms(classrooms_) {}

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

const map<DAY,string> Time::days = {{MONDAY, "Lundi"},{TUESDAY, "Mardi"},
                              {WEDNESDAY, "Mercredi"},{THURSDAY, "Jeudi"},{FRIDAY, "Vendredi"}};