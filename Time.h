//
// Created by julie on 15/04/17.
//

#ifndef TIMETABLE_GENERATOR_TIME_H
#define TIMETABLE_GENERATOR_TIME_H

#include <vector>
#include "Classroom.h"

using std::vector;
using std::string;

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
    static const Time null;

    Time(DAY day_, unsigned short hour_, vector<Classroom> classrooms_ = {});
    bool operator==(const Time &time_) const;
    bool operator!=(const Time &time_) const;
    explicit operator bool() const;
    static vector<vector<Time>> get_times_from_file(const char *file);
};


#endif //TIMETABLE_GENERATOR_TIME_H
