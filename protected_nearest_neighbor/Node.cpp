//
//  Node.cpp
//  protected_nearest_neighbor
//
//  Created by Victor Omosor  on 12/4/21.
//

#include "Node.hpp"


Node::Node(){
};

Node::Node(float accuracy, vector<int> feature_subset){
    this->feature_subset = feature_subset;
    this->accuracy = accuracy;
}

Node::~Node(){};

void Node::add_feature(int feature){
    this->feature_subset.push_back(feature);
}

void Node::remove_feature(int feature){
    
}

long double Node::get_accuracy(){
    return this->accuracy;
}

vector<int> Node::get_feature_subset(){
    return this->feature_subset;
}

void Node::set_accuracy(float accuracy){
    this->accuracy = accuracy;
}

void Node::set_feature_subset(vector<int> feature_subset){
    this->feature_subset = feature_subset;
}

void Node::print_features(){
    cout << endl << "Best accuracy : " << this->accuracy * 100 << "%" << endl;
    cout << "Using Subset : ";
    vector<int> subset = this->feature_subset;
    
    for(int i = 0; i < subset.size(); i++){
        cout << subset[i] << " ";
    }
    
    cout << endl;
    
}
