#pragma once

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <stdexcept>

template<typename T, size_t SIZE = 0> class MyArray {
public:
    constexpr MyArray() {
        num_items = SIZE;
        if (num_items) {
            data_ = std::unique_ptr<T[]>(new T[num_items]);
        } else {
            data_ = nullptr;
        }
    }

    MyArray(const std::initializer_list<T> &ilist) {
        num_items = ilist.size();
        if (ilist.size()) {
            size_t idx = 0;
            data_      = std::unique_ptr<T[]>(new T[num_items]());
            std::copy(ilist.begin(), ilist.end(), data_.get());
        } else {
            data_ = nullptr;
        }
    }

    MyArray(const MyArray &other) {
        num_items = other.num_items;
        data_     = std::unique_ptr<T[]>(new T[num_items]());
        std::copy(other.begin(), other.end(), this->data_.get());
    }

    MyArray &operator=(const MyArray &other) {
        if (this == &other) {
            return *this;
        }

        if (!this->empty()) {
            clear();
        }

        num_items = other.num_items;
        data_.reset(new T[num_items]());
        std::copy(other.begin(), other.end(), this->data_.get());
        return *this;
    }


    MyArray &operator=(const std::initializer_list<T> &ilist) {
        num_items = ilist.size();
        if (num_items) {
            data_ = std::unique_ptr<T[]>(new T[num_items]);
            std::copy(ilist.begin(), ilist.end(), data_.get());
        } else {
            data_ = nullptr;
        }
        return *this;
    }

    MyArray(MyArray &&other) noexcept
        : num_items(other.num_items), data_(std::move(other.data_)) {
        other.num_items = 0;
    }

    MyArray &operator=(MyArray &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        num_items       = other.num_items;
        other.num_items = 0;
        data_.reset(other.data_.release());
        return *this;
    }

    ~MyArray() { clear(); }

    template<typename U> void insert(U &&element) {
        std::unique_ptr<T[]> new_data_(new T[num_items + 1]());
        for (size_t i = 0; i < num_items; i++) {
            new_data_[i] = data_[i];
        }
        new_data_[num_items] = element;
        num_items++;
        data_ = std::move(new_data_);
    }

    template<typename U> void insert(U &&element, size_t pos) {
        if (pos > num_items) {
            throw std::runtime_error("The index is larger than the size");
        }

        std::unique_ptr<T[]> new_data_(new T[num_items + 1]());
        for (size_t i = 0, j = 0; i < num_items; i++, j++) {
            if (i == pos) {
                new_data_[j++] = std::forward<U>(element);
            }
            new_data_[j] = data_[i];
        }

        if (pos == num_items) new_data_[num_items] = std::forward<U>(element);

        data_ = std::move(new_data_);
        ++num_items;
    }

    void remove(size_t pos) {
        if (pos >= num_items) {
            throw std::runtime_error("The index is larger than the size");
        }
        if (empty()) {
            return;
        }
        std::unique_ptr<T[]> new_data_(new T[num_items - 1]());
        for (size_t i = 0, j = 0; i < num_items; i++, j++) {
            if (i == pos) {
                j--;
                continue;
            }
            new_data_[j] = data_[i];
        }

        --num_items;
        data_ = std::move(new_data_);
    }

    T &operator[](size_t idx) {
        if (idx >= num_items) {
            throw std::runtime_error("The index is larger than size");
        }
        return data_[idx];
    }

    const T &operator[](size_t idx) const {
        if (idx >= num_items) {
            throw std::runtime_error("The index is larger than size");
        }
        return data_[idx];
    }

    size_t size() const noexcept { return num_items; }

    bool empty() const noexcept { return num_items == 0; }

    void clear() {
        data_.reset();
        num_items = 0;
    }

    void swap(MyArray &other) noexcept {
        std::swap(num_items, other.num_items);
        std::swap(data_, other.data_);
    }


    using iterator       = T *;
    using const_iterator = const T *;

    iterator begin() { return data_.get(); }
    iterator end() { return data_.get() + num_items; }

    const_iterator begin() const { return data_.get(); }

    const_iterator end() const { return data_.get() + num_items; }

private:
    std::unique_ptr<T[]> data_;
    size_t               num_items;
};
