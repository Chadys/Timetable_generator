//
// Created by julie on 15/04/17.
//

#include "Course.h"

Course::Course(string title_, unsigned short hours_number_, COURSE_TYPE type_) :
        title(title_), hours_number(hours_number_), type(type_) {}

bool Course::operator==(const Course& course_) const{
    return this->title == course_.title;
}