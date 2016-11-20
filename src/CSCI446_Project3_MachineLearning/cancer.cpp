
#include "cancer.h"

CancerDataset::CancerDataset() : Dataset(CANCER_DATA_TYPE, CANCER_DATA_DIR) {

    sname = "cancer";
    read_data();
    find_mmr();

}

void CancerDataset::read_data() {

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

        /*  */
        string value;
        getline(iss, value, ',');

        /* Loop through each value on that line */
        datum attrs;
        bool missing = false;
        while (iss.good()) {
            string value;
            getline(iss, value, ',');

            /* convert the data to int if possible */
            try {
                attrs.push_back(stoi(value));
            } catch (invalid_argument& e) {
                missing = true;
                attrs.push_back(MISSING);
            }

            if (n == 0) is_continuous.push_back(0);
        }

        if (missing) continue;

        /* rotate the vector so the class is the first element */
        rotate(attrs.begin(), attrs.end() - 1, attrs.end());


        /* scale the classes */
        attrs[0] = attrs[0] / 2;
        data.push_back(attrs);
        n++;

    }
    data_file.close();

}