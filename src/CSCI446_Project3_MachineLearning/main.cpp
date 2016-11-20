
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

//            CancerDataset id;
//            GlassDataset id;
//    IrisDataset id;
////            SoybeanDataset id;
////        VoteDataset id;
//
//    vector<Dataset> folds = id.rand_split(2);
//    Dataset td = folds[0];
//    Dataset vd = folds[1];
//
////    TAN nb(td);
////    NaiveBayes nb(td);
//    NearestNeighbor nb(td);
//    test_learner(nb, vd);
//
////    //    test_nb();
////    test_tan();
    test_id3();


}

void tune_k_and_p() {

    uint qfolds = 5;
    uint kfolds = 2;

    uint kmin = 1;
    uint kmax = 3;
    uint pmin = 1;
    uint pmax = 3;

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
            for (uint p = pmin; p < pmax; k++) {

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
    //CancerDataset id;
    //GlassDataset id;
    //IrisDataset id;
    //SoybeanDataset id;
    VoteDataset id;
    id.discretize();
    vector<Dataset> folds = id.rand_split(2);
    Dataset td = folds[0];
    Dataset vd = folds[1];



    ID3 id3(td);
    out << "\n\n";
    out << "____________________________________________________________________" << "\n";
    out << "Tree_building: All the variables that were actually used and their respective gain" << "\n";
    id3.learn();
    uint correct = 0;
    uint sz = vd.data.size();
    out << "\n\n";
    out << "____________________________________________________________________" << "\n";
    out << "Results:" << "\n";
    for (uint i = 0; i < sz; i++) {

        uint ans = id3.answer(vd.data[i]);
        vd.print_datum(true, i);

        if (vd.data[i][0] == ans) {
            correct++;
        }

        out << "____________________________________" << "\n";
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
