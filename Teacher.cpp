//
// Created by julie on 15/04/17.
//

#include <fstream>
#include <sstream>
#include "Teacher.h"


Teacher::Teacher(string name_, vector<reference_wrapper<Time>> horaires_, vector<CourseTime_qt> courses_) :
    name(name_), horaires(horaires_), courses(courses_) {}

Teacher::CourseTime_qt::CourseTime_qt(Course &course_, unsigned int time_qt_) : course(course_), time_qt(time_qt_) {}

vector<Teacher> Teacher::get_teachers_from_file(std::map<string, Course> &all_courses, vector<vector<Time>> &all_times, const char *file){
    string line, name, course_name;
    unsigned short first_hour = all_times.front().front().hour, hour;
    unsigned int qt;
    vector<Teacher> teachers;
    std::ifstream fstream(file);

    if (fstream)
        while(std::getline(fstream, line)) {
            std::istringstream sstream(line);
            sstream >> name;
            teachers.emplace_back(name);
            for (unsigned short i = 0; i < all_times.size(); ++i) {
                std::getline(fstream, line);
                sstream = std::istringstream(line);
                sstream >> hour;
                while (sstream >> hour)
                    teachers.back().horaires.push_back(all_times[i][hour-first_hour]);
            }
            while (std::getline(fstream, line) && line != ""){
                sstream = std::istringstream(line);
                sstream >> qt >> course_name;
                teachers.back().courses.emplace_back(all_courses.at(course_name), qt);
            }
        };
    fstream.close();
    return teachers;
}
//S_Chalençon
//0 9 10
//1
//2
//3 11 12
//4
//4 Utilisation_d'ordinateurs_en_réseau
//
//A_R_d'Allonnes
//0 11 12
//1 15 16 17
//2 16 17
//3
//4
//4 Programmation_logique
//
//        R_Mazouzi
//0 13 14 15 16
//1 16 17
//2
//3 9 10 11 12 17 18
//4
//6 Programmation_logique_/_Programmation_impérative_(soutien)