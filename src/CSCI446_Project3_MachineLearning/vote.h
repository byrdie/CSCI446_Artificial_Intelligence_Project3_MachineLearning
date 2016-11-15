#ifndef VOTE_H
#define VOTE_H

#include "system.h"
#include "data.h"

#define VOTE_DATA_TYPE "House of Representatives"
#define VOTE_DATA_DIR "../../data/votes/"

class VoteDataset : public Dataset {
public:
    VoteDataset();  
private:
    void read_data();
};


#endif /* VOTE_H */



