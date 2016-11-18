
/* 
 * File:   graph.cpp
 * Author: byrdie
 *
 * Created on November 16, 2016, 6:24 PM
 */

#include "graph.h"

/* Constructor */
template <class type> Graph<type>::Graph() {
    directed = false;
}

/**
 * Add a new vertex to the graph
 * @param v: a vertex object to be added
 * @return the index of the vertex in the list
 */
template <class type> Vert<type> * Graph<type>::add_vert(string nm, type val) {

    Vert<type> * v = new Vert<type>(nm);
    verts.push_back(v);
    return v;

}

/**
 * Add edge into graph
 * @param w
 * @param v1
 * @param v2
 * @return the newly created edge
 */
template <class type> Edge<type> * Graph<type>::add_edge(uint w, Vert<type> * v1, Vert<type> * v2, uint dir) {

    if (dir != 0) {
        directed = true;
    }

    Edge<type> * e = new Edge<type>(w, "", v1, v2, dir);

    /* add edge to the list of edges in each vertex */
    v1->edges.push_back(e);
    v2->edges.push_back(e);

    /* Add edge to sorted list of edges */
    auto it = lower_bound(edges.begin(), edges.end(), e);
    edges.insert(it, e);

    return e;
}

template <class type> void Graph<type>::remove_vert(Vert<type> * v) {

    auto it = find(verts.begin(), verts.end(), v);
    verts.erase(it);

    for (uint i = 0; i < v->edges.size(); i++) {
        Edge<type> * e = v->edges[i];
        Vert<type> * vc;
        if (v == e->verts[0]) {
            vc = e->verts[1];
        } else {
            vc = e->verts[0];
        }
        auto vc_it = find(vc->edges.begin(), vc->edges.end(), e);
        vc->edges.erase(vc_it);
        delete e;
    }

    delete v;
}

template <class type> void Graph<type>::remove_edge(Edge<type> * e) {

    auto it = find(edges.begin(), edges.end(), e);
    edges.erase(it);

    Vert<type> * v1 = e->verts[0];
    Vert<type> * v2 = e->verts[1];
    auto it1 = find(v1->edges.begin(), v1->edges.end(), e);
    auto it2 = find(v2->edges.begin(), v2->edges.end(), e);
    v1->edges.erase(it1);
    v2->edges.erase(it2);

    delete e;
}

/**
 * Print graph to pdf file
 * @param dir directory to put the file
 * @param fn filename (WITHOUT EXTENSION PLEASE!!!!!!!!!!!)
 */
template <class type> void Graph<type>::print_gviz(string dir, string fn) {



    /* open file to write DOT */
    ofstream dot;
    dot.open(dir + fn + ".dot");

    /* Check if the graph is directed or undirected */
    if (directed) {
        dot << "digraph graphname {" << endl;
    } else {
        dot << "graph graphname {" << endl;
    }

//    dot << "rank=LR;" << endl;
//    dot << "ratio=\"fill\";\nsize=\"8.3,11.7!\";\nmargin=0;" << endl;


    /* Write the labels */
    for (uint i = 0; i < verts.size(); i++) {
        string next_nm = verts[i]->name;
        string next_gnm = verts[i]->gname;
        dot << next_gnm << "[label=\"" << next_nm << "\"]" << endl;
    }

    /* Write the edges */
    for (uint i = 0; i < edges.size(); i++) {
        Edge<type> * e = edges[i];
        Vert<type> * v1 = e->verts[0];
        Vert<type> * v2 = e->verts[1];
        dot << v1->gname;
        if (!directed) {
            dot << "--";
        } else {
            if (e->direction == 1) {
                dot << "->";
            } else if (e->direction == -1) {
                dot << "<-";
            } else {
                cout << "ERROR in draw graph" << endl;
            }
        }
        dot << v2->gname;
        dot << "[label=\"" << e->name << "\"]" << endl;
    }

    dot << "}";
    dot.close();

    /* Call graphviz and construct the pdf */
    string ucmd = "unflatten -f -l10 -c5 -o " + dir + fn + "1.dot " + dir + fn + ".dot";
    cout << ucmd << endl;
    system(ucmd.c_str());
    
    string cmd = "dot -Tpdf " + dir + fn + "1.dot -o " + dir + fn + ".pdf";
    system(cmd.c_str());


}

template <class type> Vert<type>::Vert(string nm) {
    name = nm;

    /* Erase the spaces in the name string to use as graphviz id */
    gname = nm;
    gname.erase(remove(gname.begin(), gname.end(), ' '), gname.end());
    gname.erase(remove(gname.begin(), gname.end(), '-'), gname.end());
}

template <class type> Edge<type>::Edge(double weight, string nm, Vert<type>* v1, Vert<type>* v2, uint dir) {

    verts.assign(2, 0);
    direction = dir;
    w = weight;
    if (nm.empty()) {
        name = to_string((uint) w);
    } else {
        name = nm;
    }

    verts[0] = v1;
    verts[1] = v2;
}

template <class type> bool Edge<type>::operator<(Edge<type>& e) {
    return w < e.w;
}

template class Graph<uint>;
template class Vert<uint>;
template class Edge<uint>;

template class Graph<vector<uint>>;
template class Vert<vector<uint>>;
template class Edge<vector<uint>>;