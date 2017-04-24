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
    Graph get_graph();

private:
    struct sequence{
        Vertex v;
        vector<VertexProperty> path;
        int score;
    };
    struct playout_choice{
        Vertex v;
        VertexProperty pos;
    };

    DataProvider &provider;
    std::unordered_map<VertexProperty, double> rollout_policy;
    std::mt19937 rand_gen;
    FullGraph _g;
    unordered_map<TimeAccessor, unsigned short> nb_classrooms_left;
    static unsigned short N_PLAYOUT;

    void init_possible_configuration();
    void update_graph(Vertex v, Graph &graph, unordered_map<TimeAccessor, unsigned short> &u_nb_classrooms_left); //map ref for update
    int get_score(Graph &graph);
    sequence playout(Vertex v, FullGraph &g, unordered_map<TimeAccessor, unsigned short> p_nb_classrooms_left); //map copy for new data
    sequence update_rollout_policy(vector<sequence> &possibilities);
    playout_choice random_choice(vector<playout_choice> &choices, vector<double> &probas);
    void lock_unmovable_teachers();
};


#endif //TIMETABLE_GENERATOR_NRPA_H
