
#include "vote.h"

VoteDataset::VoteDataset() : Dataset(VOTE_DATA_TYPE, VOTE_DATA_DIR) {
  
    read_data();
    find_mmr();
}

void VoteDataset::read_data() {

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
        while (iss.good()) {
            string value;
            uint valInt;
            getline(iss, value, ',');

            
            if (value == "?" || value == "republican") {     // y or democrat == 2
                valInt = 1;
            }
            else if (value == "y" || value == "democrat") {    // n or republican == 1
                valInt = 2;
            }
            else if (value == "n") {    // ? == MISSING
                valInt = 3;
            }
            
            datum.push_back(valInt);
            if(n == 0) is_continuous.push_back(0);

        }

        data.push_back(datum);
        n++;

    }
    data_file.close();

}

