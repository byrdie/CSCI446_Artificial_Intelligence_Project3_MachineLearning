
#include "cancer.h"

CancerDataset::CancerDataset() : Dataset(CANCER_DATA_TYPE, CANCER_DATA_DIR) {

    
    
    read_data();

}

void CancerDataset::read_data() {

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
        datum attrs;
        while (iss.good()) {
            string value;
            getline(iss, value, ',');

            /* convert the data to int if possible */
            try {
                attrs.push_back(stoi(value));
            } catch (invalid_argument& e) {
                attrs.push_back(MISSING);
            }
            
            is_continuous.push_back(0);
        }
        /* rotate the vector so the class is the first element */
        rotate(attrs.begin(), attrs.end() - 1, attrs.end());


        /* scale the classes */
        attrs[0] = attrs[0] / 2;
        data.push_back(attrs);
        

    }
    data_file.close();

}