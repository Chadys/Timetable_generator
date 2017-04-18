//
// Created by julie on 15/04/17.
//

#include "Students.h"

Students::Students(string lvl_, string subject_, char class_number_, vector<reference_wrapper<Course>> courses_) :
    lvl(lvl_), subject(subject_), class_number(class_number_), courses(courses_) {}

const Students Students::null("", "", '\0');

bool Students::operator==(const Students& students_) const{
    return this->lvl == students_.lvl &&
            this->subject == students_.subject &&
            this->class_number == students_.class_number;
}