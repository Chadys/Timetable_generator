//
// Created by julie on 15/04/17.
//

#ifndef TIMETABLE_GENERATOR_STUDENTS_H
#define TIMETABLE_GENERATOR_STUDENTS_H

#include <vector>
#include <map>
#include <boost/functional/hash_fwd.hpp>
#include "Course.h"

using std::vector;

class Students {
public:
    string lvl;
    string subject;
    char class_number;
    vector<string> courses;
    static const Students null;

    Students(string lvl_="", string subject_="", char class_number_ = '\0', vector<string> courses_ = {});
    bool operator==(const Students& students_) const;
    operator std::string() const;
};
#endif //TIMETABLE_GENERATOR_STUDENTS_H