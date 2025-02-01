#include "src/MyArray.hpp"
#include <iostream>

int main (int argc, char *argv[]) {
    std::initializer_list<int> ilist{1, 2 , 3 , 4 , 5 , 6, 7 , 8 , 9 , 10};
    MyArray<int, 10> a(ilist);
    std::cout << "orgigin version: ";
    std::for_each(a.begin(), a.end(), [](int x){
        std::cout << x << " ";
    });
    std::cout << std::endl;
    a.insert(101);
    std::cout << "after insert: ";
    std::for_each(a.begin(), a.end(), [](int x){
        std::cout << x << " ";
    });
    std::cout << std::endl;
    a.remove(2);
    std::cout << "after remove: ";
    std::for_each(a.begin(), a.end(), [](int x){
        std::cout << x << " ";
    });
    std::cout << std::endl;
    MyArray<int, 10> b{10 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 1 , 2};
    std::swap(a, b);
    std::cout << "after swap: ";
    
    std::for_each(a.begin(), a.end(), [](int x){
        std::cout << x << " ";
    });

    std::cout << std::endl;


    MyArray<int, 3> arr{1 , 2 , 3};
    MyArray<int, 3> c(std::move(arr));

    return 0;
}
