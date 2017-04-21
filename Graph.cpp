//
// Created by julie on 20/04/17.
//

#include "Graph.h"

namespace GraphFonc {

    vector<vector<TimeAccessor>> get_all_possible_times(GraphProperty &pos, Graph &graph) {
        vector<vector<TimeAccessor>> possible_times;
        unsigned int hours = pos.course->hours_number / pos.course->type;
        for (unsigned int i = 0; i <= pos.teacher->horaires.size() - hours; ++i) {
            int j;
            vector<TimeAccessor> possible_time = {};
            possible_time.push_back(pos.teacher->horaires[i]);
            for (j = 1; j < hours; ++j) {
                if (pos.teacher->horaires[i + j].day == possible_time.back().day &&
                    pos.teacher->horaires[i + j].hour == possible_time.back().hour + 1)
                    possible_time.push_back(pos.teacher->horaires[i + j]);
                else
                    break;
            }
            if (j == hours)
                possible_times.push_back(possible_time);
        }
        if (pos.course->type == COURS_TP) {
            vector<vector<TimeAccessor>> real_possible_times;
            for (unsigned int i = 0; i < possible_times.size() - 1; ++i) {
                for (int j = i + 1; j < possible_times.size(); ++j) {
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
        return possible_times;
    }

    unsigned int get_max_vertices(Graph &graph, DataProvider &provider) {
        unsigned int result = 0;
        for (const auto &s : provider.all_students)
            result += s.second->courses.size();
        return result;
    }
}