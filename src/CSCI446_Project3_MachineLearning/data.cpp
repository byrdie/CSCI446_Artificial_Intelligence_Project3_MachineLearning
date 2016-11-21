/* 
 * File:   data.cpp
 * Author: Roy Smart
 *
 * Created on October 26, 2016, 9:32 PM
 */

#include "data.h"

Dataset::Dataset(string type, string directory) {

    dataset_type = type;
    dir = directory;

    /* initialize bi-directional map for visualization */
    init_bimaps();


}

Dataset::Dataset() {

}

void Dataset::init_bimaps() {

    /* Fill the maps of the attribute names */
    ifstream attr_file(dir + "attr_name_str");
    int i = 0;
    while (attr_file.good()) {
        string value;
        getline(attr_file, value);
        attr_names.insert({i, value});
        i++;
    }
    attr_file.close();

    /* Fill the maps of the value names */
    ifstream val_file(dir + "val_name_str");
    while (val_file.good()) {

        /* get the next line from the csv */
        string line;
        getline(val_file, line);
        stringstream iss(line);

        if (line.empty()) {
            continue;
        }

        /* Loop through each value on that line */
        str_map val_name;
        int j = 0;
        while (iss.good()) {
            string value;
            getline(iss, value, ',');
            val_name.insert({j + 1, value});
            j++;
        }
        val_name.insert({MISSING, "?"});
        val_names.push_back(val_name);

    }
    val_file.close();

}

void Dataset::print_dataset(bool strs) {

    out << dataset_type << "\n";

    /* Loop through each data entry */
    for (uint i = 0; i < data.size(); i++) {

        datum attrs = data[i];

        /* Loop through each element of each data entry */
        for (uint j = 0; j < attrs.size(); j++) {
            if (strs) {
                if (j < val_names.size()) {
                    cout << val_names[j].left.find(attrs[j])->second;
                } else {
                    cout << attrs[j];
                }
            } else {
                cout << attrs[j];
            }

            if (j < attrs.size() - 1) {
                cout << ',';
            }
        }
        cout << "\n";
    }
    cout << "\n";
}



void Dataset::print_datum(bool strs, uint index) {
    vector<uint> attrs = data[index];

    for (uint j = 0; j < attrs.size(); j++) {
        if (strs) {
            out << attr_names.left.find(j)->second << ": ";
            if (j < val_names.size()) {
                out << val_names[j].left.find(attrs[j])->second;
            } else {
                out << attrs[j];
            }
                       out << ", ";
        } else {
            out << attrs[j];

            if (j < attrs.size() - 1) {
                out << ',';
            } else {
                                out << ", ";
            }
        }


    }

}

void Dataset::print_datum(datum attrs) {
    for (uint j = 0; j < attrs.size(); j++) {
        out << attr_names.left.find(j)->second << ": ";
        if (j < val_names.size()) {
            out << val_names[j].left.find(attrs[j])->second;
        } else {
            out << attrs[j];
        }
        if (j < attrs.size() - 1) {
            out << ", ";
        }



    }
}

vector<uint> Dataset::num_var_class(uint var, uint var_type, uint d_class) {
    /*Returns number of variables of a certain type and how many times that type defines a class*/
    int c_v_type = 0;
    int c_d_class = 0;
    for (uint i = 0; i < data.size(); i++) {
        if (data[i][var] == var_type) {
            c_v_type++;
            if (data[i][0] == d_class) {
                c_d_class++;
            }
        }
    }

    vector<uint> ret_val;
    ret_val.push_back(c_v_type);
    ret_val.push_back(c_d_class);
    return ret_val;
}

void Dataset::print_val(uint i, uint c) {

    if (i >= val_names.size()) {
        out << c;
    } else {
        out << val_names[i].left.find(c)->second;
    }
}

void Dataset::print_attr(uint i) {

    out << attr_names.left.find(i)->second;

}

