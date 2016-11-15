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

/**
 * Copy constructor
 * @param obj
 */
//Dataset::Dataset(Dataset& obj){
//    
//    cout << "called copy constructor" << endl;
//    
//    dataset_type = obj.dataset_type;
//    dir = obj.dir;
//    attr_names = obj.attr_names;
//    val_names=obj.val_names;
//    is_continuous = obj.is_continuous;
//    data = obj.data;
//    
//}
//
//Dataset::~Dataset(){
//    cout << "called destructor" << endl;
//}

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
    vector<uint> datum = data[index];

    for (uint j = 0; j < datum.size(); j++) {
        if (strs) {
            out << attr_names.left.find(j)->second << ": ";
            if (j < val_names.size()) {
                out << val_names[j].left.find(datum[j])->second;
            } else {
                out << datum[j];
            }
            out << "\n";
        } else {
            out << datum[j];

            if (j < datum.size() - 1) {
                out << ',';
            } else {
                out << "\n";
            }
        }


    }
    out << "_________________________________________\n";

}

void Dataset::discretize() {


    /* initialize max min and range datasets */
    max.assign(data[0].size(), 0);
    min.assign(data[0].size(), INT_MAX);
    range.assign(data[0].size(), 0);

    /* loop through the dataset and find the maximum, minimum, range, and perform appropriate binning for each attribute */
    for (uint j = 0; j < data[0].size(); j++) {
        for (uint i = 0; i < data.size(); i++) {
            if (max[j] < data[i][j]) {
                max[j] = data[i][j];
            }
            if (min[j] > data[i][j]) {
                min[j] = data[i][j];
            }
        }

        /* Use the max/min to determine the range */
        range[j] = max[j] - min[j];

        /* Using the range and the RESOLUTION preprocessor definition, put values into bins */
        if (is_continuous[j] > 0) {
            uint dx = range[j] / RESOLUTION; // change in position
            uint x = min[j]; // Current position

            /* Loop through each bin */
            uint k;
            for (k = 0; k < RESOLUTION - 1; k++) {

                /* Loop through each datum in the dataset and check if the attribute
                 * belongs in the current bin */
                for (uint i = 0; i < data.size(); i++) {
                    uint attr = data[i][j];

                    /* Deal with the last bin separately to make sure all values are included
                     * due to truncation error in integer division */
                    if (k != RESOLUTION - 2) {      // common case          
                        if ((attr >= k * dx + x) and (attr < (k + 1) * dx + x)) {
                            data[i][j] = k + 1;
                        }
                    } else {        // corner case
                        if (attr >= k * dx + x) {
                            data[i][j] = k + 1;
                        }
                    }
                }
            }
            
            /* update the properties of each attribute */
            min[j] = 1;
            max[j] = RESOLUTION;
            range[j] = max[j] - min[j];

        }
    }
}