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
using std::reference_wrapper;

class Teacher {
public:
    struct CourseTime_qt {
        Course &course;
        unsigned int time_qt;
        CourseTime_qt(Course &course_, unsigned int time_qt_);
    };

    string name;
    vector<reference_wrapper<Time>> horaires;
    vector<CourseTime_qt> courses;

    Teacher(string name_, vector<reference_wrapper<Time>> horaires_ = {}, vector<CourseTime_qt> courses_ = {});
};


#endif //TIMETABLE_GENERATOR_TEACHER_H
