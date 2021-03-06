/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.h
 * Author: byrdie
 *
 * Created on October 26, 2016, 9:31 PM
 */

#ifndef MAIN_H
#define MAIN_H

#include <QApplication>


#include "record.h"
#include "cancer.h"
#include "glass.h"
#include "iris.h"
#include "vote.h"
#include "naive_bayes.h"
#include "soybean.h"
#include "tan.h"
#include "id3.h"
#include "nearest_neighbor.h"
#include "graph.h"
#include "teacher.h"

void tune_k_and_p();
void find_convergence();

void test_id3();


/* repeatable random number generation */
void init_rand(unsigned long int seed);
unsigned long int init_rand();
vector<uint> num_vars(Dataset td);
#endif /* MAIN_H */

