
/* 
 * File:   teacher.cpp
 * Author: byrdie
 *
 * Created on November 19, 2016, 5:58 PM
 */

#include "teacher.h"

Teacher::Teacher(Learner * learner, Dataset& train, Dataset& test) {

    td = train;
    vd = test;
    pupil = learner;

    teach();

}

void Teacher::teach() {

    pupil->learn();
    vd.discretize();
    uint correct = 0;
    uint sz = vd.data.size();
    //    uint sz = 1;
#if sample_runs
    out << "_______________________________________________\n";
    out << "TESTING PHASE\n";
#endif
    for (uint i = 0; i < sz; i++) {

        uint ans = pupil->answer(vd.data[i]);
#if sample_runs
        out << "The predicted class was: ";
        out << ans;
        //        vd.print_val(0, ans);
        out << ", which is ";
#endif
        if (ans == vd.data[i][0]) {
#if sample_runs
            out << "correct";
#endif
            correct++;
        }
#if sample_runs
        else {
            out << "incorrect";
        }
        out << "\n";
#endif
    }

    precision = (double) correct / sz;

    out << "Precision: " << correct << "/" << sz << " = " << precision << "\n";

}