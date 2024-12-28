#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(size_t threadCount);
    ~ThreadPool();
    
    template <typename Func, typename ...Args>
    auto submit(Func&& func, Args&&... args) -> std::future<typename std::result_of<Func(Args...)>::type>;
    
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stopFlag;
};

ThreadPool::ThreadPool(size_t threadCount) : stopFlag(false) {
    for (size_t i = 0; i < threadCount; ++i) {
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                
                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    this->condition.wait(lock, [this]() {
                        return this->stopFlag || !this->taskQueue.empty();
                    });
                    
                    if (this->stopFlag && this->taskQueue.empty()) return;
                    
                    task = std::move(this->taskQueue.front());
                    this->taskQueue.pop();
                }
                
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stopFlag = true;
    }
    condition.notify_all();
    
    for (std::thread& worker : workers)
        if (worker.joinable())
        worker.join();
}

template <typename Func, typename... Args>
auto ThreadPool::submit(Func&& func, Args&&... args) -> std::future<typename std::result_of<Func(Args...)>::type> {
    using ReturnType = typename std::result_of<Func(Args...)>::type;
    
    auto task = std::make_shared<std::packaged_task<ReturnType()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
    
    std::future<ReturnType> result = task->get_future();
    
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        
        if (stopFlag)
            throw std::runtime_error("ThreadPool is stopeed. Cannot submit tasks");
        
        taskQueue.emplace([task]() { (*task)(); });
    }
    
    condition.notify_one();
    return result;
}

int main() {
    ThreadPool pool(4);
    
    auto future1 = pool.submit([]() {
        std::cout << "Task 1 executed by thread " << std::this_thread::get_id() << "\n";
        return 1;
    });
    
    auto future2 = pool.submit([](int a, int b) {
        std::cout << "Task 2 executed by thread " << std::this_thread::get_id() << "\n";
        return a + b;
    }, 2, 3);
    
    auto future3 = pool.submit([]() {
        std::cout << "Task 3 executed by thread " << std::this_thread::get_id() << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return 42;
    });
    
    std::cout << "Result of Task 1: " << future1.get() << "\n";
    std::cout << "Result of Task 2: " << future2.get() << "\n";
    std::cout << "Result of Task 3: " << future3.get() << "\n";
    
    return 0;
}