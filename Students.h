//
// Created by julie on 15/04/17.
//

#ifndef TIMETABLE_GENERATOR_STUDENTS_H
#define TIMETABLE_GENERATOR_STUDENTS_H

#include <functional>
#include <vector>
#include <map>
#include "Course.h"

using std::vector;
using std::reference_wrapper;

class Students {
public:
    string lvl;
    string subject;
    char class_number;
    vector<reference_wrapper<Course>> courses;

    Students(string lvl_, string subject_, char class_number_ = '\0', vector<reference_wrapper<Course>> courses_ = {});
    static vector<Students> &get_students_from_file(std::map<string, Course> &all_courses, const char *file);
};


#endif //TIMETABLE_GENERATOR_STUDENTS_H