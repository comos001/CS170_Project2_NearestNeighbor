//
//  Node.hpp
//  protected_nearest_neighbor
//
//  Created by Victor Omosor  on 12/4/21.
//

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;

class Node{
private:
    float accuracy;
    vector<int> feature_subset;
public:
    Node();
    Node(float, vector<int>);
    ~Node();
    void add_feature(int feature);
    void remove_feature(int feature);
//    setter
    void set_accuracy(float);
    void set_feature_subset(vector<int>);
    
//    getter
    long double get_accuracy();
    vector<int> get_feature_subset();
    
//    output
    void print_features();
    
};


#endif /* Node_hpp */
