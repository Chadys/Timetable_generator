//
// Created by julie on 17/04/17.
//

#ifndef TIMETABLE_GENERATOR_GRAPH_H
#define TIMETABLE_GENERATOR_GRAPH_H

#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include "GraphProperty.h"
#include "DataProvider.h"

using std::vector;

typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, GraphProperty> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

namespace GraphFonc{
    vector<vector<TimeAccessor>> get_all_possible_times(Vertex pos, Graph &graph);
    unsigned int get_final_n_vertices(DataProvider &provider);
    void print_graph(const Graph &g);
}
#endif //TIMETABLE_GENERATOR_GRAPH_H
