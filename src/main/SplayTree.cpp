//
// Created by Christoph Greger on 07.11.25.
//

#include "SplayTree.h"
#include <vector>

Node::Node(int key, int id, Node* parent, Node* left, Node* right)
    : left(left), right(right), parent(parent), key(key), id(id) {}

SplayTree::SplayTree() : root(nullptr) {}

SplayTree::~SplayTree() {

    // Iterative destructor. As depth can be O(n), recursion may lead to stack overflow.
    if (!root) return;
    std::vector<Node*> st;
    Node* curr = root;
    Node* last = nullptr;
    while (curr || !st.empty()) {
        if (curr) {
            st.push_back(curr);
            curr = curr->left;
        } else {
            Node* node = st.back();
            if (node->right && last != node->right) {
                //If right subtree not yet processed
                curr = node->right;
            } else {
                // If no right subtree or already processed
                st.pop_back();
                last = node;
                delete node;
            }
        }
    }
    root = nullptr;
}

Node* SplayTree::subtreeMin(Node* n) {
    if (!n) {
        return nullptr;
    }
    while (n->left) {
        n = n->left;
    }
    return n;
}

Node* SplayTree::subtreeMax(Node* n) {
    if (!n) {
        return nullptr;
    }
    while (n->right) {
        n = n->right;
    }
    return n;
}

Node* SplayTree::findNode(int key, int id) {
    Node* x = root;
    while (x) {
        if (SplayTree::less_pair(key, id, x->key, x->id)) {
            x = x->left;
        } else if (SplayTree::less_pair(x->key, x->id, key, id)) {
            x = x->right;
        } else {
            return x;
        }
    }
    return nullptr;
}

void SplayTree::rotateLeft(Node* x) {
    Node* y = x->right;
    if (!y) {
        return;
    }

    x->right = y->left;
    if (y->left) {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (!x->parent) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void SplayTree::rotateRight(Node* x) {
    Node* y = x->left;
    if (!y) {
        return;
    }

    x->left = y->right;
    if (y->right) {
        y->right->parent = x;
    }

    y->parent = x->parent;
    if (!x->parent) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->right = x;
    x->parent = y;
}

void SplayTree::zig(Node* x) {
    if (!x->parent) {
        return;
    }
    if (x->parent->left == x) {
        rotateRight(x->parent);
    } else {
        rotateLeft(x->parent);
    }
}

void SplayTree::zigzig(Node* x) {
    Node* p = x->parent;
    Node* g = p->parent;
    if (!g) {
        return;
    }
    if (p->left == x && g->left == p) {
        rotateRight(g);
        rotateRight(p);
    } else if (p->right == x && g->right == p) {
        rotateLeft(g);
        rotateLeft(p);
    }
}

void SplayTree::zigzag(Node* x) {
    Node* p = x->parent;
    Node* g = p->parent;
    if (!g) {
        return;
    }

    if (p->left == x && g->right == p) {
        rotateRight(p);
        rotateLeft(g);
    } else if (p->right == x && g->left == p) {
        rotateLeft(p);
        rotateRight(g);
    }
}

void SplayTree::splay(Node* x) {
    if (!x) {
        return;
    }

    while (x->parent) {
        if (!x->parent->parent) {
            zig(x);
        } else if ((x->parent->left == x && x->parent->parent->left == x->parent) ||
                   (x->parent->right == x && x->parent->parent->right == x->parent)) {
            zigzig(x);
        } else {
            zigzag(x);
        }
    }
}

void SplayTree::insert(int key, int id) {
    if (!root) {
        root = new Node(key, id);
        return;
    }

    Node* x = root;
    Node* p = nullptr;

    while (x) {
        p = x;
        if (SplayTree::less_pair(key, id, x->key, x->id)) {
            x = x->left;
        } else if (SplayTree::less_pair(x->key, x->id, key, id)) {
            x = x->right;
        } else {
            //I Decided: duplicate, do not insert, but splay this node
            splay(x);
            return;
        }
    }

    Node* n = new Node(key, id, p);
    if (SplayTree::less_pair(key, id, p->key, p->id)) {
        p->left = n;
    } else {
        p->right = n;
    }
    splay(n);
}

bool SplayTree::contains(int key, int id) {
    Node* f = findNode(key, id);
    if (f) {
        splay(f);
        return true;
    }
    return false;
}

Node* SplayTree::findSmallestByKey(int key) {
    Node* x = root;
    Node* res = nullptr;

    while (x) {
        if (key < x->key) {
            x = x->left;
        } else if (key > x->key) {
            x = x->right;
        } else {
            res = x;
            x = x->left;
        }
    }

    if (res) {
        splay(res);
    }
    return res;
}


void SplayTree::removeNode(Node* t) {
    if (!t) {
        return;
    }

    splay(t);

    Node* L = t->left;
    Node* R = t->right;

    if (L) {
        L->parent = nullptr;
    }
    if (R) {
        R->parent = nullptr;
    }

    delete t;
    root = nullptr;

    if (!L) {
        root = R;
        return;
    }

    Node* m = subtreeMax(L);
    splay(m);
    m->right = R;

    if (R) {
        R->parent = m;
    }

    root = m;
}

bool SplayTree::remove(int key, int id) {
    Node* t = findNode(key, id);
    if (!t) {
        return false;
    }
    removeNode(t);
    return true;
}

bool SplayTree::removeSmallest(int key) {
    Node* t = findSmallestByKey(key);
    if (!t || t->key != key) {
        return false;
    }
    removeNode(t);
    return true;
}