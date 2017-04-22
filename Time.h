//
// Created by julie on 15/04/17.
//

#ifndef TIMETABLE_GENERATOR_TIME_H
#define TIMETABLE_GENERATOR_TIME_H

#include <vector>
#include <string>
#include <map>
#include "Classroom.h"

using std::vector;
using std::string;
using std::map;

enum DAY{
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
};

class Time {
public:
    DAY day;
    unsigned short hour;
    vector<Classroom> classrooms;
    unsigned short courses_number;
    static const map<DAY,string> days;

    Time(DAY day_, unsigned short hour_, vector<Classroom> classrooms_ = {});
    bool operator==(const Time &time_) const;
    bool operator!=(const Time &time_) const;
    bool operator<(const Time &time_) const;
    explicit operator bool() const;
};

struct TimeAccessor{
    DAY day;
    unsigned short hour;
    TimeAccessor(DAY day_ = SUNDAY, unsigned short hour_ = 0);
    explicit operator bool() const;
    bool operator==(const TimeAccessor &time_) const;
    bool operator<(const TimeAccessor &time_) const;
};

#endif //TIMETABLE_GENERATOR_TIME_H
