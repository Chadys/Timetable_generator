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
        std::shared_ptr<Course> course;
        std::shared_ptr<Teacher> teacher;

        Period(std::shared_ptr<Course> course_ = nullptr, std::shared_ptr<Teacher> teacher_ = nullptr);
    };

    std::shared_ptr<Students>  students;
    map<TimeAccessor, Period> periods;

public:
    static vector<Timetable> get_timetables_from_graph(Graph &graph, DataProvider &provider);
    static int evaluate(vector<Timetable> tables, DataProvider &provider);

    Timetable(std::shared_ptr<Students> students_ = nullptr);
};


#endif //TIMETABLE_GENERATOR_TIMETABLE_H
