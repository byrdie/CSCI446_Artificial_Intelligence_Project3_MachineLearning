
#include "vote.h"

VoteDataset::VoteDataset() : Dataset(VOTE_DATA_TYPE, VOTE_DATA_DIR) {

    
    
    read_data();

}

void VoteDataset::read_data() {

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
        while (iss.good()) {
            string value;
            int valInt;
            getline(iss, value, ',');

            
            if (value == "y" || value == "democrat") {     // y or democrat == 2
                valInt = 2;
            }
            else if (value == "n" || value == "republican") {    // n or republican == 1
                valInt = 1;
            }
            else if (value == "?") {    // ? == MISSING
                valInt = MISSING;
            }
            
            datum.push_back(valInt);

        }
        /* rotate the vector so the class is the first element */
        rotate(datum.begin(), datum.end() - 1, datum.end());

        data.push_back(datum);
        

    }
    data_file.close();

}

