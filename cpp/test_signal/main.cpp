#include <atomic>
#include <iostream>
#include <thread>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

std::atomic_int x, y;
pthread_mutex_t mutex;
pthread_cond_t cond;
bool want_stop = false;


int r1, r2;
int ft1(){
    while(!want_stop){
        pthread_mutex_lock(&mutex);
        usleep(500 * 1000);
        pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);

        usleep(500 * 1000);        
    }

    printf("ft1 end \n");

    return 0;
}

int ft2(){

    usleep(300 * 1000);

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
