//
// Created by julie on 15/04/17.
//

#include "Vertex.h"

Vertex::Vertex(Course course_, Students students_, Teacher teacher_, vector<unsigned long> adjacents_) :
    course(course_), students(students_), teacher(teacher_), adjacents(adjacents_), time(Time::null) {
}