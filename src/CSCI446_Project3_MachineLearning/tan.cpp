
/* 
 * File:   tan.cpp
 * Author: byrdie
 *
 * Created on November 16, 2016, 4:57 PM
 */

#include "tan.h"
#include "graph.h"

TAN::TAN(Dataset train_data) : NaiveBayes(train_data) {

    /* discretize the training set before starting */
    td.discretize();

    /* We need to initialize a non-rectangular 3D array. Therefore we must loop 
     and build the vector from the inside out.*/
    for (uint j = 0; j <= td.vmax[0]; j++) { // Loop over the class values

        datum attrs = td.data[0]; // just use the zeroth datum as representative example

        /* loop over all attributes */
        vector<vector < vector<vector < uint>>>> ptable_row1;
        for (uint k = 0; k < attrs.size(); k++) {

            /* loop over every value for each attribute */
            vector<vector < vector<uint>> attr_vals1;
            for (uint l = 0; l <= td.vmax[k]; l++) {

                /* loop over all attributes */
                vector<vector < uint>> ptable_row2;
                for (uint k = 0; k < attrs.size(); k++) {

                    /* loop over every value for each attribute */
                    vector<uint> attr_vals2;
                    for (uint l = 0; l <= td.vmax[k]; l++) {
                        attr_vals2.push_back(0); // Initialize to zero
                    }
                    ptable_row2.push_back(attr_vals2);
                }
                attr_vals1.push_back(ptable_row2);
            }
            ptable_row1.push_back(attr_vals1);
        }
        ptable.push_back(ptable_row1);
    }
}

void TAN::learn() {

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

    /* Construct complete graph out of attributes */
    Graph<uint> g;
    datum attrs = td.data[0];
    for (uint i = 1; i < attrs.size(); i++) {
        g.add_vert(td.attr_names.left.find(i)->second, i);
    }

    for (uint i = 0; i < g.verts.size(); i++) {
        for (uint j = 0; j < g.verts.size(); j++) {

            Vert<uint> * v1 = g.verts[i];
            Vert<uint> * v2 = g.verts[j];
            double weight = cmi(v1->val, v2->val);
            
            g.add_edge(weight, v1, v2, 0);

        }
    }
    
    g.print_gviz("", "tan");
}

uint TAN::answer(datum attrs) {

}

double TAN::cmi(uint a1_ind, uint a2_ind) {

    uint k = a1_ind;
    uint m = a2_ind;

    /* Sum over classes */
    double zsum = 0.0;
    for (uint j = 0; j < td.vmax[0]; j++) {

        /* sum over second attribute */
        double ysum = 0.0;
        for (uint l = 0; l < td.vmax[k]; l++) {

            /* sum over third attribute */
            double xsum = 0.0;
            for (uint n = 0; n < td.vmax[m]; n++) {

                /* Evaluate probabilities */
                double P_xyz = (double) ptable[j][k][l][m][n] / (double) ptable[0][0][0][0][0]; // Compute P(x,y,z))
                double P_xy_z = (double) ptable[j][k][l][m][n] / (double) ptable[j][0][0][0][0]; // Compute P(x,y|z)
                double P_x_z = (double) ptable[j][m][n][0][0] / (double) ptable[j][0][0][0][0]; // Compute P(x|z) 
                double P_y_z = (double) ptable[j][k][l][0][0] / (double) ptable[j][0][0][0][0]; // Compute P(y|z)
                
                xsum += P_xyz * log2(P_xy_z/(P_x_z * P_y_z));
                
            }
            ysum += xsum;

        }
        zsum += ysum;

    }
    
    return zsum;

}