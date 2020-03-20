#include <atomic>
#include <iostream>
#include <thread>
#include <string.h>
#include <time.h>
#include <memory>

::clock_t fuck;
std::atomic_int x, y;
int r1, r2;
int ft1(){
    r1 = x.load(std::memory_order_relaxed);
    y.store(r1, std::memory_order_relaxed);
    return 0;
}

int ft2(){
    r2 = x.load(std::memory_order_relaxed);
    y.store(42, std::memory_order_relaxed);
    return 0;
}

class A{
public:
    A(){a = 10;}
    
public:
    void print(){std::cout << a << std::endl;}
private:
    int a;
};

int main(){

    x = 0;
    y = 0;

    std::shared_ptr<A> spa;
    spa->print();
    std::cout << "finished!" << std::endl;
//    std::thread t1(ft1);
 //   std::thread t2(ft2);

   // t1.join();
    //t2.join();

//    std::cout << "x:" << x << ",y:" << y << std::endl;
//    std::cout << "r1:" << r1 << ",r2:" << r2 << std::endl;

    return 0;
}
