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

#define val_ratio 3

class ID3 : public Learner {
public:
    Graph<vector<uint>> tree; //decision tree
    vector<uint> num_var_types; //keep track of how many different values each variable can represent
    float m_entropy; //total entropy of dataset
    uint viz_count; //increments to improve tree drawing
    Dataset val_d;
    bool will_prune;

    ID3(Dataset train_data, vector<uint> var_types, bool will_prune_1);
    void learn();
    uint answer(datum attrs);
    uint answer_val(datum attrs);
    uint answer_p(datum attrs);
    uint answer_np(datum attrs);
    void make_val_set();
    void set_num_var_types(vector<uint> num);

private:
    vector<float> compute_var_gain(Dataset set);
    float compute_class_gain(Dataset set, int d_class, int var);
    float master_entropy();
    vector<uint> num_vars();
    bool same_class(Dataset set);
    int max_gain(vector<float> gains, Dataset data);
    Vert<vector<uint>>*id3(Dataset set, Dataset parent);
    int plurality_value(Dataset set);
    bool attributes_empty(Dataset set);
    void prune();
    uint validate();
   
};


#endif /* ID3_H */

