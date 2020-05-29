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
class B;
class A : public std::enable_shared_from_this<A>{
public:
    A();//{a = 10;std::cout << "A constructor" << std::endl;}
    ~A(){std::cout << "A desconstructor" << std::endl;}
    
public:
    void print(){std::cout << a << std::endl;}
private:
    int a;
    std::shared_ptr<B> mB;
};

class B{
public:
    B(std::shared_ptr<A> a){
        mA = a;
        std::cout << "B cons" << std::endl;
    }
    virtual ~B(){
        std::cout << "B des" << std::endl;
    }

public:

private:
    std::weak_ptr<A> mA;
};

A::A(){
    //mB.reset(new B(shared_from_this()));
    a = 10;std::cout << "A constructor" << std::endl;
}

int main(){

    x = 0;
    y = 0;

    std::shared_ptr<A> spa;
    std::unique_ptr<A> upa;
    std::weak_ptr<A> wpa;
    if(!spa){
        std::cout << "got it" << std::endl;
    }else{
        spa->print();
    }
    if(!upa){
        std::cout << "got it" << std::endl;
    }else{
        upa->print();
    }
    spa.reset(new A);
    wpa = spa;
    spa.reset(new A);
    if(wpa.expired()){
        std::cout << "expired" << std::endl;
    }
    upa.reset(new A);
    std::cout << "sp reset.." << std::endl;
    spa.reset(new A);
    std::shared_ptr<A> sp1 = spa;
    spa.reset(new A);
    
    std::cout << "dd" << std::endl;
    std::cout << "finished!" << std::endl;
//    std::thread t1(ft1);
 //   std::thread t2(ft2);

   // t1.join();
    //t2.join();

//    std::cout << "x:" << x << ",y:" << y << std::endl;
//    std::cout << "r1:" << r1 << ",r2:" << r2 << std::endl;

    return 0;
}
