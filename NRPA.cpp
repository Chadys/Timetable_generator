//
// Created by julie on 19/04/17.
//

#include "NRPA.h"

NRPA::NRPA(Generator &gen_) : gen(gen_) {

}

Timetable NRPA::generate(){
    vector<sequence> possibilities;
    typename boost::graph_traits<Graph>::vertex_iterator it, it_end;
    unsigned int max_vertices = this->gen.get_max_vertices();
    while(boost::num_vertices(this->gen.possible_configuration) > max_vertices){
        for (boost::tie(it, it_end) = boost::vertices(this->gen.possible_configuration) ;it != it_end ; it++){
            if(!this->gen.possible_configuration[*it].time.empty())
                continue;
            vector<vector<reference_wrapper<Time>>> possible_times =
                    NRPA::get_all_possible_times(this->gen.possible_configuration[*it], this->gen.possible_configuration);
            if(possible_times.empty())
                return Timetable::error;
            for (vector<reference_wrapper<Time>> &possible_time : possible_times){
                Graph temp(this->gen.possible_configuration);
                temp[*it].time = possible_time;
                possibilities.push_back(this->playout(*it, temp, max_vertices));
            }
        }
        sequence best_seq = this->update_rollout_policy(possibilities);
        possibilities.clear();
        this->gen.possible_configuration[best_seq.v].time = best_seq.path.front().time;
        NRPA::update_graph(best_seq.v, this->gen.possible_configuration);
    }
    if (boost::num_vertices(this->gen.possible_configuration) < max_vertices)
        return Timetable::error;
    return Timetable(this->gen.possible_configuration);
}


vector<vector<reference_wrapper<Time>>> NRPA::get_all_possible_times(Possibility& pos, Graph &graph){
    vector<vector<reference_wrapper<Time>>> possible_times;
    unsigned int hours = pos.course.hours_number / pos.course.type;
    for (unsigned int i = 0; i <= pos.teacher.horaires.size()-hours ; ++i) {
        vector<reference_wrapper<Time>> possible_time;
        possible_time.push_back(pos.teacher.horaires[i]);
        for (int j = 1; j < hours; ++j) {
            if(pos.teacher.horaires[i+j].get().day == possible_time.back().get().day &&
                    pos.teacher.horaires[i+j].get().hour == possible_time.back().get().hour+1)
                possible_time.push_back(pos.teacher.horaires[i+j]);
            else
                break;
        }
        if (j == hours)
            possible_times.push_back(possible_time);
    }
    if ( pos.course.type == COURS_TP){
        for (unsigned int i = 0; i < possible_times.size() - 1; ++i) {
            for (int j = i+1; j < possible_times.size(); ++j)
                possible_times[i].insert(possible_times[i].end(), possible_times[j].begin(), possible_times[j].end());
        }
        possible_times.pop_back();
    }
}

void NRPA::update_graph(Vertex& v, Graph &graph){
    // retire les vertices adjacents avec même cours et même classe
    // et ceux avec même cours et même prof si le prof n'a plus d'heure pour ce cours,
    // sinon mets à jour ses heures
}

sequence NRPA::playout(Vertex& v, Graph &graph, unsigned int &max_vertices) const{
    sequence seq;
    seq.v = v;
    seq.path.push_back(graph[v]);
    NRPA::update_graph(v, graph);
    while(boost::num_vertices(graph) > max_vertices){

    }
}

sequence NRPA::update_rollout_policy(vector<sequence> &possibilities){
    //TODO : baisse policy des sequences qui rende l'emploi du temps impossible
    // augmentent la ou les meilleurs séquence (si égalité)
    // renvoit la meilleure séquence (une random parmis les meilleurs si multiple)
}