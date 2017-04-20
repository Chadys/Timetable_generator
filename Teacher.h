//
// Created by julie on 15/04/17.
//

#ifndef TIMETABLE_GENERATOR_TEACHER_H
#define TIMETABLE_GENERATOR_TEACHER_H

#include <vector>
#include <map>
#include "Course.h"
#include "Time.h"

using std::vector;
using std::map;

class Teacher {
public:
    string name;
    vector<TimeAccessor> horaires;
    map<string, unsigned int> time_by_course;
    static const Teacher null;

    Teacher(string name_="", vector<TimeAccessor> horaires_ = {}, map<string, unsigned int> time_by_course_ = {});
    bool operator==(const Teacher& teacher_) const;
};


#endif //TIMETABLE_GENERATOR_TEACHER_H
