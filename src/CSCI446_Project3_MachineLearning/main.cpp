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

    test_nb();
    //    test_tan();


}

//void test_nb() {
//        CancerDataset id;
////        GlassDataset id;
////        IrisDataset id;
//    //    SoybeanDataset id;
////    VoteDataset id;
//
//    vector<Dataset> folds = id.rand_split(2);
//    Dataset td = folds[0];
//    Dataset vd = folds[1];
//
//    NaiveBayes nb(td);
//    nb.learn();
//    vd.discretize();
//    uint correct = 0;
//    uint sz = vd.data.size();
//    for (uint i = 0; i < sz; i++) {
//        vd.print_datum(true, i);
//        uint ans = nb.answer(vd.data[i]);
//        cout << endl << "The predicted class was: ";
//        vd.print_class(ans);
//        cout << "____________________________________" << endl;
//        
//        if(ans == vd.data[i][0]){
//            correct++;
//        }
//    }
//    
//    cout << "Accuracy: " << (double) correct / (double) sz;
//}

void test_nb() {
    //        CancerDataset id;
    //        GlassDataset id;
    IrisDataset id;
    //        SoybeanDataset id;
    //    VoteDataset id;

    vector<Dataset> folds = id.rand_split(2);
    Dataset td = folds[0];
    Dataset vd = folds[1];

    NaiveBayes nb(td);
    nb.learn();
    vd.discretize();
    uint correct = 0;
    uint sz = 3;
    out << "_______________________________________________\n";
    out << "TESTING PHASE\n";
    for (uint i = 0; i < sz; i++) {
        //        vd.print_datum(true, i);
        uint ans = nb.answer(vd.data[i]);
        //        cout << endl << "The predicted class was: ";
        //        vd.print_class(ans);
        //        cout << "____________________________________" << endl;

        out << "The predicted class was: ";
        td.print_val(0, ans);
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

void test_tan() {


    CancerDataset id;
    //        GlassDataset id;
    //        IrisDataset id;
    //        SoybeanDataset id;
    //    VoteDataset id;

    vector<Dataset> folds = id.rand_split(2);
    Dataset td = folds[0];
    Dataset vd = folds[1];

    TAN tan(td);
    tan.learn();
    vd.discretize();
    uint correct = 0;
    uint sz = vd.data.size();
    for (uint i = 0; i < sz; i++) {
        vd.print_datum(true, i);
        uint ans = tan.answer(vd.data[i]);
        cout << endl << "The predicted class was: ";
        //        vd.print_class(ans);
        cout << "____________________________________" << endl;

        if (ans == vd.data[i][0]) {
            correct++;
        }
    }

    cout << "Accuracy: " << (double) correct / (double) sz;
}

void test_graph() {
    //        CancerDataset id;
    //        GlassDataset id;
    //        IrisDataset id;
    //    SoybeanDataset id;
    VoteDataset id;
    datum attrs = id.data[0];
    Graph<uint> g;
    Vert<uint> * root = g.add_vert(id.attr_names.left.find(0)->second, 0);
    for (uint i = 1; i < attrs.size(); i++) {
        Vert<uint> * next_c = g.add_vert(id.attr_names.left.find(i)->second, 0);
        g.add_edge(rand() % 127, root, next_c, 0);
    }

    g.print_gviz("", "test");
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
