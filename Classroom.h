//
// Created by julie on 15/04/17.
//

#ifndef TIMETABLE_GENERATOR_CLASSROOM_H
#define TIMETABLE_GENERATOR_CLASSROOM_H

#include <string>

class Classroom {
public:
    Classroom(char building_ = '?', unsigned short number_ = 0);
    bool operator==(const Classroom &classroom_) const;
    bool operator<(const Classroom &classroom_) const;
    operator std::string() const;
private:
    char building;
    unsigned short number;
};


#endif //TIMETABLE_GENERATOR_CLASSROOM_H
