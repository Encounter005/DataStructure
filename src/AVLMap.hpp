#pragma once
#include <functional>
#include <iostream>
#include <vector>

template<typename Key, typename Value>
struct AVLNode {
    Key      key;
    Value    value;
    int      height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(const Key& key, const Value& value)
        : key(key), value(value), height(1), left(nullptr), right(nullptr) {}
};

template<typename Key, typename Value>
class AVLMap {
    using Node = AVLNode<Key, Value>;

public:
    AVLMap() : root(nullptr) {}
    AVLMap(const AVLMap& other) = delete;
    AVLMap(AVLMap&& other) = delete;
    AVLMap& operator=(const AVLMap& other) = delete;
    AVLMap& operator=(AVLMap&& other) = delete;

    void put(const Key& key, const Value& value) {
        root = insertNode(root, key, value);
    }

    Value* get(const Key& key) const {
        return searchNode(root, key);
    }

    void remove(const Key& key) {
        root = deleteNode(root, key);
    }

    std::vector<std::pair<Key, Value>> inorder() const {
        std::vector<std::pair<Key, Value>> res;
        inorderHelper(root, res);
        return res;
    }

    ~AVLMap() {
        destroyTree(root);
    }

private:
    Node* root;

    Node* getMinimumNode(Node* node) const {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    void inorderHelper(Node* node, std::vector<std::pair<Key, Value>>& res) const {
        if (!node) return;
        inorderHelper(node->left, res);
        res.emplace_back(node->key, node->value);
        inorderHelper(node->right, res);
    }

    Node* balanceInsert(Node* node, const Key& key) {
        int balance = getBalance(node);

        // Left Left (LL)
        if (balance > 1 && key < node->left->key) {
            return rightRotate(node);
        }
        // Right Right (RR)
        if (balance < -1 && key > node->right->key) {
            return leftRotate(node);
        }
        // Left Right (LR)
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        // Right Left (RL)
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    Node* balanceDelete(Node* node) {
        int balance = getBalance(node);

        // Left Left (LL)
        if (balance > 1 && getBalance(node->left) >= 0) {
            return rightRotate(node);
        }
        // Right Right (RR)
        if (balance < -1 && getBalance(node->right) <= 0) {
            return leftRotate(node);
        }
        // Left Right (LR)
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        // Right Left (RL)
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    int getHeight(Node* node) const {
        return node ? node->height : 0;
    }

    int getBalance(Node* node) const {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }


/*
 *
                    y                             x
                   / \                           / \
                  x   T3           ==>          z   y
                 / \                               / \
                z   T2                            T2  T3
 *
 *
 *
 * */


    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }


/*
                        x                             y
                       / \                           / \
                      T1  y          ==>            x   z
                         / \                       / \
                        T2  z                     T1  T2
 *
 * */

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    Node* insertNode(Node* node, const Key& key, const Value& value) {
        if (!node) {
            return new Node(key, value);
        }

        if (key < node->key) {
            node->left = insertNode(node->left, key, value);
        } else if (key > node->key) {
            node->right = insertNode(node->right, key, value);
        } else {
            node->value = value; // Update value if key exists
            return node;
        }

        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        return balanceInsert(node, key);
    }

    Value* searchNode(Node* node, const Key& key) const {
        if (!node) {
            return nullptr;
        }

        if (key < node->key) {
            return searchNode(node->left, key);
        } else if (key > node->key) {
            return searchNode(node->right, key);
        } else {
            return &(node->value);
        }
    }

    Node* deleteNode(Node* node, const Key& key) {
        if (!node) return node;

        if (key < node->key) {
            node->left = deleteNode(node->left, key);
        } else if (key > node->key) {
            node->right = deleteNode(node->right, key);
        } else {
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                if (!temp) {
                    delete node;
                    return nullptr;
                } else {
                    *node = *temp;
                    delete temp;
                }
            } else {
                Node* temp = getMinimumNode(node->right);
                node->key = temp->key;
                node->value = temp->value;
                node->right = deleteNode(node->right, temp->key);
            }
        }

        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        return balanceDelete(node);
    }

    void destroyTree(Node* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
};
