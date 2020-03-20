

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

void test_memory_order_acquire_release()
{
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join(); t2.join();
}
