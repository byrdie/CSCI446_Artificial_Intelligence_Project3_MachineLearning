/* 
 * File:   iris.h
 * Author: byrdie
 *
 * Created on October 30, 2016, 4:31 PM
 */

#ifndef IRIS_H
#define IRIS_H
#include "system.h"
#include "data.h"

#define IRIS_DATA_TYPE "Iris Identification database"
#define IRIS_DATA_DIR "../../data/iris/"

#define IRIS_SETOSA_LEN 11
#define IRIS_VERSICOLOR_LEN 15
#define IRIS_VIRGINICA_LEN 14

#define IRIS_SETOSA 1
#define IRIS_VERSICOLOR 2
#define IRIS_VIRGINICA 3

class IrisDataset : public Dataset {
public:
    IrisDataset();  
private:
    void read_data();
};


#endif /* IRIS_H */

