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
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    //    QApplication app(argc, argv);

    init_rand();

    VoteDataset id;
    vector<Dataset> folds = id.rand_split(2);
    Dataset td = folds[0];
    Dataset vd = folds[1];

    //    cout << "training dataset" << endl;
    //    td.print_dataset(true);
    //    cout << endl;
    //    cout << "validation dataset" << endl;
    //    vd.print_dataset(true);

    //    id.print_datum(true,100);

    NaiveBayes nb(td);
    nb.learn();
    vd.discretize();
    uint correct = 0;
    uint sz = vd.data.size();
    for (uint i = 0; i < sz; i++) {
        vd.print_datum(true, i);
        uint ans = nb.answer(vd.data[i]);
        cout << endl << "The predicted class was: ";
        vd.print_class(ans);
        cout << "____________________________________" << endl;
        
        if(ans == vd.data[i][0]){
            correct++;
        }   
    }
    
    float acc = (float) correct / (float) sz;
    cout << "The accuracy was: " << acc;



    //    return app.exec();
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