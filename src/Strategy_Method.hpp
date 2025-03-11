#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <type_traits>
#include <vector>

namespace strategy {

template<typename Iterator> class SortingStrategy {
public:
    virtual void execute(Iterator first, Iterator last) = 0;
    virtual ~SortingStrategy() {}
};

template<typename Iterator>
class Quick_Sorting : public SortingStrategy<Iterator> {
public:
    virtual void execute(Iterator first, Iterator last) override {
        if (first >= last) return;

        auto pivot = partition(first, last);
        execute(first, pivot);
        execute(std::next(pivot), last);
    }

private:
    Iterator partition(Iterator first, Iterator last) {
        auto pivot = *std::prev(last);

        std::iter_swap(first, std::prev(last));

        Iterator less_than_pivot = first;

        for (auto current = first + 1; current != last; ++current) {
            if (*current < pivot) {
                std::iter_swap(++less_than_pivot, current);
            }
        }

        std::iter_swap(first, less_than_pivot);

        return less_than_pivot;
    }
};

template<typename Iterator>

class Bubble_Sorting : public SortingStrategy<Iterator> {
public:
    virtual void execute(Iterator first, Iterator last) override {
        for (auto i = first; i != last; ++i) {
            bool isSwap = false;
            for (auto j = std::next(i); j != last; ++j) {
                if (*i > *j) {
                    std::iter_swap(i, j);
                    isSwap = true;
                }
            }

            if (!isSwap) {
                return;
            }
        }
    }
};

template<typename Iterator> class ShellSort : public SortingStrategy<Iterator> {
public:
    virtual void execute(Iterator first, Iterator last) override {
        if (first >= last) return;
        std::vector<size_t> gaps;
        auto                dist = std::distance(first, last);

        for (size_t gap = dist / 2; gap > 0; gap /= 2) {
            gaps.push_back(gap);
        }

        for (const auto& gap : gaps) {
            for (auto i = first + gap; i < last; ++i) {
                for (auto j = i; j >= first + gap && *(j - gap) > *j;
                     std::advance(j, -gap)) {
                    std::iter_swap(j, j - gap);
                }
            }
        }
    }
};
template<typename Iterator>
class Insertion_Sorting : public SortingStrategy<Iterator> {
public:
    virtual void execute(Iterator first, Iterator last) override {
        for (auto i = std::next(first); i != last; ++i) {
            auto value      = *i;
            auto insert_pos = std::upper_bound(first, i, value);
            std::move_backward(insert_pos, i, i + 1);
            *insert_pos = value;
        }
    }
};

template<typename Iterator>
class Selection_Sorting : public SortingStrategy<Iterator> {
public:
    virtual void execute(Iterator first, Iterator last) override {
        for (auto i = first; i != last; ++i) {
            auto min = i;
            for (auto j = std::next(i); j != last; ++j) {
                if (*j < *min) {
                    min = j;
                }
            }
            if (i != min) {
                std::iter_swap(i, min);
            }
        }
    }
};


template<typename Iterator>
class Merge_Sorting : public SortingStrategy<Iterator> {
public:
    virtual void execute(Iterator first, Iterator last) override {
        if (first >= last) return;

        using RefType   = decltype(*first);
        using ValueType = typename std::remove_reference<RefType>::type;
        std::vector<ValueType> tmp(std::distance(first, last));

        auto merge = [&](auto                    self,
                         Iterator                first,
                         Iterator                last,
                         std::vector<ValueType>& tmp) -> void {
            if (std::distance(first, last) <= 1) return;
            auto mid = std::next(first, std::distance(first, last) / 2);
            self(self, first, mid, tmp);
            self(self, mid, last, tmp);
            auto i = first, j = mid, k = 0;
            while (i != mid && j != last) {
                if (*i < *j)
                    tmp[k++] = *i++;
                else
                    tmp[k++] = *j++;
            }
            while(i != mid) tmp[k++] = *i++;
            while (j != last) tmp[k++] = *j++;


            for (auto index1 = first, index2 = 0; index1 != last;
                 ++index1, ++index2) {
                *index1 = tmp[index2];
            }
        };

        merge(merge, first, last, tmp);
    }
};

template<typename Iterator>
class HeapMaxSorting : public SortingStrategy<Iterator> {
public:
    virtual void execute(Iterator first, Iterator last) override {
        if (first >= last) return;

        auto len = std::distance(first, last);
        auto arr = first;

        auto heapify
            = [&](auto self, auto arr, int length, int max_index) -> void {
            auto largest = max_index;
            auto left    = 2 * max_index + 1;
            auto right   = 2 * max_index + 2;

            if (left < length && *(arr + largest) < *(arr + left))
                largest = left;
            if (right < length && *(arr + largest) < *(arr + right))
                largest = right;
            if (max_index != largest) {
                std::iter_swap(
                    std::next(arr, max_index), std::next(arr, largest));
                self(self, arr, length, largest);
            }
        };

        for (int i = len / 2 - 1; i >= 0; i--) {
            heapify(heapify, first, len, i);
        }

        for (int i = len - 1; i > 0; i--) {
            std::iter_swap(first, first + i);
            heapify(heapify, arr, i, 0);
        }
    }
};

template<typename Iterator> class SortContext {
public:
    void setStrategy(std::unique_ptr<SortingStrategy<Iterator>> strategy) {
        this->strategy = std::move(strategy);
    }

    void executeStrategy(Iterator first, Iterator last) {
        strategy->execute(first, last);
    }

private:
    std::unique_ptr<SortingStrategy<Iterator>> strategy;
};



inline void test_func() {
    std::vector<int> num{
        2, 2, 2, 2, 2, 1892, 23, 12, 22332323, -1213, -1, 223, 0};
    using iterator = std::vector<int>::iterator;
    Quick_Sorting<iterator> quick_sorting;
    SortContext<iterator>   context;
    context.setStrategy(
        std::make_unique<Quick_Sorting<iterator>>(quick_sorting));
    context.executeStrategy(num.begin(), num.end());

    for (const auto& elm : num) {
        std::cout << elm << " ";
    }

    std::cout << std::endl;
}

};   // namespace strategy
