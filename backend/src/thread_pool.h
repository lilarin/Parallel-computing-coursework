#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>

class ThreadPool {
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;

public:
    explicit ThreadPool(size_t num_threads);
    ~ThreadPool();
    void enqueue(std::function<void()> task);
};

#endif // THREAD_POOL_H