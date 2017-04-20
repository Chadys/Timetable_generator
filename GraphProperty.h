//
// Created by julie on 20/04/17.
//

#ifndef TIMETABLE_GENERATOR_GRAPHPROPERTY_H
#define TIMETABLE_GENERATOR_GRAPHPROPERTY_H

#include "Course.h"
#include "Students.h"
#include "Teacher.h"
#include "DataProvider.h"

class GraphProperty {
public:
    Course &course;
    Students &students;
    Teacher &teacher;
    vector<TimeAccessor> time;
    unsigned int teacher_time_left;
    static DataProvider &provider;

    GraphProperty(Course &course_ = const_cast<Course&>(Course::null),
                Students &students_ = const_cast<Students&>(Students::null),
                Teacher &teacher_ = const_cast<Teacher&>(Teacher::null));

    GraphProperty& operator=(const GraphProperty& possibility_);

    bool operator==(const GraphProperty& possibility_) const;
};

namespace std
{
    template <>
    struct hash<GraphProperty>
    {
        size_t operator()( const GraphProperty& pos) const {
            std::size_t seed = 0;
            boost::hash_combine(seed, pos.course.title);
            boost::hash_combine(seed, pos.students.lvl);
            boost::hash_combine(seed, pos.students.class_number);
            boost::hash_combine(seed, pos.students.subject);
            boost::hash_combine(seed, pos.teacher.name);
            for (const TimeAccessor &t: pos.time){
                boost::hash_combine(seed, t.day);
                boost::hash_combine(seed, t.hour);
            }
            return seed;
        }
    };
}

#endif //TIMETABLE_GENERATOR_GRAPHPROPERTY_H
