#include <iostream>
#include "RedBlackTree.hpp"
#include <cmath>
#include <cstring>

using namespace std;

RedBlackTree::RedBlackTree(){
    root = new Node();
}

RedBlackTree::~RedBlackTree(){
    root->deleteSubtrees();
    delete root;
}

bool RedBlackTree::isEmpty(){
    return root->isSentinel();
}

void RedBlackTree::insert(int num){
    preserveTreeProperties(insertInitial(root, num));
}

void RedBlackTree::preserveTreeProperties(Node * inserted){
    if(inserted->parent == 0){//inserted is the root
        inserted->paintRed();
        return;
    }
    else if(inserted->parent->black){//tree is still valid.
        return;
    }
    else if(inserted->uncle()->isRed()){//Both parent and uncle are red
        inserted->parent->paintBlack();
        inserted->uncle()->paintBlack();
        inserted->grandparent()->paintRed();
        preserveTreeProperties(inserted->grandparent());
    }
}

/*bool RedBlackTree::remove(int num){
    Node** toRemove = find(root, num);
    if(toRemove == 0){//Node not found
        return false;
    }
    else{
        removeNode(toRemove);
        return true;
    }
}*/

/*void RedBlackTree::removeNode(Node** toRemove){
    if((*toRemove)->left == 0 && (*toRemove)->right == 0){//No children
        delete *toRemove;
        *toRemove = 0;
    }
    else if((*toRemove)->left != 0 && (*toRemove)->right != 0){//2 Children
        Node** nextLargest = &((*toRemove)->right);
        while((*nextLargest)->left != 0){
            nextLargest = &((*nextLargest)->left);
        }
        (*toRemove)->value = (*nextLargest)->value;
        removeNode(nextLargest);
    }
    else{//1 child
        Node* child = (*toRemove)->left == 0 ? (*toRemove)->right : (*toRemove)->left;
        delete *toRemove;
        *toRemove = child;
    }
}*/

void RedBlackTree::print(){
    int numLevels = getNumLevels(root, 0);
    //I'll use the same algorithm I developed for the heap, converting the data
    //to an array first.
    int nodes[(int)(pow(2, numLevels)) - 1];
    int* arrPtr = nodes;
    memset(nodes, 0, sizeof(nodes));
    populateArray(arrPtr, 0, root);
    
    int index = 0;
    for(int l=1; l <= numLevels; l++){
        //Initial spaces:
        for(int i=0; i < pow(2, numLevels-l)-1; i++){
            cout << ' ';
        }
        //Nodes:
        for(int n=0; n < pow(2, l-1); n++){
            if(nodes[index]!=0){
                cout << nodes[index] << (nodes[index]->black ? 'B' : 'R');
            }
            else{
                cout << ' ';
            }
            index++;
            //Spaces between nodes:
            for(int i=0; i < pow(2, numLevels - l + 1) - 1; i++){
                cout << ' ';
            }
        }
        cout << endl;
    }
}

Node* RedBlackTree::insertInitial(Node *& child, int num){
    //Initial insertion as in binary search tree; node colored red.
    //Returns pointer to inserted node. Assigns parent and child pointers.
    if(child->isSentinel()){
        child->value = num;
        child->paintRed();
        child->addSentinelLeafs();
        return child;
    }
    if(num < child->value){
        return insertInitial(child->left, num);
    }
    else{
        return insertInitial(child->right, num);
    }
}

int RedBlackTree::getNumLevels(Node* root, int level = 0){
    if(root == 0){
        return level;
    }
    return max(getNumLevels(root->left, level + 1), getNumLevels(root->right, level + 1));
}

void RedBlackTree::populateArray(int *& array, int index, Node* node){
    if(node == 0){
        return;
    }
    array[index] = node->value;
    populateArray(array, index*2+1, node->left);
    populateArray(array, index*2+2, node->right);
}

/*Node** RedBlackTree::find(Node *& node, int num){
    //Returns pointer to the (parent) pointer to the
    //first node with value == num.
    if(node == 0){
        return 0;
    }
    if(node->value == num){
        Node** ptr = &node;
        return ptr;
    }
    else if(node->value < num){
        return find(node->right, num);
    }
    else{
        return find(node->left, num);
    }
}*/