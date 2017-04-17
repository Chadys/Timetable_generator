//
// Created by julie on 15/04/17.
//

#include "Teacher.h"


Teacher::Teacher(string name_, vector<reference_wrapper<Time>> horaires_, vector<CourseTime_qt> courses_) :
    name(name_), horaires(horaires_), courses(courses_) {}

Teacher::CourseTime_qt::CourseTime_qt(Course &course_, unsigned int time_qt_) : course(course_), time_qt(time_qt_) {}