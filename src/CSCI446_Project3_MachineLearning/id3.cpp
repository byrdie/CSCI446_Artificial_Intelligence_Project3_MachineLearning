/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "id3.h"

ID3::ID3(Dataset train_data) : Learner(train_data) {
    sname = "ID3";
    viz_count = 200;
    //td.print_dataset(true);
    make_val_set();
    td.used.assign(td.data[0].size(), 0);
    //num_var_types = num_vars();

    tree = Graph<vector < uint >> ();
}

void ID3::learn() {
    m_entropy = master_entropy();
    id3(td, td);
    prune();

}

uint ID3::answer(datum attrs) {
    out << "Tree path: \n";
    //Start at root vertice
    Vert<vector < uint>>*vert = tree.verts[0];
    //keep going until a leaf node is reached
    while (vert->edges.size() != 1) {
        //loop over each edge
        for (int i = 0; i < vert->edges.size(); i++) {
            //check discrete values with string names
            if (vert->edges[i]->name.c_str() != to_string(attrs[vert->val[0]])) {
                if (attrs[vert->val[0]] == td.val_names[vert->val[0]].right.find(vert->edges[i]->name)->second) {
                    out << "Variable: " << vert->name << "   ";
                    out << "Type: " << vert->edges[i]->name << "\n";
                    vert = vert->edges[i]->verts[1];

                }
                //continuous variable that have been discretized and are represented by a number
            } else {
                if (atoi(vert->edges[i]->name.c_str()) == attrs[vert->val[0]]) {
                    out << "Variable: " << vert->name << "   ";
                    out << "Type: " << vert->edges[i]->name << "\n";
                    vert = vert->edges[i]->verts[1];

                }
            }
        }
    }
    //return value of leaf node
    out << "Prediction: " << vert->name << "\n\n";
    return td.val_names[0].right.find(vert->name)->second;
}

