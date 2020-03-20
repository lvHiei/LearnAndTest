

#include "inc.h"

::clock_t fuck;
std::atomic_int x, y;
int r1, r2;
int ft1(){
    r1 = y.load(std::memory_order_relaxed);
    x.store(r1, std::memory_order_relaxed);
    return 0;
}

int ft2(){
    r2 = x.load(std::memory_order_relaxed);
    y.store(42, std::memory_order_relaxed);
    return 0;
}

void test_memory_order_relaxed(){
    uint64_t cnt = 0;
    while(1){
    x = 0;
    y = 0;
    std::thread t1(ft1);
    std::thread t2(ft2);

    t1.join();
    t2.join();

    ++cnt;
    if(r1 == 42 && r2 == 42){
        break;
    }
    }
    std::cout << "x:" << x << ",y:" << y << std::endl;
    std::cout << "r1:" << r1 << ",r2:" << r2 << std::endl;
    std::cout << "count=" << cnt << std::endl;
}
