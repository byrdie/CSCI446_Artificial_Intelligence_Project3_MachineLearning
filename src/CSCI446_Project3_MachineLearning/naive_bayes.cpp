/* 
 * File:   naive_bayes.cpp
 * Author: byrdie
 *
 * Created on November 13, 2016, 9:19 PM
 */

#include "naive_bayes.h"


NaiveBayes::NaiveBayes(Dataset train_data) : Learner(train_data){
    
    training_dataset.discretize();
    
//    ptable = new vector<vector<vector<uint>>>()
    
}

void NaiveBayes::learn(){   
    
    /* Loop through every datum in the dataset */
    for(uint i  = 0; i < training_dataset.data.size(); i++){
        
        datum attrs = training_dataset.data[i];
        uint j = attrs[0];   // The class label is the first index
        
        /* Loop through the attributes */
        for(uint k = 0; k < attrs.size(); k++){ // The location of the attribute is the second index
            
            uint l = attrs[k];      // The value of the attribute is the third index
            
            /* increment the count of the appropriate index */
            (*ptable)[j][k][l] += 0;
            
        }
        
    }
    
}

uint NaiveBayes::answer(datum attrs){
    return 0;
}
