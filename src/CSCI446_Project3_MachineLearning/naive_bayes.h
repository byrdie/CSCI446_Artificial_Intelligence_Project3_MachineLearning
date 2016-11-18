
/* 
 * File:   naive_bayes.h
 * Author: byrdie
 *
 * Created on November 13, 2016, 9:19 PM
 */

#ifndef NAIVE_BAYES_H
#define NAIVE_BAYES_H

#include "learn.h"

/**
 * This is based off of http://stackoverflow.com/a/20556654
 */
class NaiveBayes : public Learner {
public:
    NaiveBayes(Dataset train_data);
    void learn();
    void count();
    uint answer(datum attrs);
    double laplace_smooth(uint x, uint N);
private:
    vector<vector<vector<uint>>> ptable;    // Table of probabilities used for classification
    
};

#endif /* NAIVE_BAYES_H */

