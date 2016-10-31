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

        /* Loop through each value on that line */
        str_map val_name;
        int j = 0;
        while (iss.good()) {
            string value;
            getline(iss, value, ',');
            val_name.insert({j, value});
            j++;
        }
        val_names.push_back(val_name);
    }
    val_file.close();

}

void Dataset::print_dataset() {

    for (uint i = 0; i < data.size(); i++) {

        vector<uint> datum = data[i];

        for (uint j = 0; j < datum.size(); j++) {

            out << datum[j];
            
        }

        out << "\n";
    }

}