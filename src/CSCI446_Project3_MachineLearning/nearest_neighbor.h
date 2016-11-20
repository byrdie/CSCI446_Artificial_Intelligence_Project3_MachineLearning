/* 
 * File:   nearest_neighbor.h
 * Author: brianmarsh
 *
 * Created on November 16, 2016, 6:56 PM
 */

#ifndef NEAREST_NEIGHBOR_H
#define NEAREST_NEIGHBOR_H

#include "learn.h"
#include <math.h>

class NearestNeighbor : public Learner {
public:
    NearestNeighbor(Dataset train_data);
    void learn();
    uint answer(datum attrs);
private:  
    vector<vector<vector<vector<vector<uint>>>>> ptable;
    uint numNeighbors;
    double vdm(vector<uint> attr1, vector<uint> attr2);
    
};

bool cmp_dist(pair<uint, double> p1, pair<uint, double> p2);

#endif /* NEAREST_NEIGHBOR_H */
