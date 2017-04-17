//
// Created by julie on 15/04/17.
//

#ifndef TIMETABLE_GENERATOR_VERTEX_H
#define TIMETABLE_GENERATOR_VERTEX_H


#include <vector>
#include "Course.h"
#include "Students.h"
#include "Teacher.h"

using std::vector;

class Vertex {
public:
    Course course;
    Students students;
    Teacher teacher;
    vector<unsigned long> adjacents;
    Time time;

    Vertex(Course course_, Students students_, Teacher teacher_, vector<unsigned long> adjacents_ = {});
};


#endif //TIMETABLE_GENERATOR_VERTEX_H
