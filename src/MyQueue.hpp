#pragma once

#include "MyList.hpp"

template <typename T> class MyQueue {
public:
    MyQueue() = default;

    explicit MyQueue( const std::initializer_list<T> &items ) : list_(items) {}
    MyQueue( const MyQueue &other ) { *this = other; }
    MyQueue( MyQueue &&other ) { *this = std::move( other ); }
    ~MyQueue() = default;

    MyQueue &operator=( const MyQueue &other ) {
        if ( this != &other ) {
            this->list_ = other.list_;
        }
        return *this;
    }

    MyQueue &operator=( MyQueue &&other ) {
        if ( this != &other ) {
            swap( other );
        }
        return *this;
    }

    void swap( MyQueue &other ) {
        if ( this == &other ) {
            return;
        }
        this->list_.swap( other.list_ );
    }

    void push( const T &value ) { list_.push_back( value ); }
    void pop() { list_.pop_front(); }

    template <typename... Args> void emplace( Args &&...args ) {
        list_.emplace_back( std::forward<Args>( args )... );
    }

    bool empty() const { return list_.empty(); }

    size_t size() const { return list_.size(); }

    T &front() const { return list_.front(); }

private:
    MyList<T> list_;
};
