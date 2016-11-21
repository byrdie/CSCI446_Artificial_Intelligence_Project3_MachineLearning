
/* 
 * File:   tan.cpp
 * Author: byrdie
 *
 * Created on November 16, 2016, 4:57 PM
 */

#include "tan.h"
#include "graph.h"

TAN::TAN(Dataset train_data) : NaiveBayes(train_data) {

    sname = "TAN";

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
            vector<vector < vector < uint>>> attr_vals1;
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

#if sample_run

    out << "_______________________________________________\n";
    out << "TRAINING PHASE\n";
    out << "_______________________________________________\n";
    out << "We will only print the distributions of the prior and the evidence since the table is too large\n";
    for (uint i = 0; i < ptable[0].size(); i++) {
        out << "     ";
        td.print_attr(i);
        out << ": ";
        for (uint j = 1; j <= td.vmax[i]; j++) {
            if (i == 0) {
                out << "P(";
                td.print_val(i, j);
                out << ") = " << (double) ptable[j][0][0][0][0] / (double) ptable[0][0][0][0][0] << ", ";
            } else {
                out << "P(";
                td.print_val(i, j);
                out << ") = " << (double) ptable[0][i][j][0][0] / (double) ptable[0][0][0][0][0] << ", ";
            }

        }
        out << "\n";
    }
#endif

    /* Construct complete graph out of attributes */
#if sample_run
    out << "\nConstruct a complete graph out of the vertices:";
#endif
    Graph<uint> g;
    datum attrs = td.data[0];
    for (uint i = 1; i < attrs.size(); i++) {
#if sample_run
        out << "[";
        td.print_attr(i);
        out << "], ";
#endif
        g.add_vert(td.attr_names.left.find(i)->second, i);
    }
#if sample_run
    out << "and use the conditional, mutual information function to calculate the weights.\n";
#endif
    for (uint i = 0; i < g.verts.size(); i++) {
        for (uint j = 0; j < i; j++) {

            /* Don't connect vertices to themselves */
            Vert<uint> * v1 = g.verts[i];
            Vert<uint> * v2 = g.verts[j];
            double weight = cmi(v1->val, v2->val);

            g.add_edge(weight, v1, v2, 0);
        }
    }

#if sample_run
    g.print_text();
    out << "\nExecute Kruskal's algoritm to find the maximum spanning tree:\n";
#endif
    mst = kruskal(&g);

    /* Transform into directed graph */
    mst->direct(mst->verts[0]);
#if sample_run
    out << "Add directions to the MST\n";
    mst->print_text();
    mst->print_gviz("../output/TAN/", td.sname);
#endif
}

uint TAN::answer(datum attrs) {

    vector<double> pd; // Probability distribution for each class
#if sample_run
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

        /* Find the parent of this attribute in the MST */
        vector<Vert < uint>*> parents = mst->find_parents(mst->find_vert(k));
        if (parents.size() == 1) { // Check to make sure there is only one parent
            uint m = parents[0]->val;
            out << "P(";
            td.print_attr(k);
            out << "|";
            td.print_attr(m);
            out << ",";
            td.print_attr(0);
            out << ") ";
        } else if (parents.size() == 0) { // Root node
            out << "P(";
            td.print_attr(k);
            out << "|";
            td.print_attr(0);
            out << ") ";
        }
    }
    out << "\n \n";
