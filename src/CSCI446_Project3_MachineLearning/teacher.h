
/* 
 * File:   teacher.h
 * Author: byrdie
 *
 * Created on November 19, 2016, 5:58 PM
 */

#ifndef TEACHER_H
#define TEACHER_H
#include "system.h"
#include "learn.h"

class Teacher {
public: 
    double precision;
    double recall;
    Dataset td;
    Dataset vd;
    Learner * pupil;
    Teacher(Learner * learner, Dataset& train, Dataset& test);
    void teach();
    
};

#endif /* TEACHER_H */

