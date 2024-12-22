#include "thread_pool.h"
#include <iostream>
#include <functional>

ThreadPool::ThreadPool(size_t num_threads) : threads(num_threads), stop(false) {
    std::cout << "INFO: Thread pool initialized with " << num_threads << " threads." << std::endl;
    for (auto& thread : threads) {
        thread = std::thread([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    condition.wait(lock, [this] { return stop || !tasks.empty(); });
                    if (stop && tasks.empty()) {
                        std::cout << "DEBUG: Thread exiting." << std::endl;
                        return;
                    }
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                std::cout << "DEBUG: Thread started executing a task." << std::endl;
                task();
                std::cout << "DEBUG: Thread finished executing a task." << std::endl;
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (auto& thread : threads) {
        thread.join();
    }
    std::cout << "INFO: Thread pool destroyed." << std::endl;
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.push(std::move(task));
    }
    condition.notify_one();
    std::cout << "DEBUG: Enqueued a new task in the thread pool." << std::endl;
}