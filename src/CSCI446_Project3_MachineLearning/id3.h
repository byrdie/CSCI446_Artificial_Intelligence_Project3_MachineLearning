/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   id3.h
 * Author: girish
 *
 * Created on November 14, 2016, 1:27 PM
 */

#ifndef ID3_H
#define ID3_H

#include "learn.h"
#include <math.h>
#include "graph.h"
class ID3 : public Learner {
public:
    vector<int> num_var_types;
    ID3(Dataset train_data);
    float m_entropy;
    void learn();
    uint answer(datum attrs);
    private:
    vector<float> compute_var_gain(Dataset set);
    float compute_class_gain(Dataset set, int d_class, int var);
    float master_entropy();
    vector<int> num_vars();
    bool same_class(Dataset set);
    int max_gain(vector<float> gains, Dataset data);
    Graph<vector<uint>> id3(Dataset set, Dataset parent);
    int plurality_value(Dataset set);
    bool attributes_empty(Dataset set);
};

#endif /* ID3_H */

