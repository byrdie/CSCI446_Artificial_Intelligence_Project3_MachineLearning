/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "id3.h"

ID3::ID3(Dataset train_data) : Learner(train_data) {
    training_dataset.print_dataset(false);
    training_dataset.used.assign(training_dataset.data[0].size(), 0);
    num_var_types = num_vars();
    m_entropy = master_entropy();
    cout << m_entropy << endl;
    cout << training_dataset.data.size() << endl;

}

void ID3::learn() {
    cout << "test" << endl;
}

uint ID3::answer(datum attrs) {
    return 0;
}

vector<int> ID3::num_vars() {
    vector<int> max_var;
    for (uint j = 0; j < training_dataset.data[0].size(); j++) {
        int instances = 0;
        for (uint i = 0; i < training_dataset.data.size(); i++) {
            if (training_dataset.data[i][j] > instances) {
                instances = training_dataset.data[i][j];
            }
        }
        cout << instances << ",";
        max_var.push_back(instances);
    }
    cout << endl;
    return max_var;
}

float ID3::master_entropy() {
    vector<float> class_num;
    for (uint i = 1; i <= num_var_types[0]; i++) {
        float num = 0;
        for (uint j = 0; j < training_dataset.data.size(); j++) {
            if (training_dataset.data[j][0] == i) {
                num += 1;
            }
        }
        class_num.push_back(num);
    }
    float entropy = 0;
    for (uint i = 0; i < class_num.size(); i++) {
        entropy -= ((double) (class_num[i] / training_dataset.data.size())) * log2(((double) (class_num[i] / training_dataset.data.size())));
        cout << entropy << endl;
    }
    return entropy;
}

vector<float> ID3::compute_var_gain(Dataset set, int var) {
    /*loop through each variable and compute gain*/
    vector<float> var_gain;

    for (uint j = 1; j < set.data[0].size(); j++) {
        float class_gain = 0;
        for (uint i = 0; i < num_var_types[0]; i++) {
            class_gain += compute_class_gain(set, i + 1, j);
        }
        var_gain.push_back(class_gain);
    }
    return var_gain;
}

float ID3::compute_class_gain(Dataset set, int d_class, int var) {
    /*compute gain for one variable where one class is positive and the rest are negative*/
    vector<float> var_entropy;
    for (uint i = 1; i < num_var_types[var] + 1; i++) {
        vector<int> var_class_count = set.num_var_class(var, i, d_class);
        int tot_var = var_class_count[0];
        int var_class = var_class_count[1];
        double d1 = ((double) var_class / tot_var) * log2((double) var_class / tot_var);
        double d2 = ((double) (1 - var_class) / tot_var) * log2((double) (1 - var_class) / tot_var);
        var_entropy.push_back((0 - d1 - d2));
        var_entropy.push_back((double) tot_var);
    }
    float entropy = 0;
    for (int i = 0; i < var_entropy.size(); i += 2) {
        entropy += ((double) var_entropy[i + 1] / set.data.size()) * var_entropy[i];
    }
    return entropy;

}

