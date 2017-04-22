//
// Created by julie on 15/04/17.
//

#include "Students.h"

Students::Students(string lvl_, string subject_, char class_number_, vector<std::shared_ptr<Course>> courses_) :
    lvl(lvl_), subject(subject_), class_number(class_number_ == ';' ? ' ' : class_number_), courses(courses_) {}

bool Students::operator==(const Students& students_) const{
    return this->lvl == students_.lvl &&
           this->subject == students_.subject &&
           this->class_number == students_.class_number;
}

Students::operator std::string() const{
    return this->subject+' '+this->lvl+' '+this->class_number;
}