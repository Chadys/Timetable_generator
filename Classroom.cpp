//
// Created by julie on 15/04/17.
//

#include <sstream>
#include <iomanip>
#include "Classroom.h"

Classroom::Classroom(char building_, unsigned short number_) :
        building(building_), number(number_) {}

bool Classroom::operator==(const Classroom &classroom_) const{
    return this->building == classroom_.building && this->number == classroom_.number;
}

bool Classroom::operator<(const Classroom &classroom_) const{
    return this->building < classroom_.building ||
            (this->building == classroom_.building && this->number < classroom_.number);
}

Classroom::operator std::string() const{
    std::stringstream stream;
    stream << this->building << ' ' << std::setfill('0') << std::setw(3) << this->number;
    return stream.str();
}