//
// Created by julie on 17/04/17.
//

#include <fstream>
#include <sstream>
#include "Generator.h"

Generator::Generator(char *classroom_filename, char *classes_filename, char *teachers_filename){
    this->init_times_from_file(classroom_filename);
    this->init_students_and_courses_from_file(classes_filename);
    this->init_teachers_from_file(teachers_filename);
    this->init_possible_configurations();
}

void Generator::init_times_from_file(const char *file){
    string line;
    std::ifstream fstream(file);
    char building;
    unsigned short number;
    unsigned int n_days, first_hour, last_hour, hour;

    if (fstream && std::getline(fstream, line)) {
        std::istringstream sstream(line);
        sstream >> n_days >> first_hour >> last_hour;
        for(unsigned int i = 0; i < n_days; i++){
            vector<Time> hours;
            for(unsigned int j = first_hour; j <= last_hour; j++)
                hours.emplace_back(static_cast<DAY>(i), j);
            this->all_times.push_back(hours);
        }
        while (std::getline(fstream, line)){
            if(line == "")
                continue;
            sstream = std::istringstream(line);
            sstream >> building >> number;
            for (unsigned int i = 0; i < n_days && std::getline(fstream, line); ++i) {
                sstream = std::istringstream(line);
                sstream >> hour; //jump over day
                while (sstream >> hour)
                    this->all_times[i][hour-first_hour].classrooms.emplace_back(building, number);
            }
        }
    }
    fstream.close();
}

void Generator::init_students_and_courses_from_file(const char *file){
    string line, lvl, subject, title;
    char class_number;
    unsigned short hours_number, type;
    std::ifstream fstream(file);

    if (fstream)
        while(std::getline(fstream, line)) {
            std::istringstream sstream(line);
            sstream >> lvl >> class_number >> subject;
            this->all_students.emplace_back(lvl, subject, class_number);
            while (std::getline(fstream, line) && line != ""){
                sstream = std::istringstream(line);
                sstream >> type >> hours_number >> title;
                if(all_courses.find(title) == all_courses.end())
                    all_courses[title] = Course(title, hours_number, static_cast<COURSE_TYPE >(type));
                this->all_students.back().courses.push_back(this->all_courses[title]);
            }
        };
    fstream.close();
}

void Generator::init_teachers_from_file(const char *file){
    string line, name, course_name;
    unsigned short first_hour = this->all_times.front().front().hour, hour;
    unsigned int qt;
    std::ifstream fstream(file);

    if (fstream)
        while(std::getline(fstream, line)) {
            std::istringstream sstream(line);
            sstream >> name;
            this->all_teachers.emplace_back(name);
            for (unsigned short i = 0; i < this->all_times.size(); ++i) {
                std::getline(fstream, line);
                sstream = std::istringstream(line);
                sstream >> hour;
                while (sstream >> hour)
                    this->all_teachers.back().horaires.push_back(this->all_times[i][hour-first_hour]);
            }
            while (std::getline(fstream, line) && line != ""){
                sstream = std::istringstream(line);
                sstream >> qt >> course_name;
                this->all_teachers.back().courses.emplace_back(this->all_courses.at(course_name), qt);
            }
        };
    fstream.close();
}

void Generator::init_possible_configurations(){

}