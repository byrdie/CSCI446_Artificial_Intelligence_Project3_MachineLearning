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
class ID3 : public Learner {
public:
    vector<int> num_var_types;
    ID3(Dataset train_data);
    float m_entropy;
    void learn();
    uint answer(datum attrs);
    private:
    vector<float> compute_var_gain(Dataset set, int var);
    float compute_class_gain(Dataset set, int d_class, int var);
    float master_entropy();
    vector<int> num_vars();
};

#endif /* ID3_H */

