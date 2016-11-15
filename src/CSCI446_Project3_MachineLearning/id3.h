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

class ID3 : public Learner {
public:
    ID3(Dataset train_data);
    void learn();
    uint answer(datum attrs);
    private:
    vector<float> compute_gain(Dataset set);
};

#endif /* ID3_H */

