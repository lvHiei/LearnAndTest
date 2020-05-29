

#include "inc.h"


std::atomic<std::string*> ptr;
int data;

void producer()
{
    std::string* p  = new std::string("Hello");
    data = 42;
    ptr.store(p, std::memory_order_release);
}

void consumer()
{
    std::string* p2;
    while (!(p2 = ptr.load(std::memory_order_acquire)))
        ;
    assert(*p2 == "Hello"); // 绝无问题
    assert(data == 42); // 绝无问题
}

std::atomic<int> state;
enum{
    IDLE,
    INIT,
    PROCESSING,
};


void thread1()
{
    while(true){

        int expect = IDLE;

        while(!state.compare_exchange_strong(expect, INIT,std::memory_order_acq_rel, std::memory_order_relaxed)){
            LOGI("thread1 is waiting\n");
            expect = IDLE;
        }

        int i = random(100, 500);
        std::this_thread::sleep_for(std::chrono::microseconds(i));
        assert(state.load() == INIT);

        state.store(IDLE, std::memory_order_release);
    }
}

void thread2()
{
    while(true){

        int expect = IDLE;

        while(!state.compare_exchange_strong(expect, PROCESSING, std::memory_order_acq_rel, std::memory_order_relaxed)){
            LOGI("thread2 is waiting\n");
            expect = IDLE;
        }

        int i = random(100, 800);
        std::this_thread::sleep_for(std::chrono::microseconds(i));
        assert(state.load() == PROCESSING);

        state.store(IDLE, std::memory_order_release);
    }
}

void test_memory_order_acquire_release()
{
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join(); t2.join();
}


void test_memory_order_acq_rel()
{
    state.store(IDLE);
    std::thread t1(thread1);
    std::thread t2(thread2);
    t1.join(); t2.join();
}
