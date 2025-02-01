cmake_minimum_required(VERSION 3.5)
project(MySTL)

# 设置编译器
set(CMAKE_CXX_COMPILER "g++")
# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

# 设置编译选项
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -g -fcoroutines -O3 -Wall -Wextra -Weffc++ -Werror=uninitialized -Werror=return-type -Wconversion -Wsign-compare -Werror=unused-result -Werror=suggest-override -Wzero-as-null-pointer-constant -Wmissing-declarations -Wold-style-cast -Wnon-virtual-dtor -Wl,--copy-dt-needed-entries"
)

# 添加 src 子目录
add_subdirectory(src)
set_target_properties(src PROPERTIES LINKER_LANGUAGE CXX)
# 创建主程序可执行文件
add_executable(runner main.cpp)

# 链接 src 库
target_link_libraries(runner src)

# 启用测试
enable_testing()

# 查找 GTest
find_package(GTest REQUIRED)
include_directories(${GTest_INCLUDE_DIRS})

# 创建测试可执行文件
add_executable(run_tests tests/test.cpp)

# 链接 GTest 和 src 库
target_link_libraries(run_tests GTest::GTest GTest::Main src)

# 添加测试
add_test(NAME my_test COMMAND run_tests)
