
#include "glass.h"

GlassDataset::GlassDataset() : Dataset(GLASS_DATA_TYPE, GLASS_DATA_DIR) {

    read_data();

}

void GlassDataset::read_data() {

    /* open the data file and loop through */
    ifstream data_file(dir + "data");
    while (data_file.good()) {

        /* get the next line from the csv */
        string line;
        getline(data_file, line);
        stringstream iss(line);

        if (line.empty()) {
            continue;
        }

        /* Read in the elements one-by-one, since we need some processing */
        vector<uint> datum;
        string value;

        /* Read in the ID number */
        getline(iss, value, ',');
        try {
            datum.push_back(stoi(value));
        } catch (invalid_argument& e) {
            datum.push_back(MISSING);
        }

        /* Read in the refractive index */
        getline(iss, value, ',');
        uint val;
        try {
            val = 100000 * stof(value);
        } catch (invalid_argument& e) {
            val = MISSING;
        }
        datum.push_back(val);

        /* Read in the mass ratios */
        for (uint j = 0; j < 8; j++) {
            getline(iss, value, ',');

            uint val;
            try {
                val = 100 * stof(value);
            } catch (invalid_argument& e) {
                val = MISSING;
            }
            datum.push_back(val);
        }

        /* Read in the class names */
        getline(iss, value, ',');
        try {
            datum.insert(datum.begin(), stoi(value));
        } catch (invalid_argument& e) {
            datum.insert(datum.begin(), MISSING);
        }
        
        data.push_back(datum);
    }
    data_file.close();

}

