//
// Created by julie on 20/04/17.
//

#include "GraphProperty.h"


GraphProperty::GraphProperty(std::shared_ptr<Course> course_, std::shared_ptr<Students> students_, std::shared_ptr<Teacher> teacher_) :
        course(course_), students(students_), teacher(teacher_),
        teacher_time_left(teacher_ != nullptr && course_ != nullptr ? teacher->time_by_course[course_] : 0) {}

bool GraphProperty::operator==(const GraphProperty& possibility_) const {
    return this->course == possibility_.course && this->students == possibility_.students &&
        this->teacher == possibility_.teacher && this->time == possibility_.time;
}