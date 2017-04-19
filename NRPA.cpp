//
// Created by julie on 19/04/17.
//

#include "NRPA.h"

NRPA::NRPA(Generator &gen_) : gen(gen_) {
    std::random_device rd;
    this->rand_gen = std::mt19937(rd());
}

vector<Timetable> NRPA::generate(){
    vector<sequence> possibilities;
    typename boost::graph_traits<Graph>::vertex_iterator it, it_end;
    unsigned int max_vertices = this->gen.get_max_vertices();
    while(boost::num_vertices(this->gen.possible_configuration) > max_vertices){
        for (boost::tie(it, it_end) = boost::vertices(this->gen.possible_configuration) ;it != it_end ; it++){
            if(!this->gen.possible_configuration[*it].time.empty())
                continue;
            vector<vector<reference_wrapper<Time>>> possible_times = Generator::get_all_possible_times(
                    this->gen.possible_configuration[*it], this->gen.possible_configuration);
            if(possible_times.empty())
                return vector<Timetable>();
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
    if (boost::num_vertices(this->gen.possible_configuration) != max_vertices)
        return vector<Timetable>();
    return Timetable::get_timetables_from_graph(this->gen.possible_configuration);
}

NRPA::sequence NRPA::playout(Vertex v, Graph &graph, unsigned int &max_vertices){
    sequence seq;
    seq.v = v;
    seq.path.push_back(graph[v]);
    NRPA::update_graph(v, graph);
    vector<playout_choice> playout_choices;
    vector<double> probas;
    playout_choice next_mod;
    typename boost::graph_traits<Graph>::vertex_iterator it, it_end;

    while(boost::num_vertices(graph) > max_vertices){
        for (boost::tie(it, it_end) = boost::vertices(graph) ;it != it_end ; it++){
            if(!graph[*it].time.empty())
                continue;
            vector<vector<reference_wrapper<Time>>> possible_times =
                    Generator::get_all_possible_times(graph[*it], graph);
            if(possible_times.empty()) {
                seq.score  = INT_MIN;
                return seq;
            }
            for (vector<reference_wrapper<Time>> &possible_time : possible_times){
                Possibility pos(graph[*it]);
                pos.time = possible_time;
                if(this->rollout_policy.find(pos) == this->rollout_policy.end())
                    this->rollout_policy[pos] = 100.;
                playout_choices.push_back({*it, pos});
                probas.push_back(this->rollout_policy[pos]);
            }
        }
        next_mod = random_choice(playout_choices, probas);
        graph[next_mod.v].time = next_mod.pos.time;
        NRPA::update_graph(next_mod.v, graph);
        playout_choices.clear();
        probas.clear();
    }
    seq.score = get_score(graph);
    return seq;
}

void NRPA::update_graph(Vertex v, Graph &graph){
    // update teacher hours for this class
    // delete adjacent vertices with same course and sames students
    // update teacher hours for this class in adjacent vertices
    // delete it if teacher doesn't have anymore hours
    graph[v].teacher_time_left -= graph[v].course.hours_number;
    vector<Vertex> to_be_deleted; //because of iterator invalidation
    for (auto pair_it = boost::adjacent_vertices(v, graph); pair_it.first != pair_it.second ; ++pair_it.first) {
        if(graph[*pair_it.first].course == graph[v].course){
            if (graph[*pair_it.first].students == graph[v].students)
                to_be_deleted.push_back(*pair_it.first);
            else if (graph[*pair_it.first].teacher == graph[v].teacher){
                if(!graph[v].teacher_time_left)
                    to_be_deleted.push_back(*pair_it.first);
                else
                    graph[*pair_it.first].teacher_time_left = graph[v].teacher_time_left;
            }
        }
    }
    for (Vertex &v : to_be_deleted){
        boost::clear_vertex(v, graph);
        boost::remove_vertex(v, graph);
    }
}

NRPA::sequence NRPA::update_rollout_policy(vector<sequence> &possibilities){
    // Lower probability of sequences that made a timetable generation impossible
    // increase the best one(s) (if equality)
    // return the sequence leading to the best timetable
    // (chosen at random if equality)
    vector<sequence> bests;
    for (sequence &possibility : possibilities){
        if(possibility.score == INT_MIN){
            for(Possibility &pos : possibility.path){
                double proba = this->rollout_policy[pos];
                if(proba > 99.)
                    proba -=0.1;
                else if(proba > 90)
                    proba -=0.2;
                else if(proba > 0.1)
                    proba -=0.05;
                this->rollout_policy[pos] = proba;
            }
        }
        else{
            if(bests.empty() || bests.front().score < possibility.score)
                bests = vector<sequence>({possibility});
            else if(!bests.empty() && bests.front().score == possibility.score)
                bests.push_back(possibility);
        }
    }
    for (sequence &best : bests)
        for(Possibility &pos : best.path){
            double proba = this->rollout_policy[pos];
            if(proba < 101.)
                proba +=0.1;
            else if(proba < 110.)
                proba +=0.2;
            else if(proba < 200.)
                proba +=0.05;
            this->rollout_policy[pos] = proba;
        };
    std::uniform_int_distribution<unsigned int> dis(0,bests.size()-1);
    return bests[dis(this->rand_gen)];
}

NRPA::playout_choice NRPA::random_choice(vector<playout_choice> &choices, vector<double> &probas){
    // return random choice based on rollout policy
    // random generator using probability
    std::discrete_distribution<unsigned int> dis(probas.begin(), probas.end());
    return choices[dis(this->rand_gen)];
}

int NRPA::get_score(Graph &graph){
    vector<Timetable> timetables = Timetable::get_timetables_from_graph(graph);
    return Timetable::evaluate(timetables);
}