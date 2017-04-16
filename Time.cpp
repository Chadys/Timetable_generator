//
// Created by julie on 15/04/17.
//

#include <fstream>
#include <sstream>
#include "Time.h"

Time::Time(DAY day_, unsigned short hour_, vector<Classroom> classrooms_) :
    day(day_), hour(hour_), classrooms(classrooms_), courses_number(0) {}

const Time Time::null(SUNDAY, 0, {});

bool Time::operator==(const Time &time_) const{
    return this->day == time_.day && this->hour == time_.hour;
}

bool Time::operator!=(const Time &time_) const{
    return !(this->day == time_.day && this->hour == time_.hour);
}

Time::operator bool() const{
    return *this != Time::null;
}

vector<vector<Time>> &Time::get_times_from_file(const char *file){
    std::string line;
    vector<vector<Time>> times;
    std::ifstream fstream(file);
    char building;
    unsigned short number;
    unsigned int n_days, first_hour, last_hour, hour;

    if (fstream && std::getline(fstream, line)) {
        std::istringstream sstream(line);
        sstream >> n_days;
        sstream >> first_hour;
        sstream >> last_hour;
        for(unsigned int i = 0; i < n_days; i++){
            vector<Time> hours;
            for(unsigned int j = first_hour; j <= last_hour; j++)
                hours.emplace_back(static_cast<DAY>(i), j);
            times.push_back(hours);
        }
        while (std::getline(fstream, line)){
            if(line == "")
                continue;
            sstream = std::istringstream(line);
            sstream >> building;
            sstream >> number;
            for (unsigned int i = 0; i < n_days && std::getline(fstream, line); ++i) {
                sstream = std::istringstream(line);
                sstream >> hour; //jump over day
                while (sstream >> hour)
                    times[i][hour-first_hour].classrooms.emplace_back(building, number);
            }
        }
    }
    fstream.close();
    return times;
}