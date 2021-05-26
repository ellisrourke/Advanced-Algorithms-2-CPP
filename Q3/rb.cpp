#include <iostream>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <bits/stdc++.h>
#include <chrono>

using namespace std;
typedef long long lld;
typedef unsigned long long llu;
using namespace std;

struct Node {
    int data; // holds the key
    Node *parent; // pointer to the parent
    Node *left; // pointer to left child
    Node *right; // pointer to right child
    int color; // 1 -> Red, 0 -> Black
};
typedef Node *NodePtr;
// class RBTree implements the operations in Red Black Tree
class RBTree {
private:
    NodePtr root;
    NodePtr TNULL;

    // initializes the nodes with appropirate values
    // all the pointers are set to point to the null pointer
    void initializeNULLNode(NodePtr node, NodePtr parent) {
        node->data = 0;
        node->parent = parent;
        node->left = nullptr;
        node->right = nullptr;
        node->color = 0;
    }

    void preOrderHelper(NodePtr node) {
        if (node != TNULL) {
            cout<<node->data<<" ";
            preOrderHelper(node->left);
            preOrderHelper(node->right);
        }
    }

    void inOrderHelper(NodePtr node) {
        if (node != TNULL) {
            inOrderHelper(node->left);
            cout<<node->data<<" ";
            inOrderHelper(node->right);
        }
    }

    void postOrderHelper(NodePtr node) {
        if (node != TNULL) {
            postOrderHelper(node->left);
            postOrderHelper(node->right);
            cout<<node->data<<" ";
        }
    }

    NodePtr searchTreeHelper(NodePtr node, int key) {
        if (node == TNULL || key == node->data) {
            return node;
        }

        if (key < node->data) {
            return searchTreeHelper(node->left, key);
        }
        return searchTreeHelper(node->right, key);
    }

