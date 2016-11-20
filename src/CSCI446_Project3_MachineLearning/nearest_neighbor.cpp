/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "nearest_neighbor.h"

NearestNeighbor::NearestNeighbor(Dataset train_data) : Learner(train_data) {

    /* discretize the training set before starting */
    td.discretize();
    td.normalize();


}

void NearestNeighbor::learn() {

        /* Build probability table*/
    /* Loop through every datum in the dataset */
    for (uint i = 0; i < td.data.size(); i++) {

        datum attrs = td.data[i];
        uint j = attrs[0]; // The class label is the first index

        ptable[j][0][0][0][0]++; // Increment the prior probability P(C)
        ptable[0][0][0][0][0]++; // Increment the total

        /* Loop through the attributes for the first time */
        for (uint k = 1; k < attrs.size(); k++) { // The location of the first attribute is the second index

            uint l = attrs[k]; // The value of the first attribute is the third index

            ptable[j][k][l][0][0]++; // Increment P(x,C)
            ptable[0][k][l][0][0]++; // Increment P(x)         

            /* Loop through the attributes */
            for (uint m = 1; m < attrs.size(); m++) { // The location of the attribute is the second index

                uint n = attrs[m]; // The value of the attribute is the third index

                ptable[j][k][l][m][n]++; // Increment P(x,y,C)
                ptable[0][k][l][m][n]++; // Increment P(x,y)         

            }

        }
    }
    
}



uint NearestNeighbor::answer(datum attrs) {
    
    out << "The vector to check is given by \n";
    td.print_datum(attrs);
    out << "\n \n";

    numNeighbors = 10;
    uint p = 5;

    vector<pair<uint, double>> distances;

    /* Loop over every datum in the dataset */
    for (uint i = 0; i < td.ndata.size(); i++) {

        // Calculate Minkowski distance between data points
        double sum = 0.0;
        for (uint k = 1; k < attrs.size(); k++) {
            double newPoint = attrs[k];
            double storedPoint = td.ndata[i][k];
            sum += pow(abs(newPoint - storedPoint),p);
        }

        // put the class and distance into distances vector
        pair<uint, double> temp;
        temp.first = td.data[i][0];
        temp.second =pow(sum, 1.0 / (double) p);
        distances.push_back(temp);

    }

    // Sort distance list on element 0 (distance)
    
    std::sort(distances.begin(), distances.end(), cmp_dist);
//    out << "The distances to all elements of the dataset are:\n";
//    for (uint i = 0; i < distances.size(); i++) {
//        out << "class: ";
//        td.print_val(0, distances[i].first);
//        out << ", distance: " << distances[i].second << "\n";
//    }

    /* Find the class with the most occurrences within the nearest neighbors */
    vector<uint> class_dist(td.vmax[0] + 1);
    out << "The closest neighbors were:\n";
    for (uint i = 0; i < numNeighbors; i++) {
        out << "class: ";
        td.print_val(0, distances[i].first);
        out << ", distance: " << distances[i].second << "\n";
        class_dist[distances[i].first]++;
    }
    return distance(class_dist.begin(), max_element(class_dist.begin(), class_dist.end()));
}

bool cmp_dist(pair<uint, double> p1, pair<uint, double> p2) {
    return p1.second < p2.second;
}