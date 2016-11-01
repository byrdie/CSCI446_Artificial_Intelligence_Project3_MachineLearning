
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

        /* Loop through each value on that line */
        vector<uint> datum;
        while (iss.good()) {
            string value;
            getline(iss, value, ',');

            /* convert the data to int if possible */
            try {
                datum.push_back(stoi(value));
            } catch (invalid_argument& e) {
                datum.push_back(MISSING);
                //                if (value.at(0) == 'D') { // This is the class 
                //                    datum.insert(datum.begin(), stoi(value.substr(1, value.size() - 1)));
                //                } else { // otherwise its a missing value
                //                    datum.push_back(MISSING);
                //                }
            }
        }
        /* rotate the vector so the class is the first element */
        rotate(datum.begin(), datum.end() - 1, datum.end());


        /* scale the classes */
        datum[0] = datum[0] / 2;
        data.push_back(datum);

    }
    data_file.close();

}