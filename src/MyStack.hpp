#pragma once

#include "MyList.hpp"

template<typename T> class MyStack {
public:
    MyStack() = default;
    explicit MyStack(const std::initializer_list<T> &items) { list_(items); }
    MyStack(const MyStack &other) { *this = other; }
    MyStack(MyStack &&other) { *this = std::move(other); }

    MyStack &operator=(const MyStack &other) {
        if (this != &other) {
            this->list_ = other.list_;
        }
        return *this;
    }

    MyStack &operator=(MyStack &&other) {
        if (this != &other) {
            this->list_.swap(other.list_);
        }
        return *this;
    }

    bool empty() const { return list_.empty(); }

    size_t size() const { return list_.size(); }

    void push(const T &value) { list_.push_front(value); }

    void pop() { list_.pop_front(); }

    template<typename... Args> void emplace(Args &&...args) {
        list_.emplace_front(std::forward<Args>(args)...);
    }

    void swap(MyStack &other) {
        if (this == &other) return;
        this->list_.Swap(other.list_);
    }

    T &top() const { return list_.front(); }

private:
    MyList<T> list_;
};
