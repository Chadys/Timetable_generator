//
// Created by julie on 15/04/17.
//

#include "Course.h"

Course::Course() : title(""), hours_number(0), type(UNITAIRE) {}

Course::Course(string title_, unsigned short hours_number_, COURSE_TYPE type_) :
        title(title_), hours_number(hours_number_), type(type_) {}