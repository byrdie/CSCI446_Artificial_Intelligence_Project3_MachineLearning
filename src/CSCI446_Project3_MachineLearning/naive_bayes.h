
/* 
 * File:   naive_bayes.h
 * Author: byrdie
 *
 * Created on November 13, 2016, 9:19 PM
 */

#ifndef NAIVE_BAYES_H
#define NAIVE_BAYES_H

#include "learn.h"

class NaiveBayes : public Learner {
public:
    NaiveBayes(Dataset train_data);
    void learn();
    uint answer(datum attrs);
    private:
};

#endif /* NAIVE_BAYES_H */

