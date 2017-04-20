//
// Created by julie on 17/04/17.
//

#ifndef TIMETABLE_GENERATOR_GENERATOR_H
#define TIMETABLE_GENERATOR_GENERATOR_H


#include <memory>
#include <unordered_map>
#include "Time.h"
#include "Course.h"
#include "Students.h"
#include "Teacher.h"

class DataProvider {
public:
    vector<vector<Time>> all_times;
    std::unordered_map<string, std::shared_ptr<Course>> all_courses;
    std::unordered_map<string, std::shared_ptr<Students>> all_students;
    std::unordered_map<string, std::shared_ptr<Teacher>> all_teachers;

    void init(char *classroom_filename, char *classes_filename, char *teachers_filename);

private:
    void init_times_from_file(const char *file);
    void init_students_and_courses_from_file(const char *file);
    void init_teachers_from_file(const char *file);
};


#endif //TIMETABLE_GENERATOR_GENERATOR_H
