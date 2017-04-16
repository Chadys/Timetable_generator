//
// Created by julie on 15/04/17.
//

#include "Students.h"

Students::Students(string lvl_, string subject_, char class_number_, vector<reference_wrapper<Course>> courses_) :
    lvl(lvl_), subject(subject_), class_number(class_number_), courses(courses_) {}

static vector<Students> &Students::get_students_from_file(std::map<string, Course> &all_courses, const char *file){

}

// Example :
//L1  A   Arts
//2 4 Sculpture
//2 4 Oil_painting
//1 2 Grec_mythology_study
//
//L1  B   Arts