void Dataset::discretize() {

    /* loop through the dataset and find the maximum, minimum, range, and perform appropriate binning for each attribute */
    for (uint j = 0; j < data[0].size(); j++) {

        /* Using the range and the RESOLUTION preprocessor definition, put values into bins */
        if (is_continuous[j] > 0) {
            double dx = (double) vrange[j] / (double) RESOLUTION; // change in position
            double x = vmin[j]; // Current position

            /* Loop through each bin */
            uint k;
            for (k = 0; k < RESOLUTION - 1; k++) {

                /* Loop through each datum in the dataset and check if the attribute
                 * belongs in the current bin */
                for (uint i = 0; i < data.size(); i++) {
                    uint attr = data[i][j];

                    /* Deal with the last bin separately to make sure all values are included
                     * due to truncation error in integer division */
                    if (k != (RESOLUTION - 2)) { // common case          
                        if ((attr >= k * dx + x) and (attr < (k + 1) * dx + x)) {
                            data[i][j] = k + 1;
                        }
                    } else { // corner case
                        if (attr >= k * dx + x) {
                            data[i][j] = k + 1;
                        }
                    }
                }
            }

            /* update the properties of each attribute */
            vmin[j] = 1;
            vmax[j] = RESOLUTION;
            vrange[j] = vmax[j] - vmin[j];
        }
    }
}

/**
 * Function for finding the min, max and range of the dataset
 */
void Dataset::find_mmr() {

    /* initialize max min and range datasets */
    vmax.assign(data[0].size(), 0);
    vmin.assign(data[0].size(), INT_MAX);
    vrange.assign(data[0].size(), 0);

    for (uint j = 0; j < data[0].size(); j++) {
        for (uint i = 0; i < data.size(); i++) {

            if (vmax[j] < data[i][j]) {
                vmax[j] = data[i][j];
            }
            if (vmin[j] > data[i][j]) {
                vmin[j] = data[i][j];
            }
        }

        /* Use the max/min to determine the range */
        vrange[j] = vmax[j] - vmin[j];
    }
}

/**
 * Split the dataset randomly into num partitions
 * @param num
 * @return 
 */
vector<Dataset> Dataset::rand_split(uint num) {

    vector<Dataset> folds;

    for (uint i = 0; i < num; i++) {
        Dataset d = *this; // Copy the info from this instance
        d.data.resize(0); // Delete the data field
        folds.push_back(d);
    }

    /* Loop over the current dataset until it is empty */
    vector<vector < uint>> dc = data; // Make a copy of the data;
    while (true) {

        /* Loop through every fold */
        for (uint i = 0; i < folds.size(); i++) {

            /* Select a new index at random */
            uint j = rand() % dc.size();

            /* Insert into new dataset and delete the original */
            datum attrs = dc[j];
            folds[i].data.push_back(attrs);
            dc.erase(dc.begin() + j);
            if (dc.empty()) {
                return folds;
            }

        }

    }
}

vector<Dataset> Dataset::get_strat_fold(uint k) {

    vector<Dataset> folds;
    
    for (uint i = 0; i < k; i++) {
        Dataset d = *this; // Copy the info from this instance
        d.data.resize(0); // Delete the data field
        folds.push_back(d);
    }

    /* randomize the elements of a vector */
    vector<vector < uint>> cdata = data; // but first make a copy  
    random_shuffle(cdata.begin(), cdata.end());
    sort(cdata.begin(), cdata.end(), cmp_class);

    while (true) {

        /* Loop through every fold */
        for (uint i = 0; i < folds.size(); i++) {

            /* Insert into new dataset and delete the original */
            datum attrs = cdata.back();
            folds[i].data.push_back(attrs);
            cdata.pop_back();
            if (cdata.empty()) {
                
                
                return folds;
            }
        }

    }
    
    

}

bool cmp_class(vector<uint> p1, vector<uint> p2) {
    return p1[0] < p2[0];
}

vector<pair<Dataset, Dataset>> folds_to_dsets(vector<Dataset> folds){
    
    /* space to store returned datasets */
    vector<pair<Dataset, Dataset>> dsets;
    
    /* Loop through each fold */
    for(uint i = 0; i < folds.size(); i++){
        

        pair<Dataset,Dataset> set; // space to store one train, test dataset pair
        set.first = folds[i];   // Allocate the validation set
        
        uint j = (i + 1) % folds.size();
        set.second = folds[j];      // Allocate the training set
        
        /* append the remaining folds into the training set */
        for(uint k = 0; k < folds.size(); k++){
            
            if((k != i) and (k != j)){
                vector<vector<uint>> * td1 = &set.second.data;
                vector<vector<uint>> td2 = folds[k].data;
                
                td1->insert(td1->end(), td2.begin(), td2.end());
            }
            
        }
        dsets.push_back(set);
        
    }
    
    return dsets;
    
}