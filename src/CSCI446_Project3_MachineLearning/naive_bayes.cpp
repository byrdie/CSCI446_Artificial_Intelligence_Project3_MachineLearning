/* 
 * File:   naive_bayes.cpp
 * Author: byrdie
 *
 * Created on November 13, 2016, 9:19 PM
 */

#include "naive_bayes.h"


NaiveBayes::NaiveBayes(Dataset train_data) : Learner(train_data){
    training_dataset.print_dataset(true);
}

void NaiveBayes::learn(){   
    cout << "test" << endl;
}

uint NaiveBayes::answer(datum attrs){
    return 0;
}