//
// Created by julie on 18/04/17.
//

#ifndef TIMETABLE_GENERATOR_TIMETABLE_H
#define TIMETABLE_GENERATOR_TIMETABLE_H


#include "Graph.h"

class Timetable {
private:
    struct Period{
        Course &course;
        Teacher &teacher;
    };
    Students &students;
    map<Time, Period> periods;

    Timetable(Students students);
public:
    static vector<Timetable> get_timetables_from_graph(Graph);
    static int evaluate(vector<Timetable> tables);
};


#endif //TIMETABLE_GENERATOR_TIMETABLE_H
