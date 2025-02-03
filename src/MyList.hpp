#pragma once
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <stdexcept>

template<typename T> class List {
public:
    List() : num_items_(0) {
        puse_ = std::shared_ptr<Node>(new Node(0));
        head_ = puse_;
    }

    explicit List(const std::initializer_list<T> &ilist) : List() {
        if (ilist.size()) {
            for (const auto &elm : ilist) {
                push_back(elm);
            }
        }
    }

    explicit List(size_t n) : List() {
        while (n--) {
            push_back(0);
        }
    }

    List(const List &other) { *this = other; }

    List &operator=(const List &other) {
        if (this == &other) {
            return *this;
        }
        clear();
        for (const auto &elm : other) {
            push_back(elm);
        }

        return *this;
    }

    List(List &&other) noexcept { *this = std::move(other); }

    List &operator=(List &&other) noexcept {
        if (this == &other) {
            return *this;
        }
        swap(other);
        return *this;
    }

    ~List() { clear(); }

    void push_back(const T &elm) {
        auto new_node = std::shared_ptr<Node>(new Node(elm));
        BindNodes(new_node, puse_);
        if (tail_ == nullptr) {
            head_ = new_node;
        } else {
            BindNodes(tail_, new_node);
        }

        tail_ = new_node;
        ++num_items_;
    }

    void pop_back() {
        if (tail_ != nullptr) {
            if (tail_->lst_ != nullptr) {
                tail_ = tail_->lst_;
                tail_->nxt_.reset();
                BindNodes(tail_, puse_);
            } else {
                tail_.reset();
                head_ = tail_ = nullptr;
            }
            --num_items_;
        }
    }

    void push_front(const T &item) {
        auto new_node = std::shared_ptr<Node>(new Node(item));
        BindNodes(new_node, head_);
        head_ = new_node;
        if (tail_ == nullptr) {
            BindNodes(head_, puse_);
            tail_ = new_node;
        }
        ++num_items_;
    }

    void pop_front() {
        if (head_ != nullptr) {
            if (head_->nxt_ != nullptr) {
                head_ = head_->nxt_;
                head_->lst_.reset();
            } else {
                head_.reset();
                tail_.reset();
            }
            --num_items_;
        }
    }

    void swap(List &other) {
        if (this == &other) {
            return;
        }
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(puse_, other.puse_);
    }

    void reverse() {
        List tmp;
        for (const auto &elm : *this) {
            tmp.push_front(elm);
        }
        this->swap(tmp);
    }

    T &operator[](size_t pos) { return find(pos)->value_; }

    bool empty() const { return num_items_ == 0; }

    size_t size() const { return num_items_; }

    T &front() const {
        if (head_ != nullptr) {
            return head_->value_;
        }
        return puse_->value_;
    }

    T &back() const {
        if (tail_ != nullptr) {
            return tail_->value_;
        }
        return puse_->value_;
    }

    void clear() {
        while (tail_ != nullptr) {
            pop_back();
        }
    }

private:
    struct Node {
        std::shared_ptr<Node> lst_, nxt_;
        T                     value_;
        template<typename U>
        Node(U &&item) : value_(item), lst_(nullptr), nxt_(nullptr) {}
    };
    std::shared_ptr<Node> find(size_t pos) {
        try {
            if (pos >= num_items_) {
                throw std::runtime_error(
                    "The pos is larger than the number of items");
            }

            if (pos == 0) {
                return head_;
            }
            if (pos == num_items_ - 1) {
                return tail_;
            }

            auto DisToTail = num_items_ - pos - 1;
            if (DisToTail < pos) {
                auto pNode = tail_;
                while (DisToTail--) {
                    pNode = pNode->lst_;
                }
                return pNode;
            } else {
                auto pNode = head_;
                while (pos--) {
                    pNode = pNode->nxt_;
                }
                return pNode;
            }

        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void BindNodes(std::shared_ptr<Node> first, std::shared_ptr<Node> second) {
        if (first != nullptr) {
            first->nxt_ = second;
        }

        if (second != nullptr) {
            second->lst_ = first;
        }
    }

    class Iterator {
    public:
        Iterator() : ptr_(nullptr) {}
        explicit Iterator(std::shared_ptr<Node> ptr) : ptr_(ptr) {}
        Iterator(const Iterator &other) { this->ptr_ = other.ptr_; }
        Iterator(Iterator &&other) : ptr_(nullptr) {
            std::swap(other.ptr_, this->ptr_);
        }
        Iterator &operator=(const Iterator &other) {
            ptr_ = other.ptr_;
            return *this;
        }
        Iterator &operator=(Iterator &&other) {
            ptr_       = other.ptr_;
            other.ptr_ = nullptr;
            return *this;
        }
        Iterator &operator++() {
            ptr_ = ptr_->nxt_;
            return *this;
        }
        Iterator &operator--() {
            ptr_ = ptr_->lst_;
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp(*this);
            ++(*this);
            return *this;
        }
        Iterator operator--(int) {
            Iterator tmp(*this);
            --(*this);
            return *this;
        }

        ~Iterator() = default;
        bool operator==(const Iterator &other) { return ptr_ == other.ptr_; }
        bool operator!=(const Iterator &other) { return ptr_ != other.ptr_; }
        T   &operator*() const { return ptr_->value_; }
        std::shared_ptr<Node> ptr_;
    };

    class ConstIterator : public Iterator {
    public:
        explicit ConstIterator(std::shared_ptr<Node> ptr) : Iterator(ptr) {}
        const T &operator*() const { return this->ptr_->value_; }
    };

public:
    Iterator      begin() { return Iterator(head_); }
    Iterator      end() { return Iterator(puse_); }
    ConstIterator begin() const { return ConstIterator(head_); }
    ConstIterator end() const { return ConstIterator(puse_); }
    ConstIterator cbegin() const { return ConstIterator(head_); }
    ConstIterator cend() const { return ConstIterator(puse_); }
    template<typename U> Iterator insert(U &&elm, Iterator pos) {
        if (pos == begin()) {
            push_front(elm);
            return begin();
        }
        if (pos == end()) {
            push_back(elm);
            return --end();
        }
        auto new_node = std::shared_ptr<Node>(new Node(elm));
        BindNodes(pos.ptr_->lst_, new_node);
        BindNodes(new_node, pos.ptr_);
        ++num_items_;
        return --pos;
    }

    template<typename... Args>
    Iterator emplace(ConstIterator pos, Args &&...args) {
        T args_data[sizeof...(Args)]{args...};
        for (size_t i = 0; i < sizeof...(Args); ++i) {
            insert(args_data[i], pos);
        }
        return pos;
    }

    template<typename... Args> void emplace_back(Args &&...args) {
        emplace(cend(), std::forward<Args>(args)...);
    }

    template<typename... Args> void emplace_front(Args &&...args) {
        emplace(cbegin(), std::forward<Args>(args)...);
    }

    void erase(Iterator pos) {
        if (pos == begin()) {
            pop_front();
        } else if (pos == --end()) {
            pop_back();
        } else {
            auto iter = pos.ptr_;
            BindNodes(iter->lst_, iter->nxt_);
            iter.reset();
            --num_items_;
        }
    }

private:
    size_t                num_items_ = 0;
    std::shared_ptr<Node> head_, tail_, puse_;
};
