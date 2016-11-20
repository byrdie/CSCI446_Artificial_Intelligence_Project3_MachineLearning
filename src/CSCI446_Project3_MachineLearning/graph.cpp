
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

template <class type> bool Graph<type>::loop_exists() {

    for (uint i = 0; i < verts.size(); i++) {
        vector<Vert<type>*> visited;
        vector<Edge<type>*> traveled;
        if (check_loop(verts[i], visited, traveled)) {
            return true;
        }
    }

    return false;

}

template <class type> bool Graph<type>::check_loop(Vert<type> * v, vector<Vert<type>*> visited, vector<Edge<type>*> traveled) {

    /* Detect a loop be checking if we ever get to a node twice */
    if (find(visited.begin(), visited.end(), v) != visited.end()) {
        return true;
    }

    /* push the current node onto the visited stack */
    visited.push_back(v);

    /* Travel to each connected vertex and recursively call */
    for (uint i = 0; i < v->edges.size(); i++) {

        Edge<type> * e = v->edges[i];

        /* Check to see if we have already used this edge */
        if (find(traveled.begin(), traveled.end(), e) != traveled.end()) {
            continue;
        }

        traveled.push_back(e);

        Vert<type> * next_v;
        if (e->verts[0] == v) {
            next_v = e->verts[1];
        } else {
            next_v = e->verts[0];
        }

        if (check_loop(next_v, visited, traveled)) {
            return true;
        }
    }

    return false;
}

template <class type> void Graph<type>::direct(Vert<type>* v) {

    directed = true;

    for (uint i = 0; i < v->edges.size(); i++) {

        Edge<type> * e = v->edges[i];

        if (e->direction == 0) {
            Vert<type> * next_v;
            if (e->verts[0] == v) {
                next_v = e->verts[1];
                e->direction = 1;
            } else {
                next_v = e->verts[0];
                e->direction = -1;
            }
            direct(next_v);
        }
    }

}

/**
 * Add a new vertex to the graph
 * @param v: a vertex object to be added
 * @return the index of the vertex in the list
 */
template <class type> Vert<type> * Graph<type>::add_vert(string nm, type val) {

    Vert<type> * v = new Vert<type>(nm, val);
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
template <class type> Edge<type> * Graph<type>::add_edge(double w, Vert<type> * v1, Vert<type> * v2, uint dir) {

    if (dir != 0) {
        directed = true;
    }

    Edge<type> * e = new Edge<type>(w, "", v1, v2, dir);

    //    /* add edge to the list of edges in each vertex */
    v1->edges.push_back(e);
    v2->edges.push_back(e);

    /* Add edge to sorted list of edges */
    edges.push_back(e);


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

template <class type> Vert<type> * Graph<type>::find_vert(type val) {

    for (uint i = 0; i < verts.size(); i++) {

        if (verts[i]->val == val) {
            return verts[i];
        }

    }

}

template <class type> vector<Vert<type>*> Graph<type>::find_parents(Vert<type> * v) {

    vector<Vert < type>*> ret;

    if (!directed) {
        cout << "called on undirected graph!" << endl;
        return ret;
    }

    /* Loop through all edges in the vertex*/
    for (uint i = 0; i < v->edges.size(); i++) {

        Edge<type> * e = v->edges[i];
        Vert<type> * v1 = e->verts[0];
        Vert<type> * v2 = e->verts[1];
        
        if(e->direction == 0){
            cout << "Erroneous undirected edge, exiting" << endl;
            return ret;
        }

        if (v == v1) {
            if(e->direction == -1){
                ret.push_back(v2);
            }
        } else {
            if(e->direction == 1){
                ret.push_back(v1);
            }
        }

    }
    return ret;
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
        dot << "digraph {" << endl;
    } else {
        dot << "graph {" << endl;
    }



        dot << "rank=LR;" << endl;
        //dot << "ratio=\"fill\";\nsize=\"8.3,11.7!\";\nmargin=0;" << endl;


    /* Write the labels */
    for (uint i = 0; i < verts.size(); i++) {
        string next_nm = verts[i]->name;
        string next_gnm = verts[i]->gname;
        dot << next_gnm << "[label=\"" << next_nm << "\"]" << endl;
    }

    //    dot << "splines=false;" << endl;

    /* Write the edges */
    for (uint i = 0; i < edges.size(); i++) {

        Edge<type> * e = edges[i];
        Vert<type> * v1 = e->verts[0];
        Vert<type> * v2 = e->verts[1];

        if (e->direction == -1) {
            Vert<type> * temp = v2;
            v2 = v1;
            v1 = temp;
        }

        dot << v1->gname;
        if (!directed) {
            dot << "--";
        } else {
            dot << "->";
        }
        dot << v2->gname;
        dot << "[label=\"" << e->name << "\"]" << endl;
    }
    
    dot << "}";
    dot.close();

    /* Call graphviz and construct the pdf */
    string ucmd = "unflatten -f -l5 -c30 -o " + dir + fn + "1.dot " + dir + fn + ".dot";
    cout << ucmd << endl;
    system(ucmd.c_str());

    string cmd = "dot -Tpdf " + dir + fn + "1.dot -o " + dir + fn + ".pdf";
    //    string cmd = "circo -Tpdf " + dir + fn + "1.dot -o " + dir + fn + ".pdf";
    system(cmd.c_str());

  
}

template <class type> void Graph<type>::print_text() {
    /* Write the edges */
    for (uint i = 0; i < edges.size(); i++) {

        Edge<type> * e = edges[i];
        Vert<type> * v1 = e->verts[0];
        Vert<type> * v2 = e->verts[1];
        out << "     [" << v1->name << "]";
        if (!directed) {
            out << "--(" << e->name << ")--";
        } else {
            if (e->direction == 1) {
                out << "--(" << e->name << ")->";
            } else if (e->direction == -1) {
                out << "<-(" << e->name << ")--";
            } else {
                cout << "ERROR in draw graph" << endl;
            }
        }
        out << "[" << v2->name << "]" << "\n";
    }
}

template <class type> Vert<type>::Vert(string nm, type value) {
    name = nm;
    val = value;
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
        name = to_string(w);
    } else {
        name = nm;
    }

    verts[0] = v1;
    verts[1] = v2;
}

bool cmp_edges(Edge<uint> * e1, Edge<uint> * e2) {
    return (e1->w < e2->w);
}


template class Graph<uint>;
template class Vert<uint>;
template class Edge<uint>;

template class Graph<vector<uint>>;
template class Vert<vector<uint>>;
template class Edge<vector<uint>>;

template class Graph<vector<float>>;
template class Vert<vector<float>>;
template class Edge<vector<float>>;
