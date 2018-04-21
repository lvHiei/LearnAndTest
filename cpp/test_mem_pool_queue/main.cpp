#include <stdlib.h>

#include "Queue.h"
#include "TimeUtil.h"
#include "logUtil.h"

using namespace vvav;

Queue* pTestQueue = NULL;
bool bStoped = false;

int random(int min, int max){
    return min + rand() % (max - min);
}



static void* thread1_func(void* arg)
{
    int i = 0;
    int sleeptime = 0;
    int buffersize = 0;
    uint8_t* data = NULL;
    while(i++ < 100000){
        buffersize = random(1024, 2048);
        data = (uint8_t*)malloc(buffersize);
        pTestQueue->push(data, buffersize, 0);
        free(data);
        sleeptime = random(2, 10);
        LOGI("push to queue %d Bytes, sleep %d ms", buffersize, sleeptime);
        TimeUtil::sleep(sleeptime);
    }

    bStoped = true;
    return NULL;
}


static void* thread2_func(void* arg)
{
    uint8_t* popData = (uint8_t*)malloc(2048);
    int64_t pts;
    int ret = 0;
    int sleeptime = 0;

    while(!bStoped || pTestQueue->getQueueSize() > 0){
        ret = pTestQueue->trypop(popData, 2048, 100, pts);
        sleeptime = random(1, 20);
        LOGI("pop from queue %d Bytes queSize:%d", ret, pTestQueue->getQueueSize());
        TimeUtil::sleep(sleeptime);
    }

    free(popData);
    return NULL;
}


void testQueue()
{
    srand(time(NULL));

    pTestQueue = new Queue();
    bStoped = false;
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1_func, NULL);
    pthread_create(&t2, NULL, thread2_func, NULL);


    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    delete pTestQueue;
}

void testMem()
{

    uint8_t** data = new uint8_t*[10000];

    LOGI("new data[]ing...");
    for(int i= 0; i < 10000; i++){
        data[i] = new uint8_t[2048];
        TimeUtil::sleep(5);
    }

    LOGI("deleting data[]ing...");
    for(int i= 0; i < 10000; i++){
        delete[] data[i];
        TimeUtil::sleep(5);
    }

    LOGI("delete data");
    delete data;
    TimeUtil::sleep(50*1000);
}

int main()
{
    testQueue();
    // testMem();

    return 0;
}