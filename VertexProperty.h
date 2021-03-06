//
// Created by julie on 20/04/17.
//

#ifndef TIMETABLE_GENERATOR_VERTEXPROPERTY_H
#define TIMETABLE_GENERATOR_VERTEXPROPERTY_H

#include "Course.h"
#include "Students.h"
#include "Teacher.h"
#include "DataProvider.h"

class VertexProperty {
public:
    std::shared_ptr<Course> course;
    std::shared_ptr<Students> students;
    std::shared_ptr<Teacher> teacher;
    vector<TimeAccessor> time;
    unsigned int teacher_time_left;
    bool deleted;

    VertexProperty(std::shared_ptr<Course> course_ = std::shared_ptr<Course>(),
                  std::shared_ptr<Students> students_ = std::shared_ptr<Students>(),
                  std::shared_ptr<Teacher> teacher_ = std::shared_ptr<Teacher>());

    bool operator==(const VertexProperty& possibility_) const;
};

namespace std
{
    template <>
    struct hash<VertexProperty>
    {
        size_t operator()( const VertexProperty& pos) const {
            std::size_t seed = 0;
            boost::hash_combine(seed, pos.course->title);
            boost::hash_combine(seed, pos.students->lvl);
            boost::hash_combine(seed, pos.students->class_number);
            boost::hash_combine(seed, pos.students->subject);
            boost::hash_combine(seed, pos.teacher->name);
            for (const TimeAccessor &t: pos.time){
                boost::hash_combine(seed, t.day);
                boost::hash_combine(seed, t.hour);
            }
            return seed;
        }
    };
}

#endif //TIMETABLE_GENERATOR_VERTEXPROPERTY_H
