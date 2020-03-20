#include <atomic>
#include <iostream>
#include <thread>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

#include "TimeUtil.h"

std::atomic_int x, y;
pthread_mutex_t mutex;
pthread_cond_t cond;
bool want_stop = false;

uint32_t last_request_time = 0;
int r1, r2;

using namespace vvav;

int ft1(){
    int rendertimes = 0;
    uint32_t now = 0;
    while(!want_stop){
        if(now == 0 || now == last_request_time){
            pthread_mutex_lock(&mutex);
            pthread_cond_wait(&cond, &mutex);
            pthread_mutex_unlock(&mutex);
        }
        printf("rendering %d times...\n", ++rendertimes);
        now = last_request_time;
        usleep(60 * 1000);        
    }

    printf("ft1 end \n");

    return 0;
}

int ft2(){

    usleep(300 * 1000);
    int i = 0;
    while(i++ < 300){
        printf("request render %d\n", i);
        last_request_time = TimeUtil::GetTickCount();
        pthread_cond_signal(&cond);
        usleep(40 * 1000);
    }
    want_stop = true;
    pthread_cond_signal(&cond);

    printf("ft2 end \n");
    return 0;
}

int main(){

    x = 0;
    y = 0;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    std::thread t1(ft1);
    std::thread t2(ft2);

    t1.join();
    t2.join();

    std::cout << "x:" << x << ",y:" << y << std::endl;
    std::cout << "r1:" << r1 << ",r2:" << r2 << std::endl;

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
