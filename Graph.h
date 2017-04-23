//
// Created by julie on 17/04/17.
//

#ifndef TIMETABLE_GENERATOR_GRAPH_H
#define TIMETABLE_GENERATOR_GRAPH_H

#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/filtered_graph.hpp>
#include "VertexProperty.h"
#include "DataProvider.h"

using std::vector;
using boost::graph_bundle;

typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS,
        VertexProperty, boost::no_property, unsigned int> FullGraph;
// the unsigned int is the size of the graph with deleted vertices
typedef boost::graph_traits<FullGraph>::vertex_descriptor Vertex;

class NotDeleted {
public:
    NotDeleted() : _g(nullptr) {}
    NotDeleted(FullGraph& graph) : _g(&graph) {}
    bool operator()(const FullGraph::vertex_descriptor v_id) const {
        return !(*_g)[v_id].deleted;
    }
private:
    FullGraph* _g;
};

class NotValidated {
public:
    NotValidated() : _g(nullptr) {}
    NotValidated(FullGraph& graph) : _g(&graph) {}
    bool operator()(const FullGraph::vertex_descriptor v_id) const {
        return !(*_g)[v_id].deleted && (*_g)[v_id].time.empty();
    }
private:
    FullGraph* _g;
};

typedef boost::filtered_graph<FullGraph, boost::keep_all, NotDeleted> Graph;
typedef boost::filtered_graph<FullGraph, boost::keep_all, NotValidated> FilterGraph;

namespace GraphFonc{
    vector<vector<TimeAccessor>> get_all_possible_times(Vertex pos, Graph &graph);
    unsigned int get_final_n_vertices(DataProvider &provider);
    void print_graph(const Graph &g);
}
#endif //TIMETABLE_GENERATOR_GRAPH_H
