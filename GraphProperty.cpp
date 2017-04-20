//
// Created by julie on 20/04/17.
//

#include "GraphProperty.h"


GraphProperty::GraphProperty(Course &course_, Students &students_, Teacher &teacher_) :
course(course_), students(students_), teacher(teacher_),
teacher_time_left(teacher_.time_by_course[course_.title]) {}

GraphProperty& GraphProperty::operator=(const GraphProperty& possibility_){
    this->course = possibility_.course;
    this->students = possibility_.students;
    this->teacher = possibility_.teacher;
    this->time = possibility_.time;
    this->teacher_time_left = possibility_.teacher_time_left;
}

bool GraphProperty::operator==(const GraphProperty& possibility_) const {
    if (this->course == possibility_.course && this->students == possibility_.students &&
        this->teacher == possibility_.teacher && this->time.size() == possibility_.time.size()) {
        for (int i = 0; i < this->time.size(); ++i) {
            if (this->time[i].day != possibility_.time[i].day || this->time[i].hour != possibility_.time[i].hour)
                return false;
        }
        return true;
    }
    return false;
}