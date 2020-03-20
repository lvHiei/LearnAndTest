#include <atomic>
#include <iostream>
#include <thread>
#include <string.h>
#include <cstdio>
#include <chrono>
#include <stdint.h>

uint64_t now() {
    auto time_now = std::chrono::system_clock::now();
    //auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
    //auto duration_in_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(time_now.time_since_epoch());
    auto duration_in_ms = std::chrono::duration_cast<std::chrono::microseconds>(time_now.time_since_epoch());
    return duration_in_ms.count();
}

class Base{
public:
    Base(){printf("Base()\n");}
//    ~Base(){printf("~Base()\n");}
    virtual ~Base(){printf("~Base()\n");}

};


class Sub1: public Base{
public:
    Sub1():Base(){printf("Sub1()\n");}
    ~Sub1(){printf("~Sub1()\n");}
//    virtual ~Sub1(){printf("~Sub1()\n");}

};

class Sub2:public Sub1{
public:
    Sub2(){printf("Sub2()\n");}
    ~Sub2(){printf("~Sub2()\n");}
//    virtual ~Sub2(){printf("~Sub2()\n");}

};

int main(){
   
    Base* base = new Sub1();
    int c = 0;
    do{
    long loopcount = 1000000;
    uint64_t begin0 = now();
    for(int i = 0; i < loopcount;++i){
        
    }
    printf("none cost %llums\n", now() - begin0);

    uint64_t begin1 = now();
    Sub1* sub = nullptr;
    for(int i = 0; i < loopcount;++i){
        sub = dynamic_cast<Sub1*>(base);     
    }

    printf("cast cost %llums\n", now() - begin1);
    }while(++c < 50);
    delete base;

    return 0;
}
