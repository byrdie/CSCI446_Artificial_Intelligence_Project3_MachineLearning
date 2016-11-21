
/*
 * File:   main.cpp
 * Author: byrdie
 *
 * Created on October 26, 2016, 9:31 PM
 */



#include "main.h"

record out("../output/test.txt");

int main(int argc, char *argv[]) {

    init_rand();
    //init_rand();

//                        CancerDataset id;
//                        GlassDataset id;
//                IrisDataset id;
//        SoybeanDataset id;
            VoteDataset id;
    
        //    vector<Dataset> folds = id.get_strat_fold(2);
        //    Dataset td = folds[0];
        //    Dataset vd = folds[1];
    
//        vector<pair<Dataset, Dataset>> cv_ds = folds_to_dsets(id.get_strat_fold(5));
//        Dataset td = cv_ds[0].second;
//        Dataset vd = cv_ds[0].first;
    
        //    TAN nb(td);
            NaiveBayes nb(id);
//        NearestNeighbor nb(td, 2, 5);
        //    ID3 nb(td);
        Teacher t(&nb, id, id);

    //    //    test_nb();
    //    test_tan();

    //    tune_k_and_p();

    find_convergence();
    //test_id3();
}

void find_convergence() {

    uint qfolds = 5;
    uint kfolds = 2;

    uint k_knn = 2;
    uint p_knn = 3;

    //    uint conv_resolution = 20;
    uint s_sz = 1;

    vector<Dataset> dsets;
    vector<vector<uint>>    id3_sizes;

    CancerDataset cancer;
    GlassDataset glass;
    IrisDataset iris;
    SoybeanDataset soybean;
    VoteDataset vote;

    dsets.push_back(cancer);
    dsets.push_back(glass);
    dsets.push_back(iris);
    dsets.push_back(soybean);
    dsets.push_back(vote);
    
    id3_sizes.push_back(num_vars(cancer));
    id3_sizes.push_back(num_vars(glass));
    id3_sizes.push_back(num_vars(iris));
    id3_sizes.push_back(num_vars(soybean));
    id3_sizes.push_back(num_vars(vote));

    /* Loop through each dataset */
    for (uint i = 0; i < dsets.size(); i++) {

        /* open data files */
        ofstream precis_kNN;
        ofstream precis_NB;
        ofstream precis_TAN;
        ofstream precis_ID3;

        precis_kNN.open("../results/kNN/test_convergence." + dsets[i].sname + ".precis.dat");
        precis_NB.open("../results/NaiveBayes/test_convergence." + dsets[i].sname + ".precis.dat");
        precis_TAN.open("../results/TAN/test_convergence." + dsets[i].sname + ".precis.dat");
        precis_ID3.open("../results/ID3/test_convergence." + dsets[i].sname + ".precis.dat");

        vector<pair<uint, double>> plist_kNN(dsets[i].data.size() / s_sz);
        vector<pair<uint, double>> plist_NB(dsets[i].data.size() / s_sz);
        vector<pair<uint, double>> plist_TAN(dsets[i].data.size() / s_sz);
        vector<pair<uint, double>> plist_ID3(dsets[i].data.size() / s_sz);



        /* loop through for each cross validation*/
        for (uint j = 0; j < qfolds; j++) {

            /* generate the cross validations */
            vector<pair<Dataset, Dataset>> cv_ds = folds_to_dsets(dsets[i].get_strat_fold(kfolds));


            /* loop through this cross validation */
            for (uint l = 0; l < cv_ds.size(); l++) {

                /* select the datasets from the list */
                Dataset vd = cv_ds[l].first;
                Dataset td = cv_ds[l].second;

                /* make a copy of the data for convergence tests */
                vector<vector < uint>> cdata = td.data;
                uint conv_resolution = cdata.size() / s_sz;
                //                uint s_sz = cdata.size() / conv_resolution;
                for (uint m = 2; m < conv_resolution; m++) {


                    uint data_sz = m * s_sz;
                    cout << "Dataset: " << dsets[i].sname << ", Training Set Size: " << data_sz << "\n";
                    vector<vector < uint >> tdata(cdata.begin(), cdata.begin() + data_sz);
                    td.data = tdata;

                    NearestNeighbor L_kNN(td, k_knn, p_knn);
                    NaiveBayes L_NB(td);
                    TAN L_TAN(td);
                    
                    ID3 L_ID3(td, id3_sizes[i], true);
                    
                    
                    //                    ID3 L_ID3(td);
                    cout << vd.data.size() << endl;

                    Teacher T_kNN(&L_kNN, td, vd);
                    Teacher T_NB(&L_NB, td, vd);
                    Teacher T_TAN(&L_TAN, td, vd);
                    Teacher T_ID3(&L_ID3, td, vd);
                    
                    //                    Teacher T_ID3(&L_ID3, td, vd);

                    plist_kNN[m].second += T_kNN.precision;
                    plist_kNN[m].first = data_sz;
                    plist_NB[m].second += T_NB.precision;
                    plist_NB[m].first = data_sz;
                    plist_TAN[m].second += T_TAN.precision;
                    plist_TAN[m].first = data_sz;
                    plist_ID3[m].second += T_ID3.precision;
                    plist_ID3[m].first = data_sz;

                }
            }

        }

        for (uint m = 2; m < plist_kNN.size(); m++) {

            if (plist_kNN[m].second != 0) {
                precis_kNN << plist_kNN[m].first << " " << plist_kNN[m].second / (qfolds * kfolds) << "\n";
                precis_NB << plist_NB[m].first << " " << plist_NB[m].second / (qfolds * kfolds) << "\n";
                precis_TAN << plist_TAN[m].first << " " << plist_TAN[m].second / (qfolds * kfolds) << "\n";
                precis_ID3 << plist_ID3[m].first << " " << plist_ID3[m].second / (qfolds * kfolds) << "\n";
            }


        }

        precis_kNN.close();
        precis_NB.close();
        precis_TAN.close();
        precis_ID3.close();

    }

}

