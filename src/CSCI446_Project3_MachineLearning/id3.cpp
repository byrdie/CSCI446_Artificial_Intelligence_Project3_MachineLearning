/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "id3.h"


ID3::ID3(Dataset train_data) : Learner(train_data){
    training_dataset.print_dataset(true);
}

void ID3::learn(){   
    cout << "test" << endl;
}

uint ID3::answer(datum attrs){
    return 0;
}