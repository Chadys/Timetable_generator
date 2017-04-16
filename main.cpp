#include <iostream>
#include <map>
#include <functional>
#include "Time.h"
#include "Course.h"
#include "Students.h"

int main(int argc, char* argv[]) {
    if (argc != 4){
        std::cout << "Usage: Timetable_Generator [Filename for classes infos]" << ' ' <<
                "[Filename for teachers infos]" << ' ' << "[Filename for classrooms infos]" << std::endl;
        return 1;
    }

    vector<vector<Time>> times = Time::get_times_from_file(argv[3]);
    std::map<string, Course> courses;
    vector<Students> students = Students::get_students_and_init_courses_from_file(courses, argv[1]);
    return 0;
}