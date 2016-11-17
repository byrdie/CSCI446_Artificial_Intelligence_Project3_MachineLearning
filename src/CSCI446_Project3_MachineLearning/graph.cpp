
/* 
 * File:   graph.cpp
 * Author: byrdie
 *
 * Created on November 16, 2016, 6:24 PM
 */

#include "graph.h"

/* Constructor */
Graph::Graph() {

}

/**
 * Add a new vertex to the graph
 * @param v: a vertex object to be added
 * @return the index of the vertex in the list
 */
Vert * Graph::add_vert(string nm) {

    Vert * v = new Vert(nm);
    verts.push_back(v);
    return v;

}

/**
 * Add edge into graph
 * @param w
 * @param v1
 * @param v2
 * @return 
 */
Edge * Graph::add_edge(uint w, Vert * v1, Vert * v2) {

    Edge * e = new Edge(w, v1, v2);

    v1->attach_edge(e);
    v2->attach_edge(e);
    e->attach_vert(v1, 0);
    e->attach_vert(v2, 1);
    edges.push_back(e);

    return e;
}

void Graph::remove_vert(Vert * v) {

    auto it = find(verts.begin(), verts.end(), v);
    verts.erase(it);

    for (uint i = 0; i < v->edges.size(); i++) {
        Edge * e = v->edges[i];
        Vert * vc;
        if (v == e->apts.first) {
            vc = e->apts.second;
        } else {
            vc = e->apts.first;
        }
        auto vc_it = find(vc->edges.begin(), vc->edges.end(), e);
        vc->edges.erase(vc_it);
        delete e;
    }

    delete v;
}

void Graph::remove_edge(Edge * e) {

    auto it = find(edges.begin(), edges.end(), e);

    Vert * v1 = e->apts.first;
    Vert * v2 = e->apts.second;
    auto it1 = find(v1->edges.begin(), v1->edges.end(), e);
    auto it2 = find(v2->edges.begin(), v2->edges.end(), e);
    v1->edges.erase(it1);
    v2->edges.erase(it2);

    delete e;
}