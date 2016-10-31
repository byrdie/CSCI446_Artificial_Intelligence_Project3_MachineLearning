/* 
 * File:   data.h
 * Author: byrdie
 *
 * Created on October 26, 2016, 9:32 PM
 */

#ifndef DATA_H
#define DATA_H

#include "system.h"

#define MISSING 0

class Dataset {
public:
    string dataset_type;
    string dir;
    str_map attr_names;
    vector<str_map> val_names;

    /* Storage for the dataset, first element is the class */
    vector<vector<uint>> data;

    Dataset(string type, string directory);
    void init_bimaps();
    void print_dataset();
private:
};


#endif /* DATA_H */

