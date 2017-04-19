//
// Created by julie on 18/04/17.
//

#ifndef TIMETABLE_GENERATOR_TIMETABLE_H
#define TIMETABLE_GENERATOR_TIMETABLE_H


#include "Graph.h"
#include "DataProvider.h"

class Timetable {
private:
    struct Period{
        Course &course;
        Teacher &teacher;

        Period(Course &course_ = const_cast<Course&>(Course::null),
               Teacher &teacher_ = const_cast<Teacher&>(Teacher::null));
        Period& operator=(const Period& timetable_);
    };
    struct RefTimeComparator {
        bool operator()(const Time& t1, const Time& t2) const {
            return t1<t2;
        }
    };

    Students &students;
    map<reference_wrapper<Time>, Period, RefTimeComparator> periods;

public:
    static vector<Timetable> get_timetables_from_graph(Graph &graph, DataProvider &provider);
    static int evaluate(vector<Timetable> tables);

    Timetable(Students &students_ = const_cast<Students&>(Students::null));
    Timetable& operator=(const Timetable& timetable_);
};


#endif //TIMETABLE_GENERATOR_TIMETABLE_H
