//
// Created by julie on 19/04/17.
//

#ifndef TIMETABLE_GENERATOR_NRPA_H
#define TIMETABLE_GENERATOR_NRPA_H


#include "Generator.h"
#include "Timetable.h"

class NRPA {
public:
    NRPA(Generator &gen_);
    Timetable generate();

private:
    struct sequence{
        Vertex v;
        vector<Possibility> path;
        int score;
    };

    Generator gen;
    std::unordered_map<Possibility, double> rollout_policy;

    static vector<vector<reference_wrapper<Time>>> get_all_possible_times(Possibility& pos, Graph &graph);
    static void update_graph(Vertex& v, Graph &graph);

    sequence playout(Vertex& v, Graph &graph, unsigned int &max_vertices) const;
    sequence update_rollout_policy(vector<sequence> &possibilities);
};


#endif //TIMETABLE_GENERATOR_NRPA_H
