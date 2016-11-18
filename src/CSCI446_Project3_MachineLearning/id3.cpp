/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "id3.h"

ID3::ID3(Dataset train_data) : Learner(train_data) {
    td.print_dataset(false);
    td.used.assign(td.data[0].size(), 0);
    num_var_types = num_vars();
    m_entropy = master_entropy();
    compute_var_gain(td);
    cout << "P_valL "<< max_gain(compute_var_gain(td),td)<<endl;
}

void ID3::learn() {
    cout << "test" << endl;
}

uint ID3::answer(datum attrs) {
    return 0;
}

Graph ID3::id3(Dataset set, Dataset parent) {
    if (set.data.size() == 0) {
        int x = 5;
    }else if(same_class(set)){
        
    }else if(attributes_empty(set)){
        
    }else{
        int argmax_idx = max_gain(compute_var_gain(set));
        
    }
}

vector<int> ID3::num_vars() {
    /*finds number of discrete values for each variable*/
    vector<int> max_var;
    for (uint j = 0; j < td.data[0].size(); j++) {
        int instances = 0;
        for (uint i = 0; i < td.data.size(); i++) {
            if (td.data[i][j] > instances) {
                instances = td.data[i][j];
            }
        }
        max_var.push_back(instances);
    }
    return max_var;
}

float ID3::master_entropy() {
    /*determines entropy of the system as a whole*/
    vector<float> class_num;
    for (uint i = 1; i <= num_var_types[0]; i++) {
        float num = 0;
        for (uint j = 0; j < td.data.size(); j++) {
            if (td.data[j][0] == i) {
                num += 1;
            }
        }
        class_num.push_back(num);
    }
    float entropy = 0;
    for (uint i = 0; i < class_num.size(); i++) {
        entropy -= ((double) (class_num[i] / td.data.size())) * log2(((double) (class_num[i] / td.data.size())));
    }
    return entropy;
}

vector<float> ID3::compute_var_gain(Dataset set) {
    /*loop through each variable and compute gain*/
    vector<float> var_gain;

    /*loops through each variable*/
    for (uint j = 1; j < set.data[0].size(); j++) {
        float class_gain = 0;

        /*loops through each class*/
        for (uint i = 0; i < num_var_types[0]; i++) {

            float loc_gain = compute_class_gain(set, i + 1, j);
            int class_count = 0;
            /*find probability of that class*/
            for (uint k = 0; k < set.data.size(); k++) {
                if (set.data[k][0] == i + 1) {
                    class_count++;
                }
            }
            /*multiply by probability of that class. Weighted average*/
            class_gain -= loc_gain * ((double) class_count / set.data.size());
        }
        /*compute gain and add to list*/
        var_gain.push_back(m_entropy - class_gain);
        cout << "gain " << m_entropy - class_gain << endl;

    }

    /*list of each attributes gain*/
    return var_gain;
}

float ID3::compute_class_gain(Dataset set, int d_class, int var) {
    /*compute gain for one variable where one class is positive and the rest are negative
     This method is for use with discrete values only.
     */
    vector<float> var_entropy;
    /*Loop through each possible value*/
    for (uint i = 1; i < num_var_types[var] + 1; i++) {
        /*calculate number of variables that are a part of that class and
         * total number of variables that are of that value
         */
        vector<int> var_class_count = set.num_var_class(var, i, d_class);
        int tot_var = var_class_count[0];
        int var_class = var_class_count[1];
        //calculate positives
        double d1 = ((double) var_class / tot_var) * log2((double) var_class / tot_var);
        //calculate negatives.
        double d2 = ((double) (tot_var - var_class) / tot_var) * log2((double) (tot_var - var_class) / tot_var);

        //change nan to 0's
        if (isnan(d1)) {
            d1 = 0;
        }
        if (isnan(d2)) {
            d2 = 0;
        }
        //push back gain
        var_entropy.push_back(((0 - d1) - d2));
        //save total number of vars to make weighted average
        var_entropy.push_back((double) tot_var);
    }


    float entropy = 0;
    //apply weighted average
    for (int i = 0; i < var_entropy.size(); i += 2) {
        if (isnan(((double) var_entropy[i + 1] / set.data.size()) * var_entropy[i])) {
            entropy += 0;
        } else {
            entropy -= ((double) var_entropy[i + 1] / set.data.size()) * var_entropy[i];
        }
    }
    return entropy;
}

bool ID3::same_class(Dataset set) {
    int class_check = set.data[0][0];
    for (uint i = 0; i < set.data.size(); i++) {
        if (set.data[i][0] != class_check) {
            return false;
        }
    }
    return true;
}

int ID3::max_gain(vector<float> gains, Dataset set) {
    /*return index of variable with the max gain*/
    int max_gain = 0;
    for (int i = 1; i < gains.size() + 1; i++) {
        if (!set.used[i] && (gains[i - 1] > gains[max_gain])) {
            max_gain = i - 1;
        }
    }
    vector<int> pos_index;
    for(int i = 1; i < gains.size() + 1; i++){
        if (!set.used[i] && (gains[i - 1] == gains[max_gain])){
            pos_index.push_back(i);
        }
    }
    return pos_index[rand()%pos_index.size()];
}

int ID3::plurality_value(Dataset set) {
    vector<int> value(num_var_types[0], 0);
    for (int i = 0; i < set.data.size(); i++) {
        value[set.data[i][0] - 1] += 1;
    }
    int max_index = 0;
    for (int i = 0; i < value.size(); i++) {
        if (value[i] > value[max_index]) {
            max_index = i;
        }
    }
    vector<int> pos_index;
    for (int i = 0; i < value.size(); i++) {
        if (value[i] == value[max_index]) {
            pos_index.push_back(i);
        }
    }
    return pos_index[rand()%pos_index.size()] + 1;
}

bool ID3::attributes_empty(Dataset set){
    for(int i = 0; i < set.used.size(); i++){
        if(set.used[i] == 0){
            return false;
        }
    }
    return true;
}