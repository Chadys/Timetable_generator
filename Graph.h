//
// Created by julie on 17/04/17.
//

#ifndef TIMETABLE_GENERATOR_GRAPH_H
#define TIMETABLE_GENERATOR_GRAPH_H

#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include "Course.h"
#include "Students.h"
#include "Teacher.h"

using std::vector;

struct Possibility {
    Course &course;
    Students &students;
    Teacher &teacher;
    Time time;
    unsigned int teacher_time_left;

    Possibility(Course &course_ = const_cast<Course&>(Course::null),
                Students &students_ = const_cast<Students&>(Students::null),
                Teacher &teacher_ = const_cast<Teacher&>(Teacher::null)) :
            course(course_), students(students_), teacher(teacher_),
            time(Time::null), teacher_time_left(teacher_.courses_names[course_.title]) {}
};
;
typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, Possibility> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

#endif //TIMETABLE_GENERATOR_GRAPH_H
