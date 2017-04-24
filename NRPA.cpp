//
// Created by julie on 19/04/17.
//

#include "NRPA.h"
unsigned short NRPA::N_PLAYOUT=2;

NRPA::NRPA(DataProvider &provider_) : provider(provider_) {
    std::random_device rd;
    this->rand_gen = std::mt19937(rd());
    this->init_possible_configuration();
}

void NRPA::init_possible_configuration(){
    std::map<string, vector<Vertex>> teachers_map;
    unsigned int first = 0, last;

    for (auto &s : this->provider.all_students){
        for (std::shared_ptr<Course> &c : s.second->courses)
            for (auto &t : this->provider.all_teachers)
                if (t.second->time_by_course.find(c) != t.second->time_by_course.end())
                    teachers_map[t.second->name].push_back(
                            boost::add_vertex(VertexProperty(c, s.second, t.second), this->_g));
        // Link between all vertices having the same students
        last = boost::num_vertices(this->_g);
        for (unsigned int i = first; i < last-1; ++i)
            for (unsigned int j = i+1; j < last; ++j) {
                auto it = boost::vertices(this->_g).first;
                boost::add_edge(*std::next(it,i), *std::next(it,j), this->_g);
            };
        first = last;
    }
    // Link between all vertices having the same teacher
    for (auto &v : teachers_map){
        for (unsigned int i = 0; i < v.second.size()-1; ++i)
            for (unsigned int j = i+1; j < v.second.size(); ++j)
                boost::add_edge(v.second[i], v.second[j], this->_g);
    }
    this->_g[graph_bundle] = boost::num_vertices(this->_g);
    this->lock_unmovable_teachers();
}

Graph NRPA::get_graph(){
    return Graph(this->_g, boost::keep_all(), NotDeleted(this->_g));
}

vector<Timetable> NRPA::generate(){
    vector<sequence> possibilities;
    Graph g = this->get_graph();
    FilterGraph filter(this->_g, boost::keep_all(), NotValidated(this->_g));
    typename boost::graph_traits<FilterGraph>::vertex_iterator it, it_end;

    while(true){
        for (boost::tie(it, it_end) = boost::vertices(filter) ;it != it_end ; it++){
            vector<vector<TimeAccessor>> possible_times =
                    GraphFonc::get_all_possible_times(*it, g, this->nb_classrooms_left, this->provider);
            if(possible_times.empty())
                return vector<Timetable>();
            for (vector<TimeAccessor> &possible_time : possible_times){
                for (int _ = 0; _ < this->N_PLAYOUT; ++_) {
                    FullGraph temp(this->_g);
                    temp[*it].time = possible_time;
                    possibilities.push_back(this->playout(*it, temp, this->nb_classrooms_left));
                }
            }
        }
        if(possibilities.empty())
            break;
        sequence best_seq = this->update_rollout_policy(possibilities);
        if(best_seq.score==INT_MIN)
            return vector<Timetable>();
        possibilities.clear();
        this->_g[best_seq.v].time = best_seq.path.front().time;
        NRPA::update_graph(best_seq.v, g, this->nb_classrooms_left);
    }
    if (this->_g[graph_bundle] < GraphFonc::get_final_n_vertices(this->provider))
        return vector<Timetable>();
    return Timetable::get_timetables_from_graph(g);
}

NRPA::sequence NRPA::playout(Vertex v, FullGraph &g, unordered_map<TimeAccessor, unsigned short> p_nb_classrooms_left){

    Graph graph(g, boost::keep_all(), NotDeleted(g));
    FilterGraph filter(g, boost::keep_all(), NotValidated(g));
    NRPA::update_graph(v, graph, p_nb_classrooms_left);
    sequence seq;
    seq.v = v;
    seq.path.push_back(graph[v]);

    vector<playout_choice> playout_choices;
    vector<double> probas;
    playout_choice next_mod;
    typename boost::graph_traits<FilterGraph>::vertex_iterator it, it_end;

    while(true){
        for (boost::tie(it, it_end) = boost::vertices(filter) ; it != it_end ; it++){
            if(!graph[*it].time.empty())
                continue;
            vector<vector<TimeAccessor>> possible_times =
                    GraphFonc::get_all_possible_times(*it, graph, p_nb_classrooms_left, this->provider);
            if(possible_times.empty()) {
                seq.score  = INT_MIN;
                return seq;
            }
            for (vector<TimeAccessor> &possible_time : possible_times){
                VertexProperty pos(graph[*it]);
                pos.time = possible_time;
                if(this->rollout_policy.find(pos) == this->rollout_policy.end())
                    this->rollout_policy[pos] = 100.;
                playout_choices.push_back({*it, pos});
                probas.push_back(this->rollout_policy[pos]);
            }
        }
        if (playout_choices.empty())
            break;
        next_mod = random_choice(playout_choices, probas);
        graph[next_mod.v].time = next_mod.pos.time;
        seq.path.push_back(graph[next_mod.v]);
        NRPA::update_graph(next_mod.v, graph, p_nb_classrooms_left);
        playout_choices.clear();
        probas.clear();
    }
    seq.score = get_score(graph);
    return seq;
}

