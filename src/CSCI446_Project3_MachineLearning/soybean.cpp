
#include "soybean.h"

SoybeanDataset::SoybeanDataset() : Dataset(SOYBEAN_DATA_TYPE, SOYBEAN_DATA_DIR) {

    
    sname = "soybean";
    read_data();
    find_mmr();

}

void SoybeanDataset::read_data() {

    /* open the data file and loop through */
    ifstream data_file(dir + "data");
    uint n = 0;
    while (data_file.good()) {

        /* get the next line from the csv */
        string line;
        getline(data_file, line);
        stringstream iss(line);
        
        if(line.empty()){
            continue;
        }

        /* Loop through each value on that line */
        vector<uint> datum;
        for(int i = 0; i< 35; i++ ) {
            string value;
            getline(iss, value, ',');

            /* convert the data to int if possible */
            try {
                datum.push_back(stoi(value)+1);
            } catch (invalid_argument& e) {
                datum.push_back(MISSING);
            }
            if(n == 0) is_continuous.push_back(0);
        }
        string value;
        getline(iss, value, ',');
        if(value.at(1) == '1'){
            datum.insert(datum.begin(), 1);
        }else if(value.at(1) == '2'){
            datum.insert(datum.begin(), 2);
        }else if(value.at(1) == '3'){
            datum.insert(datum.begin(), 3);
        }else{
            datum.insert(datum.begin(), 4);
        }
        if(n == 0) is_continuous.push_back(0);
        
        data.push_back(datum);
        n++;

    }
    data_file.close();

}