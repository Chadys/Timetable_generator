//
// Created by julie on 15/04/17.
//

#ifndef TIMETABLE_GENERATOR_TEACHER_H
#define TIMETABLE_GENERATOR_TEACHER_H

#include <functional>
#include <vector>
#include <map>
#include "Course.h"
#include "Time.h"

using std::vector;
using std::map;
using std::reference_wrapper;

class Teacher {
public:
    string name;
    vector<reference_wrapper<Time>> horaires;
    map<string, unsigned int> courses_names;
    static const Teacher null;

    Teacher(string name_, vector<reference_wrapper<Time>> horaires_ = {}, map<string, unsigned int> courses_names_ = {});
    bool operator==(const Teacher& teacher_) const;
};


#endif //TIMETABLE_GENERATOR_TEACHER_H
