//
// Created by julie on 19/04/17.
//

#include "NRPA.h"

NRPA::NRPA(DataProvider &provider_) : provider(provider_) {
    std::random_device rd;
    this->rand_gen = std::mt19937(rd());
    this->init_possible_configuration();
}

void NRPA::init_possible_configuration(){
    std::map<string, vector<Vertex>> teachers_map;
    auto first = boost::vertices(this->possible_configuration).first, last = first;

    for (auto &s : this->provider.all_students){
        for (std::shared_ptr<Course> &c : s.second->courses)
            for (auto &t : this->provider.all_teachers)
                if (t.second->time_by_course.find(c) != t.second->time_by_course.end())
                    teachers_map[t.second->name].push_back(
                            boost::add_vertex(GraphProperty(c, s.second, t.second), this->possible_configuration));
        // Link between all vertices having the same students
        last = boost::vertices(this->possible_configuration).second;
        for (auto i = first; i != last; ++i)
            for (auto j = std::next(i,1); j != last; ++j)
                boost::add_edge(*i, *j, this->possible_configuration);
        first = last;
    }
    // Link between all vertices having the same teacher
    for (auto &v : teachers_map){
        for (unsigned int i = 0; i < v.second.size()-1; ++i)
            for (unsigned int j = i+1; j < v.second.size(); ++j)
                boost::add_edge(v.second[i], v.second[j], this->possible_configuration);
    }
}

vector<Timetable> NRPA::generate(){
    vector<sequence> possibilities;
    typename boost::graph_traits<Graph>::vertex_iterator it, it_end;
    boost::tie(it, it_end) = boost::vertices(this->possible_configuration);
    unsigned int times_added = 0;

    while(times_added < boost::num_vertices(this->possible_configuration)){
        times_added = 0;
        for (boost::tie(it, it_end) = boost::vertices(this->possible_configuration) ;it != it_end ; it++){
            if(!this->possible_configuration[*it].time.empty()) {
                times_added++;
                continue;
            }
            vector<vector<TimeAccessor>> possible_times = GraphFonc::get_all_possible_times(
                    this->possible_configuration[*it], this->possible_configuration);
            if(possible_times.empty())
                return vector<Timetable>();
            for (vector<TimeAccessor> &possible_time : possible_times){
                Graph temp(this->possible_configuration);
                temp[*it].time = possible_time;
                possibilities.push_back(this->playout(*it, temp));
            }
        }
        sequence best_seq = this->update_rollout_policy(possibilities);
        possibilities.clear();
        this->possible_configuration[best_seq.v].time = best_seq.path.front().time;
        NRPA::update_graph(best_seq.v, this->possible_configuration);
    }
    if (boost::num_vertices(this->possible_configuration) <
            GraphFonc::get_max_vertices(this->possible_configuration, this->provider))
        return vector<Timetable>();
    return Timetable::get_timetables_from_graph(this->possible_configuration, this->provider);
}

NRPA::sequence NRPA::playout(Vertex v, Graph &graph){
    sequence seq;
    seq.v = v;
    seq.path.push_back(graph[v]);
    vector<playout_choice> playout_choices;
    vector<double> probas;
    playout_choice next_mod;
    typename boost::graph_traits<Graph>::vertex_iterator it, it_end;
    boost::tie(it, it_end) = boost::vertices(graph);
    unsigned int times_added = 0;

    while(boost::num_vertices(graph) > times_added){
        times_added = 0;
        for (boost::tie(it, it_end) = boost::vertices(graph) ; it != it_end ; it++){
            if(!graph[*it].time.empty()) {
                times_added++;
                continue;
            }
            vector<vector<TimeAccessor>> possible_times =
                    GraphFonc::get_all_possible_times(graph[*it], graph);
            if(possible_times.empty()) {
                seq.score  = INT_MIN;
                return seq;
            }
            for (vector<TimeAccessor> &possible_time : possible_times){
                GraphProperty pos(graph[*it]);
                pos.time = possible_time;
                if(this->rollout_policy.find(pos) == this->rollout_policy.end())
                    this->rollout_policy[pos] = 100.;
                playout_choices.push_back({*it, pos});
                probas.push_back(this->rollout_policy[pos]);
            }
        }
        next_mod = random_choice(playout_choices, probas);
        graph[next_mod.v].time = next_mod.pos.time;
        seq.path.push_back(graph[v]);
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
    graph[v].teacher_time_left -= graph[v].course->hours_number;
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
            for(GraphProperty &pos : possibility.path){
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
        for(GraphProperty &pos : best.path){
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
    vector<Timetable> timetables = Timetable::get_timetables_from_graph(graph, this->provider);
    return Timetable::evaluate(timetables, this->provider);
}