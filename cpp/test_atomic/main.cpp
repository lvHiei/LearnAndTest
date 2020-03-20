
#include "inc.h"

std::atomic_int ai;
int want = 3;
std::atomic_int aii;
void func(int type){
    ai.fetch_or(type, std::memory_order_relaxed);
    
    int cmp = want;
    if(ai.compare_exchange_strong(cmp, cmp + 1)){
//    if(ai.compare_exchange_strong(cmp, cmp)){
//    if(ai == cmp){
        ++aii;
        std::cout << "run in " << type << std::endl;
    }
}

void f1(){
    func(1);
}

void f2(){
    func(2);
}

int main(){
    //test_memory_order_relaxed();
    test_memory_order_acquire_release();

    while(1){
    ai = 0;
    aii = 0;
    std::thread t1(f1);
    std::thread t2(f2);
    t1.join(); t2.join();

    if(aii == 2){
        std::cout << "got it" << std::endl;
        break;
    }
    }
    return 0;
}
