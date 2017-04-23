//
// Created by julie on 20/04/17.
//

#include "VertexProperty.h"


VertexProperty::VertexProperty(std::shared_ptr<Course> course_, std::shared_ptr<Students> students_, std::shared_ptr<Teacher> teacher_) :
        course(course_), students(students_), teacher(teacher_),
        teacher_time_left(teacher_ && course_ ? teacher->time_by_course[course_] : 0), deleted(false) {}

bool VertexProperty::operator==(const VertexProperty& possibility_) const {
    return this->course == possibility_.course && this->students == possibility_.students &&
        this->teacher == possibility_.teacher && this->time == possibility_.time;
}