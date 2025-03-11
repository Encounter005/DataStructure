#pragma once

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <array>


class WriterAndReader {
private:
    std::mutex write_mutex;
    std::mutex read_mutex;
    std::condition_variable cv_read;
    std::condition_variable cv_write;
    std::atomic<int> read_count{0}; // 原子操作
    std::string data;
    std::atomic<int> index{0};      // 原子操作
    std::atomic<bool> stop{false};
    std::array<std::string, 4> res {"hello", "world", "hello", "cpp"};

public:

    void read() {
        while (!stop) {
            std::unique_lock<std::mutex> read_lock(read_mutex);
            cv_read.wait(read_lock, [this]() { return !data.empty() || stop; });
            if (stop) break;

            read_count++;
            read_lock.unlock();

            std::cout << "data is: " << data << std::endl;

            read_lock.lock();
            read_count--;
            if (read_count == 0) {
                cv_write.notify_one();
            }
        }
    }

    void write() {
        while (!stop) {
            std::unique_lock<std::mutex> write_lock(write_mutex);
            cv_write.wait(write_lock, [this]() { return read_count == 0 || stop; });
            if (stop) break;

            // 原子操作确保 index 安全
            int current_index = index++;
            data = res[current_index % 4];
            cv_read.notify_all();
        }
    }

    void stop_threads() {
        stop = true;
        cv_read.notify_all();  // 唤醒读者
        cv_write.notify_all(); // 唤醒写者
    }
};
