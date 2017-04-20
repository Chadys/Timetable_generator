//
// Created by julie on 15/04/17.
//

#ifndef TIMETABLE_GENERATOR_COURSE_H
#define TIMETABLE_GENERATOR_COURSE_H


#include <string>

using std::string;

enum COURSE_TYPE{
    UNITAIRE = 1,
    COURS_TP = 2
};

class Course {
public:
    string title;
    unsigned short hours_number;
    unsigned short type;

    Course(string title_ = "", unsigned short hours_number_ = 4, COURSE_TYPE type_ = COURS_TP);
    bool operator==(const Course& course_) const;
};


#endif //TIMETABLE_GENERATOR_COURSE_H
