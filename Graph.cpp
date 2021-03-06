//
// Created by julie on 20/04/17.
//

#include <iostream>
#include "Graph.h"

namespace GraphFonc {

    vector<vector<TimeAccessor>> get_all_possible_times(
            Vertex pos, Graph &graph, unordered_map<TimeAccessor, unsigned short> &nb_classrooms_left,
            DataProvider &provider) {
        TimeAccessor ta;
        vector<vector<TimeAccessor>> possible_times;
        unsigned int hours = graph[pos].course->hours_number / graph[pos].course->type;
        for (unsigned int i = 0; i <= graph[pos].teacher->horaires.size() - hours; ++i) {
            unsigned int j;
            vector<TimeAccessor> possible_time = {};
            ta = graph[pos].teacher->horaires[i];
            if(nb_classrooms_left.find(ta) == nb_classrooms_left.end())
                nb_classrooms_left[ta] = provider.all_times[ta.day][ta.hour].classrooms.size();
            if(nb_classrooms_left[ta]) {
                possible_time.push_back(graph[pos].teacher->horaires[i]);
                for (j = 1; j < hours; ++j) {
                    ta = graph[pos].teacher->horaires[i + j];
                    if (ta.day == possible_time.back().day && ta.hour == possible_time.back().hour + 1) {
                        if (nb_classrooms_left.find(ta) == nb_classrooms_left.end())
                            nb_classrooms_left[ta] = provider.all_times[ta.day][ta.hour].classrooms.size();
                        if(!nb_classrooms_left[ta])
                            break;
                        possible_time.push_back(graph[pos].teacher->horaires[i + j]);
                    }
                    else
                        break;
                }
                if (j == hours)
                    possible_times.push_back(possible_time);
            }
        }
        if (graph[pos].course->type == COURS_TP) {
            vector<vector<TimeAccessor>> real_possible_times;
            for (unsigned int i = 0; i < possible_times.size() - 1; ++i) {
                for (unsigned int j = i + 1; j < possible_times.size(); ++j) {
                    //if no time in common
                    if (std::find_first_of(possible_times[i].begin(), possible_times[i].end(),
                                           possible_times[j].begin(), possible_times[j].end())
                        == possible_times[i].end()) {
                        real_possible_times.push_back(possible_times[i]);
                        real_possible_times.back().insert(real_possible_times.back().end(), possible_times[j].begin(),
                                                     possible_times[j].end());
                    }
                }
            }
            possible_times = real_possible_times;
        }
        //teacher and students can't have another course at the same time
        for (auto pair_it = boost::adjacent_vertices(pos, graph); pair_it.first != pair_it.second ; ++pair_it.first) {
            if (!graph[*pair_it.first].time.empty()){
                for (auto it = possible_times.begin() ; it != possible_times.end();){
                    //if at least one time in common
                    if(std::find_first_of(it->begin(), it->end(), graph[*pair_it.first].time.begin(),
                                          graph[*pair_it.first].time.end()) != it->end())
                        it = possible_times.erase(it);
                    else
                        it++;
                }
            }
        }
        return possible_times;
    }

    unsigned int get_final_n_vertices(DataProvider &provider) {
        unsigned int result = 0;
        for (const auto &s : provider.all_students)
            result += s.second->courses.size();
        return result;
    }

    void print_graph(const Graph &g, DataProvider &provider){
        typename boost::graph_traits<Graph>::vertex_iterator it, it_end;
        for (boost::tie(it, it_end) = boost::vertices(g) ; it != it_end ; it++ ) {
            std::cout << ((string) *g[*it].students) << " | " << g[*it].course->title << " | " << g[*it].teacher->name << " |";
            for(TimeAccessor t : g[*it].time)
                std::cout << " " << Time::days.at(t.day) << " " << provider.all_times[t.day][t.hour].hour << 'h';
            std::cout << std::endl;
        }
        std::cout << std::endl << std::endl;
    }
    void print_graph(const FullGraph &g, DataProvider &provider){
        typename boost::graph_traits<FullGraph>::vertex_iterator it, it_end;
        for (boost::tie(it, it_end) = boost::vertices(g) ; it != it_end ; it++ ) {
            std::cout << ((string) *g[*it].students) << " | " << g[*it].course->title << " | " << g[*it].teacher->name << " |";
            for(TimeAccessor t : g[*it].time)
                std::cout << " " << Time::days.at(t.day) << " " << provider.all_times[t.day][t.hour].hour << 'h';
            std::cout << std::endl;
        }
        std::cout << std::endl << std::endl;
    }
    void print_graph(const FilterGraph &g, DataProvider &provider){
        typename boost::graph_traits<FilterGraph>::vertex_iterator it, it_end;
        for (boost::tie(it, it_end) = boost::vertices(g) ; it != it_end ; it++ ) {
            std::cout << ((string) *g[*it].students) << " | " << g[*it].course->title << " | " << g[*it].teacher->name << " |";
            for(TimeAccessor t : g[*it].time)
                std::cout << " " << Time::days.at(t.day) << " " << provider.all_times[t.day][t.hour].hour << 'h';
            std::cout << std::endl;
        }
        std::cout << std::endl << std::endl;
    }
}