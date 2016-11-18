
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

class TAN : public NaiveBayes {
public:
    TAN(Dataset train_data);
    void learn();
    void answer();
private:
    
};

#endif /* TAN_H */

