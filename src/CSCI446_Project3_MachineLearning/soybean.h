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

#ifndef SOYBEAN_H
#define SOYBEAN_H

#include "system.h"
#include "data.h"

#define SOYBEAN_DATA_TYPE "Small Soybean Database"
#define SOYBEAN_DATA_DIR "../../data/soybean/"

class SoybeanDataset : public Dataset {
public:
    SoybeanDataset();  
private:
    void read_data();
};


#endif /* SOYBEAN_H */