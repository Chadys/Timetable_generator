//
// Created by julie on 18/04/17.
//

#include "Timetable.h"

Timetable::Timetable(std::shared_ptr<Students> students_) : students(students_) {}

Timetable::Period::Period(std::shared_ptr<Course> course_, std::shared_ptr<Teacher> teacher_) : course(course_), teacher(teacher_) {}


vector<Timetable> Timetable::get_timetables_from_graph(Graph &graph, DataProvider &provider){
    std::map<string, Timetable> timetables;
    typename boost::graph_traits<Graph>::vertex_iterator it, it_end;
    for (boost::tie(it, it_end) = boost::vertices(graph) ; it != it_end ; it++ ) {
        if(timetables.find(static_cast<string>(*graph[*it].students)) == timetables.end())
            timetables.emplace(*graph[*it].students, Timetable(graph[*it].students));
        for (int i = 0; i < graph[*it].course->type; ++i)
            timetables[*graph[*it].students].periods[graph[*it].time[i*2]] = {graph[*it].course, graph[*it].teacher};
    }
    vector<Timetable> timetables_vec;
    std::transform(timetables.begin(), timetables.end(),
                   std::back_inserter(timetables_vec), [](auto &kv){ return kv.second;});
    return timetables_vec;
}

int Timetable::evaluate(vector<Timetable> tables, DataProvider &provider){
    int score = 0;
    unsigned short used_days = 0;
    unsigned short first_hour = provider.all_times.front().front().hour;
    for (const Timetable &t : tables){
        TimeAccessor last_time;
        Period last_period;
        for (auto &kv : t.periods){
            if(last_time){
                if (last_time.day != kv.first.day)
                    used_days++;
                else{
                    //if free period
                    if(kv.first.hour - last_time.hour > 1){
                        //lunch time
                        if(last_time.hour+first_hour < 12 || kv.first.hour+first_hour > 13)
                            score+=10;
                        else
                            score-=3*(kv.first.hour - last_time.hour);
                    }
                    //TODO : in else, lower score if classrooms !=
                }
            }
            last_time = kv.first;
            last_period = kv.second;
        }
        // bonus for every free day;
        score += (4-used_days)*5;
        used_days = 0;
    }
    return score;
}

//TODO : add classroom managment using Time.courses_number