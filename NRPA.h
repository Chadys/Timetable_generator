//
// Created by julie on 19/04/17.
//

#ifndef TIMETABLE_GENERATOR_NRPA_H
#define TIMETABLE_GENERATOR_NRPA_H


#include "DataProvider.h"
#include "Timetable.h"

class NRPA {
public:
    NRPA(DataProvider &gen_);
    vector<Timetable> generate();

private:
    struct sequence{
        Vertex v;
        vector<Possibility> path;
        int score;
    };
    struct playout_choice{
        Vertex v;
        Possibility pos;
    };

    DataProvider provider;
    std::unordered_map<Possibility, double> rollout_policy;
    std::mt19937 rand_gen;

    static void update_graph(Vertex v, Graph &graph);
    static int get_score(Graph &graph);

    sequence playout(Vertex v, Graph &graph, unsigned int &max_vertices);
    sequence update_rollout_policy(vector<sequence> &possibilities);
    playout_choice random_choice(vector<playout_choice> &choices, vector<double> &probas);
};


#endif //TIMETABLE_GENERATOR_NRPA_H
