//
// Created by julie on 15/04/17.
//

#include <fstream>
#include <sstream>
#include "Students.h"

Students::Students(string lvl_, string subject_, char class_number_, vector<reference_wrapper<Course>> courses_) :
    lvl(lvl_), subject(subject_), class_number(class_number_), courses(courses_) {}

vector<Students> Students::get_students_and_init_courses_from_file(std::map<string, Course> &all_courses, const char *file){
    string line, lvl, subject, title;
    char class_number;
    vector<Students> students;
    unsigned short hours_number, type;
    std::ifstream fstream(file);

    if (fstream)
        while(std::getline(fstream, line)) {
            std::istringstream sstream(line);
            sstream >> lvl >> class_number >> subject;
            students.emplace_back(lvl, subject, class_number);
            while (std::getline(fstream, line) && line != ""){
                sstream = std::istringstream(line);
                sstream >> type >> hours_number >> title;
                if(all_courses.find(title) == all_courses.end())
                    all_courses[title] = Course(title, hours_number, static_cast<COURSE_TYPE >(type));
                students.back().courses.push_back(all_courses[title]);
            }
        };
    fstream.close();
    return students;
}
