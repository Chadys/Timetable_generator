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
    vector<reference_wrapper<Time>> time;
    unsigned int teacher_time_left;

    Possibility(Course &course_ = const_cast<Course&>(Course::null),
                Students &students_ = const_cast<Students&>(Students::null),
                Teacher &teacher_ = const_cast<Teacher&>(Teacher::null)) :
            course(course_), students(students_), teacher(teacher_),
            teacher_time_left(teacher_.courses_names[course_.title]) {}

    Possibility& operator=(const Possibility& possibility_){
        this->course = possibility_.course;
        this->students = possibility_.students;
        this->teacher = possibility_.teacher;
        this->time = possibility_.time;
        this->teacher_time_left = possibility_.teacher_time_left;
    }

    bool operator==(const Possibility& possibility_) const{
        this->course == possibility_.course;
        this->students == possibility_.students;
        this->teacher == possibility_.teacher;
        this->time == possibility_.time;
    }
};
typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, Possibility> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;


namespace std
{
    template <>
    struct hash<Possibility>
    {
        size_t operator()( const Possibility& pos) const {
            std::size_t seed = 0;
            boost::hash_combine(seed, pos.course.title);
            boost::hash_combine(seed, pos.students.lvl);
            boost::hash_combine(seed, pos.students.class_number);
            boost::hash_combine(seed, pos.students.subject);
            boost::hash_combine(seed, pos.teacher.name);
            boost::hash_combine(seed, pos.time.day);
            boost::hash_combine(seed, pos.time.hour);
            return seed;
        }
    };
}
#endif //TIMETABLE_GENERATOR_GRAPH_H
