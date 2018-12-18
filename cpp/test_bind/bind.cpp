#include <atomic>
#include <iostream>
#include <thread>
#include <string.h>
#include <list>

//std::list<std::functional>

int ft1(){
    return 0;
}

int ft2(){
    return 0;
}

class Runnable{
public:
    Runnable(){}
    virtual ~Runnable(){}
    virtual void run() = 0;
};

class TestRunnable{
public:
    TestRunnable(){}

    void test();
};

void TestRunnable::test(){
    printf("TestRunnable\n");

    class Base : public Runnable{
        virtual void run(){
            printf("Base\n");
        }
    };

    Runnable* runnable = new Base();
    runnable->run();
}


int main(){


    std::thread t1(ft1);
    std::thread t2(ft2);

    t1.join();
    t2.join();

    auto func = std::bind<>;


    TestRunnable* testrun = new TestRunnable();
    testrun->test();


    delete testrun;
    return 0;
}
