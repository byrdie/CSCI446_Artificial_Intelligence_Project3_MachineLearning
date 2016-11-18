/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: byrdie
 *
 * Created on October 26, 2016, 9:31 PM
 */



#include "main.h"

record out("test.txt");

int main(int argc, char *argv[]) {

    init_rand();

//            CancerDataset id;
//            GlassDataset id;
//    IrisDataset id;
            SoybeanDataset id;
    //    VoteDataset id;

    vector<Dataset> folds = id.rand_split(2);
    Dataset td = folds[0];
    Dataset vd = folds[1];

    NearestNeighbor nb(td);
    test_learner(nb, vd);

//    //    test_nb();
//    test_tan();


}

void test_learner(Learner& nb, Dataset& vd) {

    nb.learn();
    vd.discretize();
    uint correct = 0;
    uint sz = 20;
    out << "_______________________________________________\n";
    out << "TESTING PHASE\n";
    for (uint i = 0; i < sz; i++) {

        uint ans = nb.answer(vd.data[i]);

        out << "The predicted class was: ";
        out << ans;
//        vd.print_val(0, ans);
        out << ", which is ";
        if (ans == vd.data[i][0]) {
            out << "correct";
            correct++;
        } else {
            out << "incorrect";
        }
        out << "\n";
    }
    out << "_______________________________________________\n";

    cout << "\nThe accuracy of these " << sz << " trials was: " << (double) correct / (double) sz << "\n";
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
