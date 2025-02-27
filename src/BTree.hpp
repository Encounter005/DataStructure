#pragma once

#include <iostream>
#include <vector>

template<typename T> struct BTreeNode {
    std::vector<T>             keys;
    std::vector<BTreeNode<T>*> children;
    bool                       leaf;

    BTreeNode(bool leaf1) : leaf(leaf1) {}
};


template<typename T> class BTree {
public:
    BTree(int degree) : root(nullptr), Minimum_degree(degree) {}

    void insert(const T& key) {
        if (root == nullptr) {
            root = new BTreeNode<T>(true);
            root->keys.push_back(key);
        } else {
            if (root->keys.size() == 2 * Minimum_degree - 1) {
                BTreeNode<T>* newRoot = new BTreeNode<T>(false);
                newRoot->children.push_back(root);
                splitChild(newRoot, 0, root);
                root = newRoot;
            }
            insertNonFull(root, key);
        }
    }


    BTreeNode<T>* search(const T& key) {
        return (root == nullptr) ? nullptr : search(root, key);
    }

    void traverse() {
        if (root != nullptr) {
            traverse(root);
        }
        std::cout << std::endl;
    }

private:
    BTreeNode<T>* root;
    int           Minimum_degree;
    BTreeNode<T>* search(BTreeNode<T>* node, const T& key) {
        int index = 0;
        while (index < node->keys.size() && key > node->keys[index]) index++;

        if (index < node->keys.size() && key == node->keys[index]) {
            return node;
        }

        if (node->leaf) {
            return nullptr;
        }

        return search(node->children[index], key);
    }

    void insertNonFull(BTreeNode<T>* node, const T& key) {
        int i = node->keys.size() - 1;
        if (node->leaf) {
            node->keys.push_back(T());   // Make space for the new key
            while (i >= 0 && key < node->keys[i]) {
                node->keys[i + 1] = node->keys[i];
                i--;
            }
            node->keys[i + 1] = key;
        } else {
            while (i >= 0 && key < node->keys[i]) {
                i--;
            }
            i++;
            if (node->children[i]->keys.size() == 2 * Minimum_degree - 1) {
                splitChild(node, i, node->children[i]);
                if (key > node->keys[i]) {
                    i++;
                }
            }
            insertNonFull(node->children[i], key);
        }
    }
    void splitChild(BTreeNode<T>* parent, int index, BTreeNode<T>* child) {
        BTreeNode<T>* newNode = new BTreeNode<T>(child->leaf);
        newNode->keys.resize(Minimum_degree - 1);
        child->keys.resize(Minimum_degree - 1);

        for (int i = 0; i < Minimum_degree - 1; i++) {
            newNode->keys[i] = child->keys[i + Minimum_degree];
        }

        if (!child->leaf) {
            newNode->children.resize(Minimum_degree);
            for (int i = 0; i < Minimum_degree - 1; i++) {
                newNode->keys[i] = child->keys[i + Minimum_degree];
            }

            child->children.resize(Minimum_degree);
        }

        parent->children.insert(parent->children.begin() + index + 1, newNode);
        parent->keys.insert(
            parent->keys.begin() + index, child->keys[Minimum_degree - 1]);
    }

    void traverse(BTreeNode<T>* node) {
        int i = 0;
        for (i = 0; i < node->keys.size(); i++) {
            if (!node->leaf) {
                traverse(node->children[i]);
            }
            std::cout << " " << node->keys[i];
        }

        if (!node->leaf) {
            traverse(node->children[i]);
        }
    }
};
