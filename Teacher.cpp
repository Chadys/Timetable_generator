//
// Created by julie on 15/04/17.
//

#include "Teacher.h"

Teacher::Teacher(string name_, vector<reference_wrapper<Time>> horaires_, map<string, unsigned int> courses_names_) :
    name(name_), horaires(horaires_), courses_names(courses_names_) {}

const Teacher Teacher::null("");

bool Teacher::operator==(const Teacher& teacher_) const{
    return this->name == teacher_.name;
}