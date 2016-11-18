
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

class Graph;
class Edge;
class Vert;

#include "system.h"

class Graph {
public:
//    Map<string,uint> strm;      // Data structure mapping strings to indices
    bool directed;
    vector<Vert*> verts;         // List of vertices in this graph
    vector<Edge*> edges;         // List of all edges in the graph
    Graph();
    Vert * add_vert(string nm);
    Edge * add_edge(uint w, Vert * v1, Vert * v2, uint dir);
    void remove_vert(Vert * v);
    void remove_edge(Edge * e);
    void print_gviz(string dir, string fn);
    void print_text();
};



class Vert {
public:
    string name; // Name of this vertex
    string gname;   // Name for graphviz
    vector<Edge*> edges; // List of edges connected to this vertex
    Vert(string nm);
};

class Edge {
public:
    double w; // Weight of this edge
    string name;    // name of this edge
    vector<Vert*> verts;     // Connected vertices
    int direction; // 1 for 0->1, 0 for 0--1, -1 for 0<-1
    Edge(double weight, string nm, Vert * v1, Vert * v2, uint dir);
    bool operator<(Edge& e);
};

#endif /* GRAPH_H */

