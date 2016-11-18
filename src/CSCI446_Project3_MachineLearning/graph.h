
/* 
 * File:   graph.h
 * Author: byrdie
 *
 * Created on November 16, 2016, 6:24 PM
 */

#ifndef GRAPH_H
#define GRAPH_H/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#define FORWARD 1
#define UNDIRECTED 0
#define BACKWARD -1

template <class type> class Graph;
template <class type> class Edge;
template <class type> class Vert;

#include "system.h"

template <class type> class Graph {
public:
//    Map<string,uint> strm;      // Data structure mapping strings to indices
    bool directed;
    vector<Vert<type>*> verts;         // List of vertices in this graph
    vector<Edge<type>*> edges;         // List of all edges in the graph
    Graph();
    Vert<type> * add_vert(string nm, type val);
    Edge<type> * add_edge(uint w, Vert<type> * v1, Vert<type> * v2, uint dir);
    void remove_vert(Vert<type> * v);
    void remove_edge(Edge<type> * e);
    void print_gviz(string dir, string fn);
    void print_text();
};



template <class type> class Vert {
public:
    type val;
    string name; // Name of this vertex
    string gname;   // Name for graphviz
    vector<Edge<type>*> edges; // List of edges connected to this vertex
    Vert(string nm);
};

template <class type> class Edge {
public:
    double w; // Weight of this edge
    string name;    // name of this edge
    vector<Vert<type>*> verts;     // Connected vertices
    int direction; // 1 for 0->1, 0 for 0--1, -1 for 0<-1
    Edge(double weight, string nm, Vert<type> * v1, Vert<type> * v2, uint dir);
    bool operator<(Edge<type>& e);
};

#endif /* GRAPH_H */

