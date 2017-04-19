//
// Created by julie on 17/04/17.
//

#ifndef TIMETABLE_GENERATOR_GENERATOR_H
#define TIMETABLE_GENERATOR_GENERATOR_H


#include <map>
#include "Time.h"
#include "Course.h"
#include "Students.h"
#include "Teacher.h"
#include "Graph.h"

class Generator {
public:
    vector<vector<Time>> all_times;
    std::unordered_map<string, Course> all_courses;
    vector<Students> all_students;
    vector<Teacher> all_teachers;
    Graph possible_configuration;

    Generator(char *classroom_filename, char *classes_filename, char *teachers_filename);
    unsigned int get_max_vertices();

private:
    void init_times_from_file(const char *file);
    void init_students_and_courses_from_file(const char *file);
    void init_teachers_from_file(const char *file);
    void init_possible_configuration();
};


#endif //TIMETABLE_GENERATOR_GENERATOR_H
