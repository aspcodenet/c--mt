#include <iostream>
#include <thread>
#include <string>
#include <condition_variable>
#include <vector>
#include <mutex>
#include <queue>
#include <functional>
#include <variant>


#include <cstddef>
#include <cstdint>
#include <thread>
#include <vector>
using namespace std::chrono_literals;

class ThreadPool {
public:
    using Task = std::function<void()>;

    /// constructs the threadpool with n_threads threads
    ThreadPool(size_t n_threads){
        m_threads.reserve(n_threads);
        for (size_t i = 0; i < n_threads; ++i) {
            m_threads.emplace_back(&ThreadPool::thread_main, this);
        }        
    }
    /// blocks the destruction until all tasks are finished
    ~ThreadPool(){
        std::unique_lock lock(m_tasks_mtx);
        while (!m_tasks.empty()) {
            lock.unlock();
            m_tasks_cnd.notify_all();
            lock.lock();
        }
        lock.unlock();
        for (auto& thread : m_threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }        
    }

    /// adds a task to be executed
    void add_task(Task task){
        std::unique_lock lock(m_tasks_mtx);
        m_tasks.emplace(std::move(task));
        m_tasks_cnd.notify_one();        
    }

private:
    void thread_main() {
        while (!m_shutdown) {
            // if we have tasks, we want to move the
            // first task out, remove it from the queue,
            // and then execute it
            std::unique_lock lock(m_tasks_mtx);
            // with the lock, we can now wait on it with our condition variable
            m_tasks_cnd.wait(lock);
            while (!m_tasks.empty()) {
                Task task = std::move(m_tasks.front());
                m_tasks.pop();
                lock.unlock();
                task();
                lock.lock();
            }
        }
    }

    std::queue<Task> m_tasks;
    std::mutex m_tasks_mtx;
    std::condition_variable m_tasks_cnd;
    std::vector<std::thread> m_threads;

    bool m_shutdown { false };
};


void doingWork(std::string prefix){
    for(int i = 0; i < 100; i++){
        std::cout << prefix <<  i << ":";
        std::this_thread::sleep_for(100ms);
    }
}

int main(int, char**){
    ThreadPool pool { 8 };
    for(int i = 0; i < 10; i++){
            pool.add_task([i] {
            doingWork(std::to_string(i) + "-");
        });        
    }

    while(1){
        std::this_thread::sleep_for(1000ms);        
    }


    // while(1){
    //     std::string action;
    //     std::cout << std::endl <<  "1. Start new work..." << std::endl << "2. Exit" << std::endl;
    //     std::getline(std::cin,action);
    //     if(action == "1"){
    //         doingWork("F");
    //     }
    //     if(action == "2"){
    //         return;
    //     }

    // }

    // int x=12;
    // std::this_thread::sleep_for(100ms);
    // x++; 
    // std::this_thread::sleep_for(100ms);
    // x++;
    // std::this_thread::sleep_for(100ms);
    // x++;
    // int x=15;
    // std::this_thread::sleep_for(300ms);


    // std::thread worker1(doingWork,"F");
    // std::thread worker2(doingWork,"G");

    // worker1.join();
    // worker2.join();
    // join 
    // ONE THREAD - branch
}