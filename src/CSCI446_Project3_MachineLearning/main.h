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

void test_nb();
void test_tan();
void test_kNN();
void test_graph();

/* repeatable random number generation */
void init_rand(unsigned long int seed);
unsigned long int init_rand();

#endif /* MAIN_H */

