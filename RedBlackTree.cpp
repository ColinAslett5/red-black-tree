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
    //Based on the description of the algorithm on Wikipedia's Red-black tree page.
    if(inserted->parent == 0){//inserted is the root
        inserted->paintBlack();
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
        return;
    }
    if(inserted->isRightChild() && inserted->parent->isLeftChild()){
        //Parent is red, uncle is black. Inserted is right child of parent,
        //parent is left child of grandparent.
        //Left rotation: move the inserted node into its parent's position:
        leftRotation(inserted);
        inserted = inserted->left;
        //Do not return.
    }
    else if(inserted->isLeftChild() && inserted->parent->isRightChild()){
        //"Reflection" of previous "zig-zag" case.
        rightRotation(inserted);
        inserted = inserted->right;
        //Again, do not return; what we're doing is getting rid
        //of the zig-zag by rotating for the next case if there is one;
        //we haven't taken care of preserving the tree property yet.
    }
    inserted->grandparent()->paintRed();
    inserted->parent->paintBlack();
    if(inserted->isLeftChild()){
        rightRotation(inserted->parent);
    }
    else{
        leftRotation(inserted->parent);
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
            if(nodes[index] > 0){
                cout << nodes[index] << 'B';
            }
            else if(nodes[index] < 0){
                cout << -nodes[index] << 'R';
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

Node* RedBlackTree::insertInitial(Node * child, int num){
    //Initial insertion as in binary search tree; node colored red.
    //Assigns parent and child pointers. Returns pointer to inserted node.
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

int RedBlackTree::getNumLevels(Node* currentRoot, int level = 0){
    if(currentRoot == 0){
        return level;
    }
    return max(getNumLevels(currentRoot->left, level + 1), getNumLevels(currentRoot->right, level + 1));
}

void RedBlackTree::populateArray(int *& array, int index, Node* node){
    if(node == 0){
        return;
    }
    array[index] = node->black ? node->value : -node->value;
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

void RedBlackTree::leftRotation(Node* formerChild){
    Node* formerParent = formerChild->parent;
    *parentPtrTo(formerParent) = formerChild;
    formerChild->parent = formerChild->grandparent();
    formerParent->setRight(formerChild->left);
    formerChild->setLeft(formerParent);
}

void RedBlackTree::rightRotation(Node* formerChild){
    Node* formerParent = formerChild->parent;
    *parentPtrTo(formerParent) = formerChild;
    formerChild->parent = formerChild->grandparent();
    formerParent->setLeft(formerChild->right);
    formerChild->setRight(formerParent);
}

Node** RedBlackTree::parentPtrTo(Node* child){
    Node** ptr;
    if(child->parent == 0){
        ptr = &root;
    }
    else if(child == child->parent->left){
        ptr = &(child->parent->left);
    }
    else{
        ptr = &(child->parent->right);
    }
    return ptr;
}