#endif
    /* Loop over the class values */
    for (uint j = 1; j <= td.vmax[0]; j++) { // Compute argmax_C P(C) P(xr|C) PI_x P(x|y,C)

        double P_C = laplace_smooth(ptable[j][0][0][0][0], ptable[0][0][0][0][0]); // compute P(C), where C is the class variable
        double P_x_yC = 1.0;
        double P_x = 1.0;
#if sample_run
        out << "P(";
        td.print_val(0, j);
        out << "|X) = P(";
        td.print_val(0, j);
        out << ") ";

        for (uint k = 1; k < attrs.size(); k++) {
            vector<Vert < uint>*> parents = mst->find_parents(mst->find_vert(k));
            if (parents.size() == 1) { // Check to make sure there is only one parent
                uint m = parents[0]->val;
                out << "P(";
                td.print_val(k, attrs[k]);
                out << "|";
                td.print_val(m, attrs[m]);
                out << ",";
                td.print_val(0, j);
                out << ") ";
            } else if (parents.size() == 0) { // Root node
                out << "P(";
                td.print_val(k, attrs[k]);
                out << "|";
                td.print_val(0, j);
                out << ") ";
            }
        }
        out << "\n";

        out << "P(";
        td.print_val(0, j);
        out << "|X) = (" << P_C << ")";
#endif
        /* Loop over the attributes in attrs */
        for (uint k = 1; k < attrs.size(); k++) {
            uint l = attrs[k];

            /* Find the parent of this attribute in the MST */
            vector<Vert < uint>*> parents = mst->find_parents(mst->find_vert(k));
            if (parents.size() == 1) { // Check to make sure there is only one parent

                uint m = parents[0]->val;
                uint n = attrs[m];
                double likelihood = laplace_smooth(ptable[j][k][l][m][n], ptable[j][k][l][0][0]); // Compute P(x|y,C)
                double evidence = laplace_smooth(ptable[0][k][l][0][0], ptable[0][0][0][0][0]); // Compute P(x)
#if sample_run
                out << "(" << likelihood << ")";
#endif

                P_x_yC *= likelihood;
                P_x *= evidence;

            } else if (parents.size() == 0) { // Root node

                double likelihood = laplace_smooth(ptable[j][k][l][0][0], ptable[j][0][0][0][0]); // Compute P(x|C)
                double evidence = laplace_smooth(ptable[0][k][l][0][0], ptable[0][0][0][0][0]); // Compute P(x)
#if sample_run
                out << "(" << likelihood << ")";
#endif

                P_x_yC *= likelihood;
                P_x *= evidence;

            } else { // More than one parent somehow
                cout << "Incorrect MST" << endl;
                return 0;
            }


        }



        double val = P_C * P_x_yC / P_x; // compute probability for this class
        pd.push_back(val);
#if sample_run
        out << "\n";
        out << "P(";
        td.print_val(0, j);
        out << "|X) = " << val << "\n \n";
#endif
    }

    /* return the class with the highest probability */
    return distance(pd.begin(), max_element(pd.begin(), pd.end())) + 1;

}

double TAN::cmi(uint a1_ind, uint a2_ind) {

    uint k = a1_ind;
    uint m = a2_ind;

    /* Sum over classes */
    double zsum = 0.0;
    for (uint j = 0; j <= td.vmax[0]; j++) {

        /* sum over second attribute */
        double ysum = 0.0;
        for (uint l = 0; l <= td.vmax[k]; l++) {

            /* sum over third attribute */
            double xsum = 0.0;
            for (uint n = 0; n <= td.vmax[m]; n++) {

                /* Evaluate probabilities */
                double p = 1e-6;
                double r = 1.0;
                double P_xyz = ((double) ptable[j][k][l][m][n] + r * p) / ((double) ptable[0][0][0][0][0] + r); // Compute P(x,y,z))
                double P_xy_z = ((double) ptable[j][k][l][m][n] + r * p) / ((double) ptable[j][0][0][0][0] + r); // Compute P(x,y|z)
                double P_x_z = ((double) ptable[j][m][n][0][0] + r * p) / ((double) ptable[j][0][0][0][0] + r); // Compute P(x|z) 
                double P_y_z = ((double) ptable[j][k][l][0][0] + r * p) / ((double) ptable[j][0][0][0][0] + r); // Compute P(y|z)

                xsum += P_xyz * log2(P_xy_z / (P_x_z * P_y_z));


            }
            ysum += xsum;

        }
        zsum += ysum;

    }
    return zsum;

}

Graph<uint> * TAN::kruskal(Graph<uint>* cg) {

    /* Create a space for the MST */
    Graph<uint> * mst = new Graph<uint>();

    /* Move all vertices from the input into the mst */
    for (uint i = 0; i < cg->verts.size(); i++) {
        Vert<uint> * v = cg->verts[i];
        v->edges.clear();
        mst->verts.push_back(v);
    }

    /* start by sorting the edges */
    sort(cg->edges.begin(), cg->edges.end(), cmp_edges);
#if sample_run
    out << "Start by sorting the edges by increasing weight\n";
    cg->print_text();
    out << "Next, build the MST by selecting the heighest weight, non-cyclic edges\n";
#endif

    /* Loop until we have a tree (number of vertices - 1)*/
    while (true) {

        /* Select a new edge from the input */
        Edge<uint> * cg_e = cg->edges.back();
        cg->edges.pop_back();

        /* Add edge into MST */
        Edge<uint> * e = mst->add_edge(cg_e->w, cg_e->verts[0], cg_e->verts[1], cg_e->direction);
#if sample_run
        out << "Selected new edge with weight: " << e->w << " => ";
#endif

        /* check for loops */
        if (mst->loop_exists()) {
#if sample_run
            out << "Loop detected, deleting edge\n";
#endif
            mst->remove_edge(e);
        } else {
#if sample_run
            out << "No loop detected, adding edge to MST\n";
#endif
        }

        /* Check if we have made a tree */
        if ((mst->verts.size() - 1) == mst->edges.size()) {
#if sample_run
            out << "All vertices connected, the final MST is:\n";
            mst->print_text();
#endif
            break;
        }
    }



    return mst;

}