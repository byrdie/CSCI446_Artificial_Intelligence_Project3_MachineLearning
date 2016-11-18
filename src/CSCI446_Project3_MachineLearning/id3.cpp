/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "id3.h"

ID3::ID3(Dataset train_data) : Learner(train_data) {
    viz_count = 0;
    td.print_dataset(false);
    td.used.assign(td.data[0].size(), 0);
    num_var_types = num_vars();
    m_entropy = master_entropy();

    tree = Graph<vector < uint >> ();
    learn();

    tree.print_gviz("../output/ID3", "test");


}

void ID3::learn() {
    id3(td, td);
}

uint ID3::answer(datum attrs) {
    Vert<vector < uint>>*vert = tree.verts[0];
    while (vert->edges.size() != 1) {
        for (int i = 0; i < vert->edges.size(); i++) {
            if (attrs[vert->val[0]] == td.val_names[vert->val[0]].right.find(vert->edges[i]->name)->second) {
                vert = vert->edges[i]->verts[1];
            }
        }
    }
    return td.val_names[0].right.find(vert->name)->second;
}

Vert<vector<uint>>*ID3::id3(Dataset set, Dataset parent) {
    viz_count++;
    if (set.data.size() == 0) {
        cout << "O-1" << endl;
        vector<uint> temp;
        uint class_id = plurality_value(parent);
        Vert<vector < uint>>*v = tree.add_vert(set.val_names[0].left.find(class_id)->second, temp);
        vector<int> data;
        return v;
    } else if (same_class(set)) {
        cout << "O-2" << endl;
        vector<uint> temp;
        Vert<vector < uint>>*v = tree.add_vert(set.val_names[0].left.find(set.data[0][0])->second, temp);
        return v;
    } else if (attributes_empty(set)) {
        cout << "O-3" << endl;
        vector<uint> temp;
        uint class_id = plurality_value(set);
        Vert<vector < uint>>*v = tree.add_vert(set.val_names[0].left.find(class_id)->second, temp);
        return v;
    } else {
        cout << "O-3" << endl;

        vector<float> gain = compute_var_gain(set);
        vector<uint> temp;
        uint argmax_idx = max_gain(gain, set);
        temp.push_back(argmax_idx);
        Vert<vector < uint>>*v = tree.add_vert(set.attr_names.left.find(argmax_idx)->second, temp);
        //cout << set.attr_names.left.find(argmax_idx)->second << endl;
        set.used[argmax_idx] = 1;

        for (uint i = 0; i < num_var_types[argmax_idx]; i++) {
            Dataset var_copy = set;
            var_copy.data.resize(0); // Delete the data field
            for (uint j = 0; j < set.data.size(); j++) {
                if (set.data[j][argmax_idx] == (i + 1)) {
                    var_copy.data.push_back(set.data[j]);
                }
            }
            cout<< "Max_idx "<<argmax_idx << endl;
            cout <<set.val_names[argmax_idx].left.find(i + 1)->second << endl;
            cout<< "Max_idx "<<argmax_idx << endl;
            Vert<vector < uint>>*sub_tree = id3(var_copy, set);

            Edge<vector < uint>>*edge = tree.add_edge(0, v, sub_tree, 1);

            edge->name = set.val_names[argmax_idx].left.find(i + 1)->second;
            sub_tree->gname = to_string(viz_count);
            viz_count++;
        }
        cout << "test" << endl;
        return v;
    }


}

vector<uint> ID3::num_vars() {
    /*finds number of discrete values for each variable*/
    vector<uint> max_var;
    for (uint j = 0; j < td.data[0].size(); j++) {
        uint instances = 0;
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
        if (isnan(((double) (class_num[i] / td.data.size())) * log2(((double) (class_num[i] / td.data.size()))))) {
            entropy -= 0;
        } else {
            entropy -= ((double) (class_num[i] / td.data.size())) * log2(((double) (class_num[i] / td.data.size())));
        }
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
    // cout << num_var_types[var] << endl;
    for (uint i = 1; i < num_var_types[var] + 1; i++) {
        /*calculate number of variables that are a part of that class and
         * total number of variables that are of that value
         */
        //cout << "Test2" << endl;
        vector<uint> var_class_count = set.num_var_class(var, i, d_class);
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
    for (uint i = 0; i < var_entropy.size(); i += 2) {
        if (isnan(((double) var_entropy[i + 1] / set.data.size()) * var_entropy[i])) {
            entropy += 0;
        } else {
            entropy -= ((double) var_entropy[i + 1] / set.data.size()) * var_entropy[i];
        }
    }

    return entropy;
}

bool ID3::same_class(Dataset set) {
    uint class_check = set.data[0][0];
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
    for (uint i = 1; i < gains.size() + 1; i++) {
        if (!set.used[i] && (gains[i - 1] > gains[max_gain])) {
            max_gain = i - 1;
        }
    }

    vector<uint> pos_index;
    for (uint i = 1; i < gains.size() + 1; i++) {
        if (!set.used[i] && (gains[i - 1] == gains[max_gain])) {
            pos_index.push_back(i);
        }
    }
    return pos_index[rand() % pos_index.size()];
}

int ID3::plurality_value(Dataset set) {
    vector<uint> value(num_var_types[0], 0);
    for (uint i = 0; i < set.data.size(); i++) {
        value[set.data[i][0] - 1] += 1;
    }
    int max_index = 0;
    for (uint i = 0; i < value.size(); i++) {
        if (value[i] > value[max_index]) {
            max_index = i;
        }
    }
    vector<uint> pos_index;
    for (uint i = 0; i < value.size(); i++) {
        if (value[i] == value[max_index]) {
            pos_index.push_back(i);
        }
    }
    return pos_index[rand() % pos_index.size()] + 1;
}

bool ID3::attributes_empty(Dataset set) {
    for (uint i = 0; i < set.used.size(); i++) {
        if (set.used[i] == 0) {
            return false;
        }
    }
    return true;
}