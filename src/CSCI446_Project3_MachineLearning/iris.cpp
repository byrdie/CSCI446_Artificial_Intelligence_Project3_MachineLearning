/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "iris.h"

IrisDataset::IrisDataset() : Dataset(IRIS_DATA_TYPE, IRIS_DATA_DIR) {

    read_data();

}

void IrisDataset::read_data() {

    /* open the data file and loop through */
    ifstream data_file(dir + "data");
    uint n = 0;
    while (data_file.good()) {

        /* get the next line from the csv */
        string line;
        getline(data_file, line);
        stringstream iss(line);

        if (line.empty()) {
            continue;
        }

        vector<uint> datum;
        string value;
        /* Read in the elements one-by-one, since we need some processing */
        for (int i = 0; i < 4; i++) {
            getline(iss, value, ',');
            uint val;
            
            try {
                val = 10 * stof(value);
      
            } catch (invalid_argument& e) {
                val = MISSING;
            }
            datum.push_back(val);
            if(n == 0) is_continuous.push_back(10);
        }
        /*Read in Class name*/
        uint val;
        getline(iss, value, ',');
        switch (value.length()) {
            case IRIS_SETOSA_LEN:
                val = IRIS_SETOSA;
                break;

            case IRIS_VERSICOLOR_LEN:
                val = IRIS_VERSICOLOR;
                break;

            case IRIS_VIRGINICA_LEN:
                val = IRIS_VIRGINICA;
                break;

            default:
                val = MISSING;
        }

        datum.insert(datum.begin(), val);
        if(n == 0) is_continuous.insert(is_continuous.begin(), 0);

        data.push_back(datum);
        n++;
    }
    data_file.close();
}