void NRPA::update_graph(Vertex v, Graph &graph, unordered_map<TimeAccessor, unsigned short> &u_nb_classrooms_left){
    // update teacher hours for this class
    // delete adjacent vertices with same course and sames students
    // update teacher hours for this class in adjacent vertices
    // delete it if teacher doesn't have anymore hours
    graph[v].teacher_time_left -= graph[v].course->hours_number;
    for (auto pair_it = boost::adjacent_vertices(v, graph); pair_it.first != pair_it.second ; ++pair_it.first) {
        if (graph[*pair_it.first].students == graph[v].students){
            //course already planned
            //or disallow having the same teacher in different subjects
            if(graph[*pair_it.first].course == graph[v].course || graph[*pair_it.first].teacher == graph[v].teacher){
                graph[graph_bundle]--;
                graph[*pair_it.first].deleted = true;
            }
        }
        //teacher can't exceed their allotted hours for one course
        else if (graph[*pair_it.first].teacher == graph[v].teacher && graph[*pair_it.first].course == graph[v].course){
            if(!graph[v].teacher_time_left && graph[*pair_it.first].time.empty()){
                graph[graph_bundle]--;
                graph[*pair_it.first].deleted = true;
            }
            else
                graph[*pair_it.first].teacher_time_left = graph[v].teacher_time_left;
        }
    }
    //delete number of classroom available for each timeslot
    for(const TimeAccessor &ta : graph[v].time)
        u_nb_classrooms_left[ta]--;
}

NRPA::sequence NRPA::update_rollout_policy(vector<sequence> &possibilities){
    // Lower probability of sequences that made a timetable generation impossible
    // increase the best one(s) (if equality)
    // return the sequence leading to the best timetable
    // (chosen at random if equality)
    vector<sequence> bests;
    for (sequence &possibility : possibilities){
        if(possibility.score == INT_MIN){
            for(VertexProperty &pos : possibility.path){
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
        for(VertexProperty &pos : best.path){
            double proba = this->rollout_policy[pos];
            if(proba < 101.)
                proba +=0.1;
            else if(proba < 110.)
                proba +=0.2;
            else if(proba < 200.)
                proba +=0.05;
            this->rollout_policy[pos] = proba;
        };
    if(!bests.size())
        return possibilities[0];
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
    if (graph[graph_bundle] < GraphFonc::get_final_n_vertices(this->provider))
        return INT_MIN;
    vector<Timetable> timetables = Timetable::get_timetables_from_graph(graph);
    return Timetable::evaluate(timetables, this->provider);
}

void NRPA::lock_unmovable_teachers() {
    //validate Vertex where the teacher has the same number of hours in disponibility and course
    // (AKA no other time choice
    Graph g = this->get_graph();
    for (auto pair_it = boost::vertices(g); pair_it.first != pair_it.second ; ++pair_it.first) {
        if (g[*pair_it.first].teacher->horaires.size() == g[*pair_it.first].teacher_time_left){
            vector<vector<TimeAccessor>> possible_times;
            if ((possible_times = GraphFonc::get_all_possible_times(
                    *pair_it.first, g, this->nb_classrooms_left, this->provider)).size() == 1){
                g[*pair_it.first].time = possible_times.front();
                NRPA::update_graph(*pair_it.first, g, this->nb_classrooms_left);
            }
        }
    }
}