/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cancer.h
 * Author: byrdie
 *
 * Created on October 30, 2016, 4:31 PM
 */

#ifndef CANCER_H
#define CANCER_H

#include "system.h"
#include "data.h"

#define CANCER_DATA_TYPE "Wisconsin Breast Cancer"
#define CANCER_DATA_DIR "../../data/cancer/"

class CancerDataset : public Dataset {
public:
    CancerDataset();  
private:
    void read_data();
};


#endif /* CANCER_H */

