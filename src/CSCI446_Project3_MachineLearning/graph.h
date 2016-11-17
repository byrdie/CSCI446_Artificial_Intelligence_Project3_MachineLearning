
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



class Graph;
class Edge;
class Vert;

#include "system.h"

class Graph {
public:
//    Map<string,uint> strm;      // Data structure mapping strings to indices
    vector<Vert*> verts;         // List of vertices in this graph
    vector<Edge*> edges;         // List of all edges in the graph
    Graph();
    Vert * add_vert(string nm);
    Edge * add_edge(uint w, Vert * v1, Vert * v2);
    void remove_vert(Vert * v);
    void remove_edge(Edge * e);
    string print_gviz();
    string print_text();
};



class Vert {
public:
    string name; // Name of this vertex
    vector<Edge*> edges; // List of edges connected to this vertex
    Vert(string nm);
    void attach_edge(Edge * e);
    void detach_edge(Edge * e);
};

class Edge {
public:
    double w; // Weight of this edge
    string name;    // name of this edge
    pair<Vert*, Vert*> apts;     // Connected vertices
    int direction; // 1 for 
    Edge(uint weight, Vert * v1, Vert * v2);
    Edge(string nm, Vert * v1, Vert * v2);
    void attach_vert(Vert * v, uint ind);
    void detach_vert(Vert * v);
};

#endif /* GRAPH_H */

