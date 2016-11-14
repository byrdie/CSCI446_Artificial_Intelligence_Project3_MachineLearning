
/* 
 * File:   learn.h
 * Author: byrdie
 *
 * Created on November 13, 2016, 8:54 PM
 */

#ifndef LEARN_H
#define LEARN_H


#include "system.h"
#include "data.h"

class Learner {
public:
    Dataset training_dataset;

    Learner(Dataset train_data);
    virtual void learn() = 0;
    virtual uint answer(datum attrs) = 0;

};



#endif /* LEARN_H */

