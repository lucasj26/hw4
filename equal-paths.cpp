#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

bool depthHelper(Node* root, int depth, int& firstDepth)
{
    if(root==nullptr){return true;}

    if(root->left == nullptr && root->right == nullptr){    
        if (firstDepth==-1){
            firstDepth = depth;
        }
        else{
            return firstDepth == depth;
        }
    }


    bool left = depthHelper(root->left, depth + 1, firstDepth);
    bool right = depthHelper(root->right, depth + 1, firstDepth);
    return left && right;
}


bool equalPaths(Node * root)
{
    // Add your code below


    int depth = -1;
    return depthHelper(root,0,depth); 

}