void tune_k_and_p() {

    uint qfolds = 1;
    uint kfolds = 10;

    uint kmin = 1;
    uint kmax = 20;
    uint pmin = 1;
    uint pmax = 10;

    vector<Dataset> dsets;

    CancerDataset cancer;
    GlassDataset glass;
    IrisDataset iris;
    SoybeanDataset soybean;
    VoteDataset vote;

    dsets.push_back(cancer);
    dsets.push_back(glass);
    dsets.push_back(iris);
    dsets.push_back(soybean);
    dsets.push_back(vote);



    /* Loop through each dataset */
    for (uint i = 0; i < dsets.size(); i++) {

        /* open data files */
        ofstream precis_fp;
        //                ofstream recall_fp;
        precis_fp.open("../results/kNN/tune_k_and_p." + dsets[i].sname + ".precis.dat");
        //                recall_fp.open("../results/kNN/tune_k_and_p." + dsets[i].sname + ".recall.dat");

        /* loop through k */
        for (uint k = kmin; k < kmax; k++) {

            /* loop through p */
            for (uint p = pmin; p < pmax; p++) {

                out << "k = " << k << ", p = " << p << "\n";

                /* loop through for each cross validation*/
                for (uint j = 0; j < qfolds; j++) {

                    /* generate the cross validations */
                    vector<pair<Dataset, Dataset>> cv_ds = folds_to_dsets(dsets[i].get_strat_fold(kfolds));

                    /* loop through this cross validation */
                    for (uint l = 0; l < cv_ds.size(); l++) {

                        /* select the datasets from the list */
                        Dataset vd = cv_ds[l].first;
                        Dataset td = cv_ds[l].second;

                        NearestNeighbor nb(td, k, p);
                        Teacher t(&nb, td, vd);

                        precis_fp << k << " " << p << " " << t.precision << "\n";

                    }


                }

            }

        }
        precis_fp.close();

    }

}

void test_id3() {
    CancerDataset id;
   // GlassDataset id;
    //IrisDataset id;
    //SoybeanDataset id;
    //VoteDataset id;
    id.discretize();
    vector<Dataset> folds = id.rand_split(2);
    Dataset td = folds[0];
    Dataset vd = folds[1];



    ID3 id3(td, num_vars(id), true);
    //id3.set_num_var_types(num_vars(id));
    

    out << "\n\n";
    out << "____________________________________________________________________" << "\n";
    out << "Tree_building: All the variables that were actually used and their respective gain" << "\n";
    id3.learn();
    uint correct = 0;
    uint sz = vd.data.size();
    out << "\n\n";
    out << "____________________________________________________________________" << "\n";
    out << "Results:" << "\n";
    //id3.tree.print_gviz("../output/ID3", "test");
    for (uint i = 0; i < sz; i++) {
        vd.print_datum(true, i);
        out << "\n\n";
        uint ans = id3.answer(vd.data[i]);
        
        //vd.print_datum(true, i);

        if (vd.data[i][0] == ans) {
            correct++;
        }

        out << "\n____________________________________" << "\n";
    }
    cout << "ratio: " << correct << "/" << sz << endl;
    id3.tree.print_gviz("../output/ID3", "test");

}

/* Prepare random number generation */
void init_rand(unsigned long int seed) {
    srand(seed);
    printf("Seed: %lu\n", seed);
}

unsigned long int init_rand() {
    unsigned int seed = time(NULL);
    srand(seed);
    printf("Seed: %u\n", seed);
    return seed;
}
vector<uint> num_vars(Dataset td) {
    /*finds number of discrete values for each variable*/
    vector<uint> max_var;
    for (uint j = 0; j < td.data[0].size(); j++) {
        uint instances = 0;
        for (uint i = 0; i < td.data.size(); i++) {
            if (td.data[i][j] > instances) {
                instances = td.data[i][j];
            }
        }
        max_var.push_back(instances);
    }
    return max_var;
}
