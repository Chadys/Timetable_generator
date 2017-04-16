//
// Created by julie on 15/04/17.
//

#ifndef TIMETABLE_GENERATOR_TEACHER_H
#define TIMETABLE_GENERATOR_TEACHER_H

#include <vector>
#include "Course.h"
#include "Time.h"

using std::vector;

class Teacher {
public:
    struct CourseTime_qt {
        Course &course;
        unsigned int time_qt;
    };

    string name;
    vector<Time> horaires;
    vector<CourseTime_qt> courses;

    Teacher(string name_, vector<Time> horaires_ = {}, vector<CourseTime_qt> courses_ = {});
};


#endif //TIMETABLE_GENERATOR_TEACHER_H
