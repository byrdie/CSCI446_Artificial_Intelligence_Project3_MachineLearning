/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "nearest_neighbor.h"

NearestNeighbor::NearestNeighbor(Dataset train_data, uint norm, uint k) : NaiveBayes(train_data) {

    p = norm;
    numNeighbors = k;
}


uint NearestNeighbor::answer(datum attrs) {
    
    out << "The vector to check is given by \n";
    td.print_datum(attrs);
    out << "\n \n";

    numNeighbors = 10;
    uint p = 5;

    vector<pair<uint, double>> distances;

    /* Loop over every datum in the dataset */
    for (uint i = 0; i < td.data.size(); i++) {

        // put the class and distance into distances vector
        pair<uint, double> temp;
        temp.first = td.data[i][0];
        temp.second = vdm(attrs, td.data[i]);
        distances.push_back(temp);

    }

    // Sort distance list to find nearest neighbors
    std::sort(distances.begin(), distances.end(), cmp_dist);

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

double NearestNeighbor::vdm(vector<uint> attrs1, vector<uint> attrs2){
    
    double sum1 = 0.0;
    
    /* Loop over the attributes */
    for(uint k = 1; k < attrs1.size(); k++){
        
        uint l = attrs1[k];
        uint n = attrs2[k];
        
        double sum2 = 0.0;
        
        /* Loop over the classes */
        for(uint j = td.vmin[0]; j <= td.vmax[0]; j++){
            
            double P_axc = laplace_smooth((double) ptable[j][k][l], (double) ptable[0][k][l]);
            double P_ayc = laplace_smooth((double) ptable[j][k][n], (double) ptable[0][k][n]);
            
            sum2 += pow(abs(P_axc - P_ayc), p);
            
        }
        sum1 += sum2;               
        
    }
    return pow(sum1, 1.0 / (double) p);
}