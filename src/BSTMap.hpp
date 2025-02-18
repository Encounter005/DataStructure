#pragma once
#include <iostream>

template<typename Key, typename T> struct TreeNode {
    std::pair<Key, T> data;
    TreeNode*         left;
    TreeNode*         right;
    TreeNode*         parent;


    TreeNode(
        const Key& key, const T& value, TreeNode* parent = nullptr,
        TreeNode* left = nullptr, TreeNode* right = nullptr)
        : data(std::make_pair(key, value))
        , left(left)   // 正确初始化
        , right(right)
        , parent(parent) {}
};


template<typename Key, typename T> class Map {
public:
    Map() : root(nullptr) {}
    ~Map() { clear(root); }

    // 禁止拷贝构造和赋值
    Map(const Map&)            = delete;
    Map& operator=(const Map&) = delete;

    // 插入或更新键值对
    void insert(const Key& key, const T& value) {
        if (root == nullptr) {
            root = new TreeNode<Key, T>(key, value);
            return;
        }

        TreeNode<Key, T>* current = root;
        TreeNode<Key, T>* parent  = nullptr;

        while (current != nullptr) {
            parent = current;
            if (key < current->data.first) {
                current = current->left;
            } else if (key > current->data.first) {
                current = current->right;
            } else {
                // 键已存在，更新值
                current->data.second = value;
                return;
            }
        }

        if (key < parent->data.first) {
            parent->left = new TreeNode<Key, T>(key, value, parent);
        } else {
            parent->right = new TreeNode<Key, T>(key, value, parent);
        }
    }

    // 查找元素，返回指向节点的指针
    TreeNode<Key, T>* find(const Key& key) const {
        TreeNode<Key, T>* current = root;
        while (current != nullptr) {
            if (key < current->data.first) {
                current = current->left;
            } else if (key > current->data.first) {
                current = current->right;
            } else {
                return current;
            }
        }
        return nullptr;
    }

    void erase(const Key& key) {
        TreeNode<Key, T>* node = find(key);
        if (node == nullptr) return;

        // Case 3: 节点有两个子节点
        if (node->left && node->right) {
            TreeNode<Key, T>* successor = minimum(node->right);
            node->data                  = successor->data;
            node                        = successor;   // 将待删除节点转移到后继
        }

        // 处理子节点（此时node最多有一个子节点）
        TreeNode<Key, T>* child = node->left ? node->left : node->right;

        // 更新子节点的父指针
        if (child) {
            child->parent = node->parent;
        }

        // 更新父节点的子指针
        if (node->parent == nullptr) {
            root = child;
        } else if (node == node->parent->left) {
            node->parent->left = child;
        } else {
            node->parent->right = child;
        }

        delete node;
    }
    // 清空所有节点
    void clear() {
        clear(root);
        root = nullptr;
    }

    // 获取迭代器
    class Iterator {
    public:
        Iterator(TreeNode<Key, T>* node = nullptr) : current(node) {}

        std::pair<Key, T>& operator*() const { return current->data; }

        std::pair<Key, T>* operator->() const { return &(current->data); }

        // 前置递增
        Iterator& operator++() {
            current = successor(current);
            return *this;
        }

        // 后置递增
        Iterator operator++(int) {
            Iterator temp = *this;
            current       = successor(current);
            return temp;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

    private:
        TreeNode<Key, T>* current;

        TreeNode<Key, T>* minimum(TreeNode<Key, T>* node) const {
            if (node == nullptr) return nullptr;
            while (node->left != nullptr) {
                node = node->left;
            }
            return node;
        }

        TreeNode<Key, T>* successor(TreeNode<Key, T>* node) const {
            if (node->right != nullptr) {
                return minimum(node->right);
            }

            TreeNode<Key, T>* p = node->parent;
            while (p != nullptr && node == p->right) {
                node = p;
                p    = p->parent;
            }
            return p;
        }
    };

    Iterator begin() const { return Iterator(minimum(root)); }

    Iterator end() const { return Iterator(nullptr); }

private:
    TreeNode<Key, T>* root;

    // 删除树中的所有节点
    void clear(TreeNode<Key, T>* node) {
        if (node == nullptr) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    // 找到最小的节点
    TreeNode<Key, T>* minimum(TreeNode<Key, T>* node) const {
        if (node == nullptr) return nullptr;
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    // 找到最大的节点
    TreeNode<Key, T>* maximum(TreeNode<Key, T>* node) const {
        if (node == nullptr) return nullptr;
        while (node->right != nullptr) {
            node = node->right;
        }
        return node;
    }
};
