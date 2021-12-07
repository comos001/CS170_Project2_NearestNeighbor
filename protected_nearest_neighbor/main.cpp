#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include <string>
#include <sstream>
#include <fstream>
#include <limits>
#include <cmath>
#include <chrono>
#include "Node.hpp"


using namespace std::chrono;
using namespace std;

vector <vector<float>> load_data(string filename){
    ifstream testfile;
    vector <vector<float>> objects;
    vector <float> features;
    float numerical_data = 0;
    string content;
    string space;
    
    testfile.open(filename);
    
    if (testfile.is_open()){
        while(getline(testfile, space)){
            stringstream line(space);
            while(line >> content) {
                numerical_data = stof(content);
                features.push_back(numerical_data);
            }
            objects.push_back(features);
            features.clear();
        }
    }
    return objects;
}


float leave_one_out_cross_validation(vector<vector<float>> data, vector<int> current_set, int feature_to_add, int feature_to_remove){
    float distance = 0;
    float nn_dist = 0;
    int nn_label = 0;
    int nn_loc = 0;
    float accuracy = 0;
    float correct_class = 0;
    bool first_obj = true;
  
    vector <vector<float>> temp_data;
    temp_data = data;
    
    //Options for preparing data based on either backward elimination or forward selection
    // go through temp zero out anything not found in current set
    if (feature_to_add != 0 && feature_to_remove == 0){
        for (int a = 0; a < temp_data.size(); a++){
            for(int b = 1; b < temp_data[a].size(); b++){
                if (find(current_set.begin(), current_set.end(), b) != current_set.end()){
                //means that it is found
                }
                else{
                    if (b != feature_to_add){
                        temp_data[a][b] = 0;
                    }
                }
            }
        }
    }
    
    if (feature_to_remove != 0 && feature_to_add == 0){
        for (int a = 0; a < temp_data.size(); a++){
            for(int b = 1; b < temp_data[a].size(); b++){
                if (find(current_set.begin(), current_set.end(), b) != current_set.end()){
                    if(b == feature_to_remove){
                        temp_data[a][b] = 0;
                    }
                }
                else{
                    temp_data[a][b] = 0;
                }
            }
        }
    }
    
    
    
    
    
    for (int i = 0; i < temp_data.size(); i++){
        for (int j = 0; j < temp_data.size(); j++){
            if (j != i){
                distance = 0;
                for (int k = 1; k < temp_data[j].size(); k++){
                    distance += pow((temp_data[i][k] - temp_data[j][k]), 2);
                }
                float temp = sqrt(distance);
                distance = temp;
                
                if (distance < nn_dist || (j == 0) || (first_obj == true)){
                    nn_dist = distance;
                    nn_loc = j;
                    nn_label = data[j][0];
                    first_obj = false;
                }
            }
        }
        
        if (data[i][0] == nn_label){
            correct_class++;
        }
//        cout << "object " << i << " is class " << data[i][0] << endl;
//        cout << "Its nearest neighbor is " << nn_loc << " which is in class " << nn_label << endl;
    }
    
    accuracy = correct_class/data.size();
    return accuracy;
}





Node forwardSelection(vector<vector<float>> data){
    vector <int> feature_to_add_at_this_level;
    vector <int> current_set;
    int feature_to_add = 0; 
    float best_accuracy_so_far = 0;
    float top_accuracy = 0;
    vector<int> top_subset;
    float accuracy;
    Node top;
    

    for (int i = 0; i < data[0].size() - 1; i++){
        cout << "On level " << i + 1 << " of the search tree\n";

        best_accuracy_so_far = 0;
        accuracy = 0;
      
        for (int j = 1; j < data[i].size(); j++){
            //check if j is in feature selected vector
            if (find(current_set.begin(), current_set.end(), j) != current_set.end()){
                //data is found
            }
            else{
                cout << "--Considering adding feature " << j << endl;
                accuracy = leave_one_out_cross_validation(data, current_set, j, 0);
               

                //check for best accuracy
                if (accuracy > best_accuracy_so_far){
                    best_accuracy_so_far = accuracy;
                    feature_to_add = j;
                }
            }
        }
//        cout << "feature added: " << feature_to_add << endl;
//        cout << "best so far: " << best_accuracy_so_far << endl;
        current_set.push_back(feature_to_add);
//        cout << "feature list: ";
//        for (int d = 0; d < current_set.size(); d++){
//            cout << current_set[d];
//        }
//        cout << endl;
        
        if (best_accuracy_so_far > top_accuracy){
            top_accuracy = best_accuracy_so_far;
            top_subset = current_set;
        }

    }
    
    top.set_accuracy(top_accuracy);
    top.set_feature_subset(top_subset);

    return top;
}


