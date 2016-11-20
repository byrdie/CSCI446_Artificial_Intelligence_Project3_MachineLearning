/* 
 * File:   nearest_neighbor.h
 * Author: brianmarsh, byrdie
 *
 * Created on November 16, 2016, 6:56 PM
 */

#ifndef NEAREST_NEIGHBOR_H
#define NEAREST_NEIGHBOR_H

#include "learn.h"
#include "naive_bayes.h"
#include <math.h>

class NearestNeighbor : public NaiveBayes {
public:
    NearestNeighbor(Dataset train_data, uint norm, uint k);
//    void learn();
    uint answer(datum attrs);
private:  
    uint p;
    uint numNeighbors;
    double vdm(vector<uint> attrs1, vector<uint> attrs2);
    
};

bool cmp_dist(pair<uint, double> p1, pair<uint, double> p2);

#endif /* NEAREST_NEIGHBOR_H */
