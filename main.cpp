#include <iostream>
using  namespace std;

template<typename T, int order>
class BTree{
private:
struct Node{
    T keys[order];
    Node* childern[order+1] ;
    int keyCount ;
    Node* parent ;
    Node():keyCount(0) , parent(nullptr){
        for(int i = 0; i<order+1;i++){
            childern[i] = nullptr;
        }
    }
    int inesrtKey(T key){
        for(int i = 0; i< keyCount;i++){
            if(key<keys[i]){
                for(int j =keyCount;j>i; j-- ){
                    keys[j] = keys[j-1];
                    childern[j+1] = childern[j];
                }
                keys[i] = key;
                keyCount++;
                return i;
            }
        }
        keys[keyCount] = key;
        keyCount++;
        return keyCount-1;
    }
};
Node* root;

Node* GetTheSuitableLeaf(T key, Node* node){
    if(node->childern[0] == nullptr){
        return node;
    }
    for(int i = 0; i< node->keyCount ; i++){
        if(key < node->keys[i]){
            return GetTheSuitableLeaf(key, node->childern[i]);
        }
    }
    return GetTheSuitableLeaf(key,node->childern[node->keyCount]);
}

void SplitNode(Node* node){
    if(node->keyCount< order)return;
    Node* newChild = new Node();
    if(node->parent == nullptr){
        root  = new Node();
        node->parent = root;
    }
    newChild->parent = node->parent;
    int midIndex =  (node->keyCount-1)/2;
    int i = node->parent->inesrtKey(node->keys[midIndex]);
    node->parent->childern[i] = node;
    node->parent->childern[i+1] = newChild;

    for(int j= midIndex+1; j<order; j++){
        newChild->inesrtKey(node->keys[j]);
        newChild->childern[j-midIndex-1] = node->childern[j];
        if(node->childern[j] != nullptr){
            node->childern[j]->parent = newChild;
        }
    }

    newChild->childern[order-midIndex-1] = node->childern[order];
    if(node->childern[order] != nullptr){
        node->childern[order]->parent = newChild;
    }

    node->keyCount = midIndex;
    SplitNode(node->parent);
}


public:
    BTree(): root(nullptr){}

    void Insert(T key){
        /*
         * 1- if there is no root then make the root and add the key to it's first element in the array
         * 2- if the root has a children check if the key is bigger than the last element or less than first element
         *      - go to the child node and make the same check if it has a children:
         *          yes: check the last and less and go to them and repeat
         *          no: check the key count if the node is full split and add the key to the parent and check if the parent full and so on
        */
        if(root == nullptr){
            root = new Node();
            root->keys[0] = key;
            root->keyCount++;
            return;
        }
        Node* leaf = GetTheSuitableLeaf(key, root);
        leaf->inesrtKey(key);
        SplitNode(leaf);
    }
    void Print(){
        printNode(root, "");
    }
    void printNode(Node* node, string strat){

        if(node == nullptr)return;

        cout<< strat<< node->keys[0];

        for(int i =1 ; i< node->keyCount; i++){
            cout<< "," <<node->keys[i];
        }
        cout<<"\n";
        for(int i = 0 ; i<node->keyCount+1; i++){
            printNode(node->childern[i], strat +"  ");
        }
    }
};


int main() {
    // Construct a BTree of order 3, which stores int data
    BTree<int,3> t1;

    t1.Insert(1);
    t1.Insert(5);
    t1.Insert(0);
    t1.Insert(4);
    t1.Insert(3);
    t1.Insert(2);

    t1.Print(); // Should output the following on the screen:

    /*
    1,4
      0
      2,3
      5
    */

    // Construct a BTree of order 5, which stores char data
    BTree<char,5> t;

    // Look at the example in our lecture:
    t.Insert('G');
    t.Insert('I');
    t.Insert('B');
    t.Insert('J');
    t.Insert('C');
    t.Insert('A');
    t.Insert('K');
    t.Insert('E');
    t.Insert('D');
    t.Insert('S');
    t.Insert('T');
    t.Insert('R');
    t.Insert('L');
    t.Insert('F');
    t.Insert('H');
    t.Insert('M');
    t.Insert('N');
    t.Insert('P');
    t.Insert('Q');

    t.Print(); // Should output the following on the screen:

    /*
    K
      C,G
        A,B
        D,E,F
        H,I,J
      N,R
        L,M
        P,Q
        S,T
    */

    return 0;
}
