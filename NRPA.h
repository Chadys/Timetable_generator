//
// Created by julie on 19/04/17.
//

#ifndef TIMETABLE_GENERATOR_NRPA_H
#define TIMETABLE_GENERATOR_NRPA_H


#include "DataProvider.h"
#include "Timetable.h"
#include "Graph.h"

class NRPA {
public:
    NRPA(DataProvider &provider_);
    vector<Timetable> generate();

private:
    struct sequence{
        Vertex v;
        vector<GraphProperty> path;
        int score;
    };
    struct playout_choice{
        Vertex v;
        GraphProperty pos;
    };

    DataProvider &provider;
    std::unordered_map<GraphProperty, double> rollout_policy;
    std::mt19937 rand_gen;
    Graph possible_configuration;

    void init_possible_configuration();
    static void update_graph(Vertex v, Graph &graph);
    int get_score(Graph &graph);
    sequence playout(Vertex v, Graph &graph, unsigned int &max_vertices);
    sequence update_rollout_policy(vector<sequence> &possibilities);
    playout_choice random_choice(vector<playout_choice> &choices, vector<double> &probas);
};


#endif //TIMETABLE_GENERATOR_NRPA_H
