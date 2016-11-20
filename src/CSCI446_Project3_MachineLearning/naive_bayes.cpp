
/* 
 * File:   naive_bayes.cpp
 * Author: byrdie
 *
 * Created on November 13, 2016, 9:19 PM
 */

#include "naive_bayes.h"

NaiveBayes::NaiveBayes(Dataset train_data) : Learner(train_data) {

    sname = "NaiveBayes";
    
    /* discretize the training set before starting */
    td.discretize();

    /* We need to initialize a non-rectangular 3D array. Therefore we must loop 
     and build the vector from the inside out.*/
    for (uint j = 0; j <= td.vmax[0]; j++) { // Loop over the class values

        datum attrs = td.data[0]; // just use the zeroth datum as representative example

        /* loop over all attributes */
        vector<vector < uint>> ptable_row;
        for (uint k = 0; k < attrs.size(); k++) {

            /* loop over every value for each attribute */
            vector<uint> attr_vals;
            for (uint l = 0; l <= td.vmax[k]; l++) {
                attr_vals.push_back(0); // Initialize to zero
            }
            ptable_row.push_back(attr_vals);
        }
        ptable.push_back(ptable_row);
    }

}

void NaiveBayes::learn() {

    count();

}

void NaiveBayes::count() {
    /* Loop through every datum in the dataset */
    for (uint i = 0; i < td.data.size(); i++) {

        datum attrs = td.data[i];
        uint j = attrs[0]; // The class label is the first index

        ptable[j][0][0]++; // Increment the prior probability
        ptable[0][0][0]++; // Increment the total

        /* Loop through the attributes */
        for (uint k = 1; k < attrs.size(); k++) { // The location of the attribute is the second index

            uint l = attrs[k]; // The value of the attribute is the third index

            ptable[j][k][l]++; // Increment the likelihood
            ptable[0][k][l]++; // Increment the evidence           

        }
    }

    out << "_______________________________________________\n";
    out << "TRAINING PHASE\n";
    out << "_______________________________________________\n";
    out << "We will only print the distributions of the prior and the evidence since the table is too large\n";
    for (uint i = 0; i < ptable[0].size(); i++) {
        td.print_attr(i);
        cout << ": ";
        for (uint j = 1; j <= td.vmax[i]; j++) {
            if (i == 0) {
                out << "P(";
                td.print_val(i, j);
                out << ") = " << (double) ptable[j][0][0] / (double) ptable[0][0][0] << ", ";
            } else {
                out << "P(";
                td.print_val(i, j);
                out << ") = " << (double) ptable[0][i][j] / (double) ptable[0][0][0] << ", ";
            }

        }
        out << "\n";
    }
}

uint NaiveBayes::answer(datum attrs) {

    vector<double> pd; // Probability distribution for each class
    
    out << "_______________________________________________\n";
    
    out << "The vector to check is given by \n";
    td.print_datum(attrs);
    out << "\n \n";

    /* print out equation */
    out << "Calculate the most probable class using the distribution\n";
    out << "P(";
    td.print_attr(0);
    out << "|X) = P(";
    td.print_attr(0);
    out << ")";

    for (uint k = 1; k < attrs.size(); k++) {
        out << "P(";
        td.print_attr(k);
        out << "|";
        td.print_attr(0);
        out << ") ";
    }
    out << "\n \n";

    /* Loop over the class values */
    for (uint j = 1; j <= td.vmax[0]; j++) {

        /* The prior probability is the tally of the number instances of a class in the training dataset
         * divided by the total number of items in the training dataset */
        double P_C = ((double) ptable[j][0][0]) / ((double) ptable[0][0][0]);
        double P_x_C = 1.0;
        double P_x = 1.0;


        out << "P(";
        td.print_val(0, j);
        out << "|X) = P(";
        td.print_val(0, j);
        out << ") ";

        for (uint k = 1; k < attrs.size(); k++) {
            out << "P(";
            td.print_val(k,attrs[k]);
            out << "|";
            td.print_val(0,j);
            out << ") ";
        }
        
        out << "\n";

        out << "P(";
        td.print_val(0, j);
        out << "|X) = (" << P_C << ")";

        for (uint k = 1; k < attrs.size(); k++) {

            uint l = attrs[k];
            double likelihood = laplace_smooth(ptable[j][k][l], ptable[j][0][0]);
            double evidence = ((double) ptable[0][k][l]) / ((double) ptable[0][0][0]);

            out << "(" << likelihood << ")";

            P_x_C *= likelihood;
            P_x *= evidence;
        }

        out << "\n";

        /* calculate probability distribution for this class*/
        double val = P_C * P_x_C;
        pd.push_back(val);
        
        out << "P(";
        td.print_val(0, j);
        out << "|X) = " << val << "\n \n";
    }

    /* return the class with the highest probability */
    return distance(pd.begin(), max_element(pd.begin(), pd.end())) + 1;

}

double NaiveBayes::laplace_smooth(uint x, uint N) {

    double m = 1.0;
    double p = 1e-6;

    return (x + m * p) / (N + m);

}