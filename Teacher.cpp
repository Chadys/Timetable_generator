//
// Created by julie on 15/04/17.
//

#include "Teacher.h"

Teacher::Teacher(string name_, vector<TimeAccessor> horaires_, map<std::shared_ptr<Course>, unsigned int> time_by_course_) :
    name(name_), horaires(horaires_), time_by_course(time_by_course_) {}

bool Teacher::operator==(const Teacher& teacher_) const{
    return this->name == teacher_.name;
}