
#include "soybean.h"

SoybeanDataset::SoybeanDataset() : Dataset(SOYBEAN_DATA_TYPE, SOYBEAN_DATA_DIR) {

    
    
    read_data();

}

void SoybeanDataset::read_data() {

    /* open the data file and loop through */
    ifstream data_file(dir + "data");
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
                //                if (value.at(0) == 'D') { // This is the class 
                //                    datum.insert(datum.begin(), stoi(value.substr(1, value.size() - 1)));
                //                } else { // otherwise its a missing value
                //                    datum.push_back(MISSING);
                //                }
            }
        }
        string value;
        uint val;
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
        

        /* rotate the vector so the class is the first element */
        //rotate(datum.begin(), datum.end() - 1, datum.end());


        /* scale the classes */
        //datum[0] = datum[0] / 2;
        data.push_back(datum);
        

    }
    data_file.close();

}