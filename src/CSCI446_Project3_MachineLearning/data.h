/* 
 * File:   data.h
 * Author: byrdie
 *
 * Created on October 26, 2016, 9:32 PM
 */

#ifndef DATA_H
#define DATA_H

#include "system.h"
#include <algorithm>

#define MISSING INT_MAX

#define RESOLUTION 10

class Dataset {
public:
    string dataset_type; // string storing the dataset type for human reading
    string dir; // location of the dataset
    str_map attr_names; // Human readable attribute names
    vector<str_map> val_names; // Human readable value of attribute names
    
    /* properties of the attributes */
    vector<uint> vmax;
    vector<uint> vmin;
    vector<uint> vrange;

    /* store int here showing the exponent. 0 if discrete*/
    vector<uint> is_continuous; // use this vector to track if an element is continuous

    /* Storage for the dataset, first element is the class */
    vector<vector<uint>> data;

    /*Used by ID3 to determine if a variable has been used yet*/
    vector<uint> used;
    Dataset();
    Dataset(string type, string directory);
    void init_bimaps();
    void print_dataset(bool strs);
    void print_datum(bool strs, uint index);
    void print_datum(datum attrs);
    vector<uint> num_var_class(uint var, uint var_type, uint d_class);
        
    
    void print_val(uint i, uint c);
    void print_attr(uint i);
    void discretize();
    void find_mmr();
    vector<Dataset>rand_split(uint num);
    vector<Dataset> get_strat_fold(uint k);
private:
};

bool cmp_class(vector<uint> p1, vector<uint> p2);


#endif /* DATA_H */

