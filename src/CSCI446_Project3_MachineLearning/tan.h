
/* 
 * File:   tan.h
 * Author: byrdie
 *
 * Created on November 16, 2016, 4:57 PM
 */

#ifndef TAN_H
#define TAN_H

#include "system.h"
#include "naive_bayes.h"
#include "graph.h"

class TAN : public NaiveBayes {
public:
    TAN(Dataset train_data);
    void learn();
    uint answer(datum attrs);
private:
    Graph<uint> * mst;
    vector<vector<vector<vector<vector<uint>>>>> ptable;
    double cmi(uint a1_ind, uint a2_ind);
    Graph<uint> * kruskal(Graph<uint> * cg);
};

#endif /* TAN_H */

