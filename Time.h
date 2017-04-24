//
// Created by julie on 15/04/17.
//

#ifndef TIMETABLE_GENERATOR_TIME_H
#define TIMETABLE_GENERATOR_TIME_H

#include <vector>
#include <string>
#include <map>
#include <boost/functional/hash_fwd.hpp>
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
    static const map<DAY,string> days;

    Time(DAY day_, unsigned short hour_, vector<Classroom> classrooms_ = {});
    bool operator==(const Time &time_) const;
    bool operator!=(const Time &time_) const;
    bool operator<(const Time &time_) const;
};

struct TimeAccessor{
    DAY day;
    unsigned short hour;
    TimeAccessor(DAY day_ = SUNDAY, unsigned short hour_ = 0);
    explicit operator bool() const;
    bool operator==(const TimeAccessor &time_) const;
    bool operator<(const TimeAccessor &time_) const;
};

namespace std
{
    template <>
    struct hash<TimeAccessor>
    {
        size_t operator()( const TimeAccessor& ta) const {
            std::size_t seed = 0;
            boost::hash_combine(seed, ta.day);
            boost::hash_combine(seed, ta.hour);
            return seed;
        }
    };
}

#endif //TIMETABLE_GENERATOR_TIME_H
