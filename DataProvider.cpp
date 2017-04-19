//
// Created by julie on 17/04/17.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "DataProvider.h"

DataProvider::DataProvider(char *classroom_filename, char *classes_filename, char *teachers_filename){
    this->init_times_from_file(classroom_filename);
    this->init_students_and_courses_from_file(classes_filename);
    this->init_teachers_from_file(teachers_filename);
    this->init_possible_configuration();
}

void DataProvider::init_times_from_file(const char *file){
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

void DataProvider::init_students_and_courses_from_file(const char *file){
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

void DataProvider::init_teachers_from_file(const char *file){
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
                this->all_teachers.back().courses_names[course_name] = qt;
            }
        };
    fstream.close();
}

void DataProvider::init_possible_configuration(){
    std::map<string, vector<Vertex>> teachers_map;
    unsigned int first = 0, last;

    for (Students &s : all_students){
        for (Course &c : s.courses)
            for (Teacher &t : all_teachers)
                if (t.courses_names.find(c.title) != t.courses_names.end())
                    teachers_map[t.name].push_back(boost::add_vertex(Possibility(c, s, t), this->possible_configuration));
        // Link between all vertices having the same students
        last = boost::num_vertices(this->possible_configuration);
        for (unsigned int i = first; i < last-1; ++i)
            for (unsigned int j = i+1; j < last; ++j)
                boost::add_edge(i, j, this->possible_configuration);
        first = last;
    }
    // Link between all vertices having the same teacher
    for (auto &v : teachers_map){
        for (unsigned int i = 0; i < v.second.size()-1; ++i)
            for (unsigned int j = i+1; j < v.second.size(); ++j)
                boost::add_edge(i, j, this->possible_configuration);
    }
}

unsigned int DataProvider::get_max_vertices(){
    unsigned int result = 0;
    for (Students &s : this->all_students)
        result += s.courses.size();
    return result;
}


vector<vector<reference_wrapper<Time>>> DataProvider::get_all_possible_times(Possibility& pos, Graph &graph){
    vector<vector<reference_wrapper<Time>>> possible_times;
    unsigned int hours = pos.course.hours_number / pos.course.type;
    for (unsigned int i = 0; i <= pos.teacher.horaires.size()-hours ; ++i) {
        int j;
        vector<reference_wrapper<Time>> possible_time = {};
        possible_time.push_back(pos.teacher.horaires[i]);
        for (j = 1; j < hours; ++j) {
            if(pos.teacher.horaires[i+j].get().day == possible_time.back().get().day &&
               pos.teacher.horaires[i+j].get().hour == possible_time.back().get().hour+1)
                possible_time.push_back(pos.teacher.horaires[i+j]);
            else
                break;
        }
        if (j == hours)
            possible_times.push_back(possible_time);
    }
    if (pos.course.type == COURS_TP){
        vector<vector<reference_wrapper<Time>>> real_possible_times;
        for (unsigned int i = 0; i < possible_times.size() - 1; ++i) {
            for (int j = i+1; j < possible_times.size(); ++j) {
                if(std::find_first_of(possible_times[i].begin(), possible_times[i].end(),
                                      possible_times[j].begin(), possible_times[j].end(),
                                      [] (const Time &t1, const Time &t2) -> bool { return t1 == t2; })
                   == possible_times[i].end()){
                    real_possible_times.push_back(possible_times[i]);
                    possible_times.back().insert(possible_times.back().end(), possible_times[j].begin(), possible_times[j].end());
                }
            }
        }
        possible_times = real_possible_times;
    }
}