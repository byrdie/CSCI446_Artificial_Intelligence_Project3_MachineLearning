/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "nearest_neighbor.h"

NearestNeighbor::NearestNeighbor(Dataset train_data) : Learner(train_data) {

    /* discretize the training set before starting */
    td.discretize();


}

void NearestNeighbor::learn() {

    /* Loop through every datum in the dataset */
    for (uint i = 0; i < td.data.size(); i++) {

        datum attrs = td.data[i];
        
        vector<uint> attr_vals;
        // loop over every value for each attribute
        for (uint k = 0; k < attrs.size(); k++) {
            
            attr_vals.push_back(attrs[k]); // Initialize to zero
            
        }
        
        // Not sure if I should just use td.data here instead
        storedValues.push_back(attr_vals); // Store each datum in a local array
        
    }

}

uint NearestNeighbor::answer(datum attrs) {
    
    numNeighbors = 5;
    
    /* Loop over every datum in the dataset */
    for (uint i = 0; i <= td.data.size(); i++) { 
                
        // Calculate Minkowski distance between data points
        uint sum = 0;
        for (uint k = 1; k < attrs.size(); k++){
            uint newPoint = attrs[k];
            uint storedPoint = storedValues[i][k];
            sum += pow(newPoint - storedPoint, 2.0); 
        }
        
        // put the class and distance into distances vector
        vector<uint> temp;
        temp.push_back(storedValues[i][0]); // distances[i][1] = class
        temp.push_back(pow(sum, 1.0 / 2.0)); // distances[i][0] = distance
        distances.push_back(temp);
        
        
    }
    
    // Sort distance list on element 0 (distance)
    std::sort(distances.begin(), distances.end());
    
    // Find highest occurring class within number of neighbors given
    uint finalClass;
    int frequency = 1;
    int maxFreq = 0;
    vector<vector<uint>> classCount;
    for (int i = 0; i < numNeighbors; i++) {
        if (distances[i][1] == distances[i+1][1]) {
            frequency++;
            if (frequency > maxFreq) {
                maxFreq = frequency;
                finalClass = distances[i][1];
            }
        }
        else {
            frequency = 1;
        }
    }
    
    return finalClass;
}
