#include <atomic>
#include <iostream>
#include <thread>
#include <string.h>
#include <time.h>

using namespace std;

std::thread t1, t2;

int ft1(){
    int i = 100;
    std::this_thread::sleep_for(std::chrono::microseconds(i));
    cout << "f1 end" << endl;
    return 0;
}

int ft2(){
    int i = 10;
    std::thread t1(ft1);
    std::this_thread::sleep_for(std::chrono::microseconds(i));
    t1.join();
    cout << "f2 end" << endl;
    return 0;
}


int main(){


    std::thread t2(ft2);

//    t1.join();
    t2.join();

    return 0;
}
