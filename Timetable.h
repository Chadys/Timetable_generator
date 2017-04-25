//
// Created by julie on 18/04/17.
//

#ifndef TIMETABLE_GENERATOR_TIMETABLE_H
#define TIMETABLE_GENERATOR_TIMETABLE_H


#include "Graph.h"
#include "DataProvider.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wall"
#include "ExcelFormat/ExcelFormat.h"
#pragma GCC diagnostic pop

#define MALUS_ONE_COURSE_IN_DAY -100
#define MALUS_IDLE_HOUR -3
#define MALUS_SAME_COURSE_CONSECUTIVE -10
#define MALUS_NO_FREE_DAY -10
#define BONUS_LUNCHTIME 10
#define BONUS_PER_FREE_DAY 10
#define NEUTRAL_LUNCHTIME 0

class Timetable {
private:
    struct Period{
        std::shared_ptr<Course> course;
        std::shared_ptr<Teacher> teacher;
        Classroom classroom;

        Period(std::shared_ptr<Course> course_ = std::shared_ptr<Course>(),
               std::shared_ptr<Teacher> teacher_ = std::shared_ptr<Teacher>());
    };

    std::shared_ptr<Students>  students;
    map<TimeAccessor, Period> periods; //ordered to make evaluation go in timely order
    static void add_classrooms(vector<Timetable> &timetables, DataProvider &provider);

public:
    static vector<Timetable> get_timetables_from_graph(Graph &graph);
    static int evaluate(const vector<Timetable> &tables, DataProvider &provider);
    static void create_excel(vector<Timetable> &timetables, DataProvider &provider, const char *filename);

    Timetable(std::shared_ptr<Students> students_ = std::shared_ptr<Students>());
};


#endif //TIMETABLE_GENERATOR_TIMETABLE_H
