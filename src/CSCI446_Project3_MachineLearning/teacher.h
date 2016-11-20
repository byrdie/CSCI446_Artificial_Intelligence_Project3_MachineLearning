/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   teacher.h
 * Author: byrdie
 *
 * Created on November 19, 2016, 5:58 PM
 */

#ifndef TEACHER_H
#define TEACHER_H

class Teacher {
    
    double precision;
    double recall;
    Dataset td;
    Dataset vd;
    Teacher(Dataset train, Dataset validate);
    
};

#endif /* TEACHER_H */

