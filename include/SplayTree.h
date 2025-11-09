//
// Created by Christoph Greger on 07.11.25.
//

#ifndef SPLAY_TREES_SPLAYTREE_H
#define SPLAY_TREES_SPLAYTREE_H

struct Node {
    Node* left;
    Node* right;
    Node* parent;
    int   key;
    int   id;
    Node(int key, int id, Node* parent=nullptr, Node* left=nullptr, Node* right=nullptr);
    ~Node() = default;
};

class SplayTree {
private:
    Node* root;

    static bool less_pair(int k1, int i1, int k2, int i2) {
        return (k1 < k2) || (!(k2 < k1) && i1 < i2); // (key,id) lexicographic, for shorter use in code
    }

    Node* subtreeMin(Node* n);         // leftmost in subtree
    Node* subtreeMax(Node* n);           // rightmost in subtree
    Node* findNode(int key, int id);     // nullptr if not found

    void rotateLeft(Node* x);
    void rotateRight(Node* x);
    void zig(Node* x);
    void zigzig(Node* x);
    void zigzag(Node* x);
    void splay(Node* x);

    void removeNode(Node* p);

public:
    SplayTree();
    ~SplayTree();

    void insert(int key, int id);
    bool contains(int key, int id);
    Node* findSmallestByKey(int key);
    bool remove(int key, int id);
    bool removeSmallest(int key);
};


#endif //SPLAY_TREES_SPLAYTREE_H