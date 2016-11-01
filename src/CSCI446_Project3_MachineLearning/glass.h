/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   glass.h
 * Author: byrdie
 *
 * Created on October 30, 2016, 4:31 PM
 */

#ifndef GLASS_H
#define GLASS_H

#include "system.h"
#include "data.h"

#define GLASS_DATA_TYPE "Glass Identification database"
#define GLASS_DATA_DIR "../../data/glass/"

class GlassDataset : public Dataset {
public:
    GlassDataset();  
private:
    void read_data();
};

#endif /* GLASS_H */

