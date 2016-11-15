/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "id3.h"


ID3::ID3(Dataset train_data) : Learner(train_data){
    td.print_dataset(true);
    td.used.assign(td.data[0].size(), 0);
}

void ID3::learn(){   
    cout << "test" << endl;
}

uint ID3::answer(datum attrs){
    return 0;
}

vector<float> ID3::compute_gain(Dataset set){
    //loop through each variable
    for(uint j = 1; j < set.data[0].size(); j++){
        for(uint i = 0; i < set.data.size(); i++){
            int x = 5;
        }
    }
}