Node backwardElimination(vector<vector<float>> data){
    vector <int> feature_to_add_at_this_level;
    vector <int> current_set;
    vector <int>::iterator feature_to_remove;
    int feature_removed = 0;
    float best_accuracy_so_far = 0;
    float top_accuracy = 0;
    vector<int> top_subset;
    float accuracy;
    Node top;
    
    vector <vector<float>> temp = data;
//  populate current set with all the features
    for (int feature = 1; feature < temp[0].size(); feature++){
        current_set.push_back(feature);
    }
    
   
    
    for (int i = 0; i < data[0].size() - 1; i++){
        cout << "On level " << i + 1 << " of the search tree\n";

        best_accuracy_so_far = 0;
        accuracy = 0;
      
        for (int j = 1; j < data[i].size(); j++){
            //check if j is in feature selected vector
            if (find(current_set.begin(), current_set.end(), j) != current_set.end()){
                cout << "--Considering removing feature " << j << endl;
                accuracy = leave_one_out_cross_validation(data, current_set, 0, j);
               
                //check for best accuracy
                if (accuracy > best_accuracy_so_far){
                    best_accuracy_so_far = accuracy;
                    feature_removed = j;
                }
            }
        }
        cout << "feature removed: " << feature_removed << endl;
//        cout << "best so far: " << best_accuracy_so_far << endl;
        
        feature_to_remove = remove(current_set.begin(), current_set.end(), feature_removed);
        current_set.pop_back();
        
//        cout << "feature list: ";
//        for (int d = 0; d < current_set.size(); d++){
//            cout << current_set[d];
//        }
//        cout << endl;
        
        if (best_accuracy_so_far > top_accuracy){
            top_accuracy = best_accuracy_so_far;
            top_subset = current_set;
        }

    }
    
    top.set_accuracy(top_accuracy);
    top.set_feature_subset(top_subset);

    return top;
}


int main(int argc, const char * argv[]) {
    vector<vector<float>> data;
    int input;
    Node result;
    string filename;


    cout << "NEAREST NEIGHBOR, CS170 PROJECT 2" << endl << endl;
    cout << "Options" << endl;
    cout << "1 - Small data set" << endl << "2 - Large data set" << endl;
    cin >> input;


    if (input == 1){
        filename = "/Users/victoromosor/Downloads/Ver_2_CS170_Fall_2021_Small_data__61.txt";
    }
    else{
        filename = "/Users/victoromosor/Desktop/protected_nearest_neighbor/protected_nearest_neighbor/Ver_2_CS170_Fall_2021_LARGE_data__87.txt";
    }

    data = load_data(filename);

    cout << endl << "Algorithm" << endl;
    cout << "1 - Forward Selection" << endl << "2 - Backward Elimination" << endl;
    cin >> input;

    if (input == 1){
        auto start = high_resolution_clock::now();
        result = forwardSelection(data);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<seconds>(stop - start);
        cout << endl << "Execution time: " << duration.count()  << " s" << endl;
        result.print_features();
    }
    else{
        auto start = high_resolution_clock::now();
        result = backwardElimination(data);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << endl << "Execution time: " << duration.count() << " s" << endl;
        result.print_features();
    }
    
    vector<int> test;
    test.push_back(10);
    test.push_back(2);

    swap(test[0], test[test.size() - 1]);
    test.pop_back();

    for (int i = 0; i < test.size(); i++){
        cout << test[i] << " ";
    }
  
    
    return 0;
}