    // fix the rb tree modified by the delete operation
    void fixDelete(NodePtr x) {
        NodePtr s;
        while (x != root && x->color == 0) {
            if (x == x->parent->left) {
                s = x->parent->right;
                if (s->color == 1) {
                    // case 3.1
                    s->color = 0;
                    x->parent->color = 1;
                    leftRotate(x->parent);
                    s = x->parent->right;
                }

                if (s->left->color == 0 && s->right->color == 0) {
                    // case 3.2
                    s->color = 1;
                    x = x->parent;
                } else {
                    if (s->right->color == 0) {
                        // case 3.3
                        s->left->color = 0;
                        s->color = 1;
                        rightRotate(s);
                        s = x->parent->right;
                    }

                    // case 3.4
                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->right->color = 0;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                s = x->parent->left;
                if (s->color == 1) {
                    // case 3.1
                    s->color = 0;
                    x->parent->color = 1;
                    rightRotate(x->parent);
                    s = x->parent->left;
                }

                if (s->right->color == 0 && s->right->color == 0) {
                    // case 3.2
                    s->color = 1;
                    x = x->parent;
                } else {
                    if (s->left->color == 0) {
                        // case 3.3
                        s->right->color = 0;
                        s->color = 1;
                        leftRotate(s);
                        s = x->parent->left;
                    }

                    // case 3.4
                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->left->color = 0;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = 0;
    }


    void rbTransplant(NodePtr u, NodePtr v){
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left){
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void deleteNodeHelper(NodePtr node, int key) {
        // find the node containing key
        NodePtr z = TNULL;
        NodePtr x, y;
        while (node != TNULL){
            if (node->data == key) {
                z = node;
            }

            if (node->data <= key) {
                node = node->right;
            } else {
                node = node->left;
            }
        }

        if (z == TNULL) {
            cout<<"Couldn't find key in the tree"<<endl;
            return;
        }

        y = z;
        int y_original_color = y->color;
        if (z->left == TNULL) {
            x = z->right;
            rbTransplant(z, z->right);
        } else if (z->right == TNULL) {
            x = z->left;
            rbTransplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            } else {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (y_original_color == 0){
            fixDelete(x);
        }
    }

    // fix the red-black tree
    void fixInsert(NodePtr k){
        NodePtr u;
        while (k->parent->color == 1) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left; // uncle
                if (u->color == 1) {
                    // case 3.1
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        // case 3.2.2
                        k = k->parent;
                        rightRotate(k);
                    }
                    // case 3.2.1
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    leftRotate(k->parent->parent);
                }
            } else {
                u = k->parent->parent->right; // uncle

                if (u->color == 1) {
                    // mirror case 3.1
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        // mirror case 3.2.2
                        k = k->parent;
                        leftRotate(k);
                    }
                    // mirror case 3.2.1
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) {
                break;
            }
        }
        root->color = 0;
    }

    void printHelper(NodePtr root, string indent, bool last) {
        // print the tree structure on the screen
        if (root != TNULL) {
            cout<<indent;
            if (last) {
                cout<<"R----";
                indent += "     ";
            } else {
                cout<<"L----";
                indent += "|    ";
            }

            string sColor = root->color?"RED":"BLACK";
            cout<<root->data<<"("<<sColor<<")"<<endl;
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
        // cout<<root->left->data<<endl;
    }

public:
    RBTree() {
        TNULL = new Node;
        TNULL->color = 0;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        root = TNULL;
    }

    // Pre-Order traversal
    // Node->Left Subtree->Right Subtree
    void preorder() {
        preOrderHelper(this->root);
    }

    // In-Order traversal
    // Left Subtree -> Node -> Right Subtree
    void inorder() {
        inOrderHelper(this->root);
    }

    // Post-Order traversal
    // Left Subtree -> Right Subtree -> Node
    void postorder() {
        postOrderHelper(this->root);
    }

    // search the tree for the key k
    // and return the corresponding node
    NodePtr searchTree(int k) {
        return searchTreeHelper(this->root, k);
    }

    // find the node with the minimum key
    NodePtr minimum(NodePtr node) {
        while (node->left != TNULL) {
            node = node->left;
        }
        return node;
    }

    // find the node with the maximum key
    NodePtr maximum(NodePtr node) {
        while (node->right != TNULL) {
            node = node->right;
        }
        return node;
    }

    // find the successor of a given node
    NodePtr successor(NodePtr x) {
        // if the right subtree is not null,
        // the successor is the leftmost node in the
        // right subtree
        if (x->right != TNULL) {
            return minimum(x->right);
        }

        // else it is the lowest ancestor of x whose
        // left child is also an ancestor of x.
        NodePtr y = x->parent;
        while (y != TNULL && x == y->right) {
            x = y;
            y = y->parent;
        }
        return y;
    }

    // find the predecessor of a given node
    NodePtr predecessor(NodePtr x) {
        // if the left subtree is not null,
        // the predecessor is the rightmost node in the
        // left subtree
        if (x->left != TNULL) {
            return maximum(x->left);
        }

        NodePtr y = x->parent;
        while (y != TNULL && x == y->left) {
            x = y;
            y = y->parent;
        }

        return y;
    }

    // rotate left at node x
    void leftRotate(NodePtr x) {
        NodePtr y = x->right;
        x->right = y->left;
        if (y->left != TNULL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    // rotate right at node x
    void rightRotate(NodePtr x) {
        NodePtr y = x->left;
        x->left = y->right;
        if (y->right != TNULL) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    // insert the key to the tree in its appropriate position
    // and fix the tree
    void insert(int key) {
        // Ordinary Binary Search Insertion
        NodePtr node = new Node;
        node->parent = nullptr;
        node->data = key;
        node->left = TNULL;
        node->right = TNULL;
        node->color = 1; // new node must be red

        NodePtr y = nullptr;
        NodePtr x = this->root;

        while (x != TNULL) {
            y = x;
            if (node->data < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        // y is parent of x
        node->parent = y;
        if (y == nullptr) {
            root = node;
        } else if (node->data < y->data) {
            y->left = node;
        } else {
            y->right = node;
        }

        // if new node is a root node, simply return
        if (node->parent == nullptr){
            node->color = 0;
            return;
        }

        // if the grandparent is null, simply return
        if (node->parent->parent == nullptr) {
            return;
        }

        // Fix the tree
        fixInsert(node);
    }

    NodePtr getRoot(){
        return this->root;
    }

    // delete the node from the tree
    void deleteNode(int data) {
        deleteNodeHelper(this->root, data);
    }

    // print the tree structure on the screen
    void prettyPrint() {
        if (root) {
            printHelper(this->root, "", true);
        }
    }
};

////////////////////////////////////////////////////////////////////////////////////
/*
 Petar 'PetarV' Velickovic
 Data Structure: van Emde Boas Tree (vEB)
*/

/*
 van Emde Boas Tree structure for holding a set of integers in a given universe.
 Complexity:    O(1) for min, max and first
                O(log log u) for member, insert, delete, pred, succ, extractMin and decreaseKey
*/

class vEB
{
    int u;
    int min;
    int max;
    vEB *summary;
    std::vector<vEB *> children;

    int high(int k){
        int x = std::ceil(std::sqrt(u));
        return std::floor(k / x);
    }

    int low(int k){
        int x = std::ceil(std::sqrt(u));
        return k % x;
    }

    int index(int k, int kk){
        return (k * (int)std::sqrt(u)) + kk;
    }

public:

    vEB(int U){
        // u = std::pow(std::sqrt(U), 2);
        u = U;
        min = U;
        max = -1;

        if (u <= 2){
            summary = nullptr;
            children = std::vector<vEB *> (0, nullptr);
        }
        else{
            int children_count = std::ceil(std::sqrt(u));
            int children_size = std::ceil(u / std::sqrt(u));

            summary = new vEB(children_count);
            children = std::vector<vEB *> (children_count, nullptr);
            for(int i = 0; i < children_count; ++i){
                children[i] = new vEB(children_count);
            }
        }
    }

    void insert(int k){
        if(min > max){
            min = max = k;
            return;
        }

        if(k < min){
            int temp;
            temp = min;
            min = k;
            k = temp;
        }

        if(k > max)
            max = k;

        if(u == 2){
            max = k;
            return;
        }

        int i = high(k);
        int j = low(k);

        children[i]->insert(j);

        if(children[i]->max == children[i]->min)
            summary->insert(i);
    }

    void remove(int k){
        if(min > max)
            return;

        if(min == max){
            min = u;
            max = -1;
            return;
        }

        if(u == 2){
            if(k == 1){
                if(min == 1){
                    min = u;
                    max = -1;
                }
                else if(min == 0)
                    max = 0;
            }
            else
            if(max == 0){
                min = u;
                max = -1;
            }
            else if(max == 1)
                min = 1;
            return;
        }

        if(k == min){
            int i = summary->min;
            min = index(i, children[i]->min);
            return;
        }

        int i = high(k);
        int j = low(k);

        children[i]->remove(j);

        if(children[i]->min > children[i]->max){
            summary->remove(i);
        }

        if(k == max){
            if(summary->min > summary->max){
                max = min;
            }
            else{
                i = summary->min;
                max = index(i, children[i]->max);
            }
        }
    }

    int getMin(){
        return min;
    }
    int getMax(){
        return max;
    }

    int universe(){
        return u;
    }

    int successor(int k){
        if(k <= min)
            return min;
        else if(k > max)
            return u;

        int i = high(k);
        int j = low(k);

        if(j <= children[i]->max){
            int res = children[i]->successor(j);
            if(res >= (u / (int)std::sqrt(u)))
                return u;
            return k - j + res;
        }
        else{
            int res = children[summary->successor(i)]->min;
            if(res >= children[summary->min]->u)
                return u;
            return index(summary->successor(i), res);
        }
    }
};


int main() {
    int n = 600000;
    srand(time(0));
    int nums[n];

    for(int i = 0; i < n; i++){
        nums[i] = i;
    };

    RBTree bst;
    auto begin = std::chrono::high_resolution_clock::now();
    for(int i=0; i < n; i++){ bst.insert(nums[i]); }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cout << "RB Insert : " << fixed << elapsed.count() * 1e-9 << setprecision(5) << " sec " << endl;


    begin = std::chrono::high_resolution_clock::now();
    for(int i=0; i < n; i++){ bst.searchTree(nums[i]); }
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cout << "RB Search : " << fixed << elapsed.count() * 1e-9 << setprecision(5) << " sec " << endl;

    begin = std::chrono::high_resolution_clock::now();
    for(int i=0; i < n; i++){ bst.deleteNode(nums[i]); }
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cout << "RB Delete : " << fixed << elapsed.count() * 1e-9 << setprecision(5) << " sec " << endl;

    vEB tree(n);
    begin = std::chrono::high_resolution_clock::now();
    for(int i=0; i < n; i++){ tree.insert(nums[i]); }
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cout << "VEB Insert : " << fixed << elapsed.count() * 1e-9 << setprecision(5) << " sec " << endl;

    begin = std::chrono::high_resolution_clock::now();
    for(int i=0; i < n; i++){ tree.remove(nums[i]); }
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cout << "VEB Remove : " << fixed << elapsed.count() * 1e-9 << setprecision(5) << " sec " << endl;

    return 0;
};