Vert<vector<uint>>*ID3::id3(Dataset set, Dataset parent) {
    /*The main method described in the textbook*/
    viz_count++;
    //If no more data, use plurality of the parent
    if (set.data.size() == 0) {
        vector<uint> temp;
        uint class_id = plurality_value(parent);
        Vert<vector < uint>>*v = tree.add_vert(set.val_names[0].left.find(class_id)->second, temp);
        vector<int> data;
        return v;
        //if all are of the same class, return that class
    } else if (same_class(set)) {
        vector<uint> temp;
        Vert<vector < uint>>*v = tree.add_vert(set.val_names[0].left.find(set.data[0][0])->second, temp);
        return v;
        //if there are no more attributes to split on, return plurality of the set
    } else if (attributes_empty(set)) {
        vector<uint> temp;
        uint class_id = plurality_value(set);
        Vert<vector < uint>>*v = tree.add_vert(set.val_names[0].left.find(class_id)->second, temp);
        return v;
    } else {
        vector<uint> temp;

        //compute index of max gain
        vector<float> gain = compute_var_gain(set);
        uint argmax_idx = max_gain(gain, set);

        out << "Variable= " << set.attr_names.left.find(argmax_idx)->second << "\n";
        out << "max_gain= " << gain[argmax_idx - 1] << "\n";
        out << "gain_list= " << gain[argmax_idx - 1];
        for (uint i = 0; i < gain.size(); i++) {
            out << gain[i] << ",";
        }
        out << "\n\n";
        temp.push_back(argmax_idx);
        temp.push_back((float) plurality_value(set));

        //create vertex that contains argmax
        Vert<vector < uint>>*v = tree.add_vert(set.attr_names.left.find(argmax_idx)->second, temp);

        //set variable as used
        set.used[argmax_idx] = 1;

        //for each possible value of the variable
        for (uint i = 0; i < num_var_types[argmax_idx]; i++) {

            //create new dataset with datums that contain the possible value
            Dataset var_copy = set;
            var_copy.data.resize(0); // Delete the data field
            //copy dataset over
            for (uint j = 0; j < set.data.size(); j++) {
                if (set.data[j][argmax_idx] == (i + 1)) {
                    var_copy.data.push_back(set.data[j]);
                }
            }

            //recurse on new set

            Vert<vector < uint>>*sub_tree = id3(var_copy, set);

            //add edge between the result of the recursive call and the current node
            Edge<vector < uint>>*edge = tree.add_edge(0, v, sub_tree, 1);

            //if continuous, use integer else use string values to name edges
            if (set.is_continuous[argmax_idx] == 0) {
                edge->name = set.val_names[argmax_idx].left.find(i + 1)->second;
            } else {
                edge->name = to_string(i + 1);
                edge->w = 1;
            }

            //seperate tree info for printing
            sub_tree->gname = to_string(viz_count);
            viz_count++;
        }

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

    //count instances of each class
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

    //calculate sum of entropy from each class
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
    for (uint i = 1; i < num_var_types[var] + 1; i++) {
        /*calculate number of variables that are a part of that class and
         * total number of variables that are of that value
         */
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


    float gain = 0;
    //apply weighted average
    for (uint i = 0; i < var_entropy.size(); i += 2) {
        if (isnan(((double) var_entropy[i + 1] / set.data.size()) * var_entropy[i])) {
            gain += 0;
        } else {
            gain -= ((double) var_entropy[i + 1] / set.data.size()) * var_entropy[i];
        }
    }

    return gain;
}

bool ID3::same_class(Dataset set) {
    //checks if all datums in a set are of the same class
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
    //set max_gain to next unused variable
    for (uint i = 1; i < gains.size() + 1; i++) {
        if (set.used[max_gain + 1]) {
            max_gain = i;
        }
    }
    //find the max gain
    for (uint i = 1; i < gains.size() + 1; i++) {
        if (!set.used[i] && (gains[i - 1] > gains[max_gain])) {
            max_gain = i - 1;
        }
    }


    //find variable that have an equal gain
    vector<uint> pos_index;
    for (uint i = 1; i < gains.size() + 1; i++) {
        if (!set.used[i] && (gains[i - 1] == gains[max_gain])) {
            pos_index.push_back(i);
        }
    }
    //randomly choose from list to break ties
    return pos_index[rand() % pos_index.size()];
}

int ID3::plurality_value(Dataset set) {
    //returns the majority of classes in that set
    vector<uint> value(num_var_types[0], 0);
    //find and save number of instances of each class
    for (uint i = 0; i < set.data.size(); i++) {
        value[set.data[i][0] - 1] += 1;
    }
    //find the max class
    int max_index = 0;
    for (uint i = 0; i < value.size(); i++) {
        if (value[i] > value[max_index]) {
            max_index = i;
        }
    }
    vector<uint> pos_index;
    //find and save class count that are equal to the max class
    for (uint i = 0; i < value.size(); i++) {
        if (value[i] == value[max_index]) {
            pos_index.push_back(i);
        }
    }
    //randomly choose from list of classes to break ties.
    return pos_index[rand() % pos_index.size()] + 1;
}

bool ID3::attributes_empty(Dataset set) {
    //checks if all attributes have been used
    for (uint i = 1; i < set.used.size(); i++) {
        if (set.used[i] == 0) {
            return false;
        }
    }
    return true;
}

void ID3::prune() {
    for (uint i = 0; i < tree.verts.size(); i++) {
        if (tree.verts[i]->edges.size() == 1) {
            tree.verts[i]->pruning.push_back(1);
        } else {
            tree.verts[i]->pruning.push_back(0);
        }
    }
    uint error = validate();
    //cout << "Sucessfull Pruning!!!!!!!!!!!!!!!!!!!"<<endl;
    for (uint i = 0; i < tree.verts.size(); i++) {
        if (tree.verts[i]->edges.size() > 1) {
            tree.verts[i]->pruning[0] = 1;
            uint result = validate();
            if (result <= error) {
                tree.verts[i]->pruning[0] = 0;
            } else {
                cout << "Sucessfull Pruning!!!!!!!!!!!!!!!!!!! " << result << endl;
                cout << "Sucessfull Pruning!!!!!!!!!!!!!!!!!!! " << error << endl << endl;
            }
        }
    }
    for (uint i = 0; i < tree.verts.size(); i++) {
        if (tree.verts[i]->pruning[0] == 1 && tree.verts[i]->edges.size() > 1) {
            for (uint j = 0; j < tree.verts[i]->edges.size(); j++) {
                tree.remove_edge(tree.verts[i]->edges[j]);
            }

        }
    }
}

void ID3::make_val_set() {
    vector<pair<Dataset, Dataset>> cv_ds = folds_to_dsets(td.get_strat_fold(3));
    td = cv_ds[0].second;
    val_d = cv_ds[0].first;
}

uint ID3::validate() {
    uint correct = 0;
    for (uint i = 0; i < val_d.data.size(); i++) {

        uint ans = answer_val(val_d.data[i]);
        if (val_d.data[i][0] == ans) {
            correct++;
        }

    }
    return correct;
}

uint ID3::answer_val(datum attrs) {

    //Start at root vertice
    Vert<vector < uint>>*vert = tree.verts[0];
    //keep going until a leaf node is reached
    while (vert->pruning[0] != 1) {
        //loop over each edge
        for (int i = 0; i < vert->edges.size(); i++) {
            //check discrete values with string names
            if (vert->edges[i]->name.c_str() != to_string(attrs[vert->val[0]])) {
                if (attrs[vert->val[0]] == td.val_names[vert->val[0]].right.find(vert->edges[i]->name)->second) {
                    out << "Variable: " << vert->name << "   ";
                    out << "Type: " << vert->edges[i]->name << "\n";
                    vert = vert->edges[i]->verts[1];
                }
                //continuous variable that have been discretized and are represented by a number
            } else {
                if (atoi(vert->edges[i]->name.c_str()) == attrs[vert->val[0]]) {
                    out << "Variable: " << vert->name << "   ";
                    out << "Type: " << vert->edges[i]->name << "\n";
                    vert = vert->edges[i]->verts[1];
                }
            }
        }
    }
    //return value of leaf node

    if (vert->edges.size() > 1) {
       out << "Prediction: " ;
        td.print_val(0, vert->val[1]);
        return vert->val[1];
    } else {
        out << "Prediction: " << vert->name << "\n\n";
        return td.val_names[0].right.find(vert->name)->second;
    }

}

void ID3::set_num_var_types(vector<uint> num) {
    num_var_types = num;
}
