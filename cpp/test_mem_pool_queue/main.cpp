#include <stdlib.h>

#include "Queue.h"
#include "TimeUtil.h"
#include "logUtil.h"

using namespace vvav;

Queue* pTestQueue = NULL;
bool bStoped = false;


int popMaxCost = 0;
int popMinCost = 10000000;
int64_t popSumCost = 0;
int popAvgCost = 0;

int random(int min, int max){
    return min + rand() % (max - min);
}



static void* thread1_func(void* arg)
{
    int i = 0;
    int sleeptime = 0;
    int buffersize = 0;
    uint8_t* data = NULL;
    int looptime = 0;
    uint64_t t0, t1;
    const int max_wloop_time = 10000000;
    int wlooptime = max_wloop_time;
    int maxCost = 0;
    int minCost = 10000000;
    int64_t sumCost = 0;
    int avgCost = 0;
loop:
    i = 0;
    wlooptime = wlooptime < 10000 ? max_wloop_time : wlooptime / 10;
    while(i++ < wlooptime){
        if(i % 100000 == 0){
            LOGI("clear queue...............");
            pTestQueue->clear();
        }
        buffersize = random(1024, 2048);
        data = (uint8_t*)malloc(buffersize);
        t0 = TimeUtil::GetTickCountUs();
        pTestQueue->push(data, buffersize, 0);
        t1 = TimeUtil::GetTickCountUs();
        free(data);
        sleeptime = random(0, 5);
        
        int cost = t1 - t0;
        if(cost < minCost){
            minCost = cost;
        }
        
        if(cost > maxCost){
            maxCost = cost;
        }

        sumCost += cost;
        avgCost = sumCost / i;

        LOGI("push to queue %d Bytes, sleep %d us cost %u us", buffersize, sleeptime, t1-t0);
        TimeUtil::usleep(sleeptime);
    }
    ++looptime;
    time_t now = time(NULL);
    LOGI("looptime %d push mincost:%d,maxCost:%d,avgCost:%d,pop mincost:%d,maxCost:%d,avgCost:%d, now %s", 
            looptime,minCost, maxCost, avgCost, popMinCost, popMaxCost, popAvgCost, ctime(&now));
    if(looptime < 1000){
        maxCost = 0;
        minCost = 100000;
        sumCost = 0;
        avgCost = 0;
        
        popMaxCost = 0;
        popMinCost = 100000;

        goto loop;
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
    uint64_t t0, t1;
    int64_t i = 0;
    
    while(!bStoped || pTestQueue->getQueueSize() > 0){
        t0 = TimeUtil::GetTickCountUs();
        ret = pTestQueue->trypop(popData, 2048, 100, pts);
        t1 = TimeUtil::GetTickCountUs();

        i++;
        int cost = t1 -t0;
        if(cost < popMinCost){
            popMinCost = cost;
        }
        
        if(cost > popMaxCost){
            popMaxCost = cost;
        }
        
        popSumCost += cost;
        popAvgCost = popSumCost / i;

        sleeptime = random(0, 4);
        LOGI("pop from queue %d Bytes queSize:%d, cost %u us", ret, pTestQueue->getQueueSize(), t1 - t0);
        TimeUtil::usleep(sleeptime);
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


    pTestQueue->clear();
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
