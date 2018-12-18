//
// Created by mj on 16-2-19.
//

#include <errno.h>
#include <assert.h>
#include <memory.h>
#include <sys/time.h>

#include "Queue.h"

#include "const.h"
#include "logUtil.h"
#include "TimeUtil.h"

namespace vvav {
    Queue::Queue() {
        m_bFinished = false;
        m_nPackets = 0;
        m_pFirstPkt = NULL;
        m_pLastPkt = NULL;

        m_pMemPoolFirstPtk = NULL;
        m_pMemPoolLastPtk = NULL;
        m_nMemPoolPackets = 0;

        m_iMemPoolMaxCapacity = 0;
        m_iMemPoolMinCapacity = INT32_MAX;

//        m_cond = PTHREAD_COND_INITIALIZER;
        pthread_mutex_init(&m_mutex, NULL);

        pthread_cond_init(&m_cond, NULL);
    }


    Queue::~Queue() {
        clear();
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_cond);
    }

    int32_t Queue::push(void *data, int32_t length, int64_t pos) {
        if (!data || length <= 0) {
            LOGE("queue push: parameter error! data=%p,length=%d", data, length);
            return -1;
        }

        pthread_mutex_lock(&m_mutex);

        StruPacketList *pkt = getMemPoolPacket(length);

        if (!pkt || !pkt->data) {
            pthread_mutex_unlock(&m_mutex);
            return -1;
        }

        pkt->length = length;
        pkt->pts = pos;
        memcpy(pkt->data, data, pkt->length);
        pkt->next = NULL;

        // 第一个入队列的
        if (NULL == m_pFirstPkt && NULL == m_pLastPkt) {
            m_pFirstPkt = pkt;
            m_pLastPkt = pkt;
        } else {
            m_pLastPkt->next = pkt;
            m_pLastPkt = pkt;
        }

        ++m_nPackets;
        pthread_cond_signal(&m_cond);

        pthread_mutex_unlock(&m_mutex);

        return 0;
    }


    int32_t Queue::pop(void *data, int32_t length, int64_t &pts) {
        int ret = 0;

        if (!data) {
            return -1;
        }

        pthread_mutex_lock(&m_mutex);

        StruPacketList *popPkt = m_pFirstPkt;

        while (!popPkt) { //is empty
            if (m_bFinished) {
//            LOGI("queue pop: finished");
                pthread_mutex_unlock(&m_mutex);
                return -1;
            }

            pthread_cond_wait(&m_cond, &m_mutex);
            popPkt = m_pFirstPkt;
        }

        if (length < popPkt->length) {
            LOGE("queue pop : length not enough!");
            pthread_mutex_unlock(&m_mutex);
            return -1;
        }

        if (!popPkt->data) {
            LOGE("queue pop : packet data is NULL!");
            pthread_mutex_unlock(&m_mutex);
            return -1;
        }

        memcpy(data, popPkt->data, popPkt->length);
        ret = popPkt->length;
        pts = popPkt->pts;

        m_pFirstPkt = m_pFirstPkt->next;
        if (m_pLastPkt == popPkt) {
            m_pLastPkt = NULL;
        }

        if (!addMemPoolPacket(popPkt)) {
            delete[] popPkt->data;
            delete popPkt;
        }

        --m_nPackets;

        pthread_mutex_unlock(&m_mutex);

        return ret;
    }

    int32_t Queue::trypop(void *data, int32_t length, long timeOutwithms, int64_t &pts) {
        int ret = 0;

        if (!data) {
            return -1;
        }
        pthread_mutex_lock(&m_mutex);

        struct timeval now;
        struct timespec outtime;
        StruPacketList *popPkt = m_pFirstPkt;

        while (!popPkt) { //is empty'
            if (m_bFinished) {
//            LOGI("queue trypop : finished");
                pthread_mutex_unlock(&m_mutex);
                return -1;
            }

            gettimeofday(&now, NULL);
            outtime.tv_sec = now.tv_sec + timeOutwithms / 1000;
            outtime.tv_nsec = now.tv_usec * 1000 + (timeOutwithms % 1000) * 1000 * 1000;

            if (outtime.tv_nsec >= TIME_OVER_FLOW) {
                ++(outtime.tv_sec);
                outtime.tv_nsec -= TIME_OVER_FLOW;
            }

            int waitResult = pthread_cond_timedwait(&m_cond, &m_mutex, &outtime);

            if (ETIMEDOUT == waitResult) {
                pthread_mutex_unlock(&m_mutex);
                return ret;
            } else if (EINVAL == waitResult) {
                LOGE("Queue::trypop wait invalid value");
                pthread_mutex_unlock(&m_mutex);
                return ret;
            }

            popPkt = m_pFirstPkt;
        }

        if (length < popPkt->length) {
            LOGE("queue trypop : length not enough! poplength = %d, length = %d", popPkt->length,
                 length);
            pthread_mutex_unlock(&m_mutex);
            return -1;
        }

        if (!popPkt->data) {
            LOGE("queue trypop : packet data is NULL!");
            pthread_mutex_unlock(&m_mutex);
            return -1;
        }

        memcpy(data, popPkt->data, popPkt->length);
        ret = popPkt->length;
        pts = popPkt->pts;

        m_pFirstPkt = m_pFirstPkt->next;
        if (m_pLastPkt == popPkt) {
            m_pLastPkt = NULL;
        }

        if (!addMemPoolPacket(popPkt)) {
            LOGI("delete popPkt %p", popPkt);
            delete[] popPkt->data;
            delete popPkt;
        }

        --m_nPackets;

        pthread_mutex_unlock(&m_mutex);

        return ret;
    }


    int32_t Queue::trypopWithPts(void *data, int32_t length, long timeOutwithms, int64_t pts) {
        int ret = 0;

        if (!data) {
            return -1;
        }
        pthread_mutex_lock(&m_mutex);

        struct timeval now;
        struct timespec outtime;
        StruPacketList *popPkt = m_pFirstPkt;

        while (!popPkt) { //is empty'
            if (m_bFinished) {
//            LOGI("queue trypop : finished");
                pthread_mutex_unlock(&m_mutex);
                return -1;
            }

            gettimeofday(&now, NULL);
            outtime.tv_sec = now.tv_sec + timeOutwithms / 1000;
            outtime.tv_nsec = now.tv_usec * 1000 + (timeOutwithms % 1000) * 1000 * 1000;

            if (outtime.tv_nsec >= TIME_OVER_FLOW) {
                ++(outtime.tv_sec);
                outtime.tv_nsec -= TIME_OVER_FLOW;
            }

            int waitResult = pthread_cond_timedwait(&m_cond, &m_mutex, &outtime);

            if (ETIMEDOUT == waitResult) {
                pthread_mutex_unlock(&m_mutex);
                return ret;
            } else if (EINVAL == waitResult) {
                LOGE("Queue::trypop wait invalid value");
                pthread_mutex_unlock(&m_mutex);
                return ret;
            }

            popPkt = m_pFirstPkt;
        }

        if (length < popPkt->length) {
            LOGE("queue trypop : length not enough! poplength = %d, length = %d", popPkt->length,
                 length);
            pthread_mutex_unlock(&m_mutex);
            return -1;
        }

        if (!popPkt->data) {
            LOGE("queue trypop : packet data is NULL!");
            pthread_mutex_unlock(&m_mutex);
            return -1;
        }

        if (popPkt->pts > pts) {
            pthread_mutex_unlock(&m_mutex);
            return 0;
        }

        memcpy(data, popPkt->data, popPkt->length);
        ret = popPkt->length;
//    pts = popPkt->pts;

        m_pFirstPkt = m_pFirstPkt->next;
        if (m_pLastPkt == popPkt) {
            m_pLastPkt = NULL;
        }

        if (!addMemPoolPacket(popPkt)) {
            delete[] popPkt->data;
            delete popPkt;
        }

        --m_nPackets;

        pthread_mutex_unlock(&m_mutex);

        return ret;
    }


    void Queue::clear() {
        pthread_mutex_lock(&m_mutex);

        clearQueue();
        clearMemPoolQueue();

        pthread_mutex_unlock(&m_mutex);
    }

    void Queue::clearQueue() {
        StruPacketList *pkt = m_pFirstPkt;

        while (NULL != pkt) {
            StruPacketList *deletePkt = pkt;
            pkt = pkt->next;
            if (deletePkt->data) {
                delete[] deletePkt->data;
                deletePkt->data = NULL;
            }
            delete deletePkt;
            deletePkt = NULL;
        }

        m_pFirstPkt = NULL;
        m_pLastPkt = NULL;

        m_nPackets = 0;
        m_bFinished = false;

        pthread_cond_signal(&m_cond);
    }

    void Queue::clearMemPoolQueue() {
        StruPacketList *pkt = m_pMemPoolFirstPtk;

        while (NULL != pkt) {
            StruPacketList *deletePkt = pkt;
            pkt = pkt->next;
            if (deletePkt->data) {
                delete[] deletePkt->data;
                deletePkt->data = NULL;
            }
            delete deletePkt;
            deletePkt = NULL;
        }

        m_pMemPoolFirstPtk = NULL;
        m_pMemPoolLastPtk = NULL;

        m_nMemPoolPackets = 0;

        m_iMemPoolMaxCapacity = 0;
        m_iMemPoolMinCapacity = INT32_MAX;
    }

    StruPacketList *Queue::getMemPoolPacket(int length) {
        // 调用该函数的时候保证外面加锁

        StruPacketList *retPkt = getBestPacket(length);

        // 内存池中没有内存块时  直接申请一块内存
        if (NULL == retPkt) {
            retPkt = new StruPacketList();
            if (!retPkt) {
                LOGE("queue getMemPoolPacket: malloc PacketList failed!");
                return NULL;
            }

            retPkt->data = new u_int8_t[length];
            if (!retPkt->data) {
                delete retPkt;
                LOGE("queue getMemPoolPacket: malloc pkt->data failed!");
                return NULL;
            }
            retPkt->capacity = length;
            LOGD("getMemPoolPacket ret new packet %p, capacity:%d", retPkt, retPkt->capacity);
            return retPkt;
        }

        return retPkt;
    }


    StruPacketList *Queue::getBestPacket(int length) {
        // 调用该函数的时候保证外面加锁
        if(!m_pMemPoolFirstPtk){
            return NULL;
        }

        StruPacketList* bestPkt = NULL;
        StruPacketList* bestLastPkt = NULL;
        StruPacketList* pkt = m_pMemPoolFirstPtk;
        StruPacketList* lastpkt = NULL;

        // 如果内存池里内存大小都一样 则直接返回m_pMemPoolFirstPtk
        if(m_iMemPoolMinCapacity == m_iMemPoolMaxCapacity){
            if(length > m_iMemPoolMaxCapacity){
                return NULL;
            }

            bestPkt = m_pMemPoolFirstPtk;
            handleGetPakcet(bestPkt, bestLastPkt);
            return bestPkt;
        }

        while(pkt){
            // 如果当前的pkt的长度与需要的相同 则直接返回该packet
            if(pkt->capacity == length){
                handleGetPakcet(pkt, lastpkt);
                return pkt;
            }
            // 如果pkt的容量 大于需要的长度 则计算bestPkt
            else if (pkt->capacity > length){
                // 如果bestPkt为NULL 则将bestPkt指向pkt bestLastPkt指向lastpkt
                if(!bestPkt){
                    bestPkt = pkt;
                    bestLastPkt = lastpkt;
                }
                else{
                    // 如果bestPkt不为空，则判断长度pkt的容量是否小于bestPkt，
                    // 如果小于 则将bestPkt指向pkt bestLastPkt指向lastpkt
                    if(pkt->capacity < bestPkt->capacity){
                        bestPkt = pkt;
                        bestLastPkt = lastpkt;
                    }
                }
            }
            // 如果pkt的容量 小于需要的长度 则继续查找
            else{

            }

            // pkt前移
            lastpkt = pkt;
            pkt = pkt->next;
        }

        // 如果没有找到大于需要长度的·packet 则返回空
        if(!bestPkt){
            return NULL;
        }

        handleGetPakcet(bestPkt, bestLastPkt);

        return bestPkt;
    }


    void Queue::handleGetPakcet(StruPacketList *pkt, StruPacketList* lastPkt)
    {
        assert(m_pMemPoolFirstPtk);
        assert(pkt);
        assert(!(pkt != m_pMemPoolFirstPtk && !lastPkt));

        if(!m_pMemPoolFirstPtk || !pkt){
            return;
        }

        if(pkt != m_pMemPoolFirstPtk && !lastPkt){
            return;
        }

        // 如果当前packet是第一个pkt 则将m_pMemPoolFirstPtk前移
        if(pkt == m_pMemPoolFirstPtk){
            m_pMemPoolFirstPtk = m_pMemPoolFirstPtk->next;
            // 如果m_pMemPoolFirstPtk为NULL 则将m_pMemPoolLastPtk也置为NULL
            if (!m_pMemPoolFirstPtk) {
                m_pMemPoolLastPtk = NULL;
                
                m_iMemPoolMaxCapacity = 0;
                m_iMemPoolMinCapacity = INT32_MAX;
            }
        }else{
            // 如果pkt为中间的packet 则将其上一个packet lastpkt 的next指向pkt的next
            lastPkt->next = pkt->next;
            // 如果pkt 为m_pMemPoolLastPtk 则将m_pMemPoolLastPtk 置为pkt的上一个packet
            if(pkt == m_pMemPoolLastPtk){
                m_pMemPoolLastPtk = lastPkt;
                m_pMemPoolLastPtk->next = NULL;
            }
        }

        updateMemPoolValue();

        --m_nMemPoolPackets;
        LOGI("handleGetPakcet mempoolsize:%d", m_nMemPoolPackets);

        
        int i = 0;
        for(StruPacketList* lPtk = m_pMemPoolFirstPtk; lPtk != NULL; lPtk = lPtk->next){
        //    LOGI("handleGetPakcet pakcet %p", lPtk);
            i++;
        }
        

        if(i != m_nMemPoolPackets){
            LOGI("handleGetPakcet error packets:%d, count:%d", m_nMemPoolPackets, i);
        }
    }


    bool Queue::addMemPoolPacket(StruPacketList *pkt) {
        // 调用该函数的时候保证外面加锁
        if (!pkt || !pkt->data) {
            return false;
        }

        // 内存池未满 则直接加入队列
        if (m_nMemPoolPackets < MAX_MEM_POOL_QUEUE_LENGTH) {
            // 脱离之前的队列
            pkt->next = NULL;
            memset(pkt->data, 0, pkt->capacity);

            if(m_iMemPoolMaxCapacity < pkt->capacity){
                m_iMemPoolMaxCapacity = pkt->capacity;
            }

            if(m_iMemPoolMinCapacity > pkt->capacity){
                m_iMemPoolMinCapacity = pkt->capacity;
            }

            // 放到内存池队列的队尾
            if (!m_pMemPoolFirstPtk) {
                m_pMemPoolFirstPtk = pkt;
                m_pMemPoolLastPtk = pkt;
            } else {
                m_pMemPoolLastPtk->next = pkt;
                m_pMemPoolLastPtk = pkt;
            }


            for(StruPacketList* lPtk = m_pMemPoolFirstPtk; lPtk != NULL; lPtk = lPtk->next){
                LOGI("addMemPoolPacket not full pakcet %p", lPtk);
            }

            ++m_nMemPoolPackets;
            return true;
        }

        // 内存池已满 则判断队列中是否有小于该块内存的packet
        // 如果有 则替换掉最小的;如果没有 则返回
        if(m_iMemPoolMinCapacity >= pkt->capacity){
            return false;
        }

        StruPacketList* minPkt = NULL;
        StruPacketList* minLastPkt = NULL;
        StruPacketList* loopPkt = m_pMemPoolFirstPtk;
        StruPacketList* loopLastPkt = NULL;

        while(loopPkt){
            if(loopPkt->capacity == m_iMemPoolMinCapacity){
                minPkt = loopPkt;
                minLastPkt = loopLastPkt;
                break;
            }

            loopLastPkt = loopPkt;
            loopPkt = loopPkt->next;
        }

//        assert(minPkt);


        if(minPkt == m_pMemPoolFirstPtk){
            pkt->next = m_pMemPoolFirstPtk->next;
            m_pMemPoolFirstPtk = pkt;
//            m_pMemPoolFirstPtk->next = minPkt->next;
        }else if(minPkt == m_pMemPoolLastPtk){
            minLastPkt->next = pkt;
            m_pMemPoolLastPtk = pkt;
            m_pMemPoolLastPtk->next = NULL;
        }else{
            pkt->next = minPkt->next;
            minLastPkt->next = pkt;
        }

        updateMemPoolValue();

        
        int i = 0;
        for(StruPacketList* lPtk = m_pMemPoolFirstPtk; lPtk != NULL; lPtk = lPtk->next){
        //    LOGI("addMemPoolPacket full pakcet %p", lPtk);
            i++;
        }
        
        if(i != m_nMemPoolPackets){
            LOGI("addMemPoolPacket error packets:%d, count:%d", m_nMemPoolPackets, i);
        }

        delete[] minPkt->data;
        delete minPkt;

        return true;
    }

    void Queue::updateMemPoolValue() {
        // 调用该函数的时候保证外面加锁
        int minCapacity = INT32_MAX;
        int maxCapacity = 0;

        StruPacketList* loopPkt = m_pMemPoolFirstPtk;
        while (loopPkt){
            if(loopPkt->capacity > maxCapacity){
                m_iMemPoolMaxCapacity = maxCapacity;
            }

            if(loopPkt->capacity < minCapacity){
                minCapacity = loopPkt->capacity;
            }
            loopPkt = loopPkt->next;
        }

        m_iMemPoolMaxCapacity = maxCapacity;
        m_iMemPoolMinCapacity = minCapacity;
    }

    void Queue::finish() {
        pthread_mutex_lock(&m_mutex);
        m_bFinished = true;
        pthread_mutex_unlock(&m_mutex);
    }


    void Queue::start() {
        pthread_mutex_lock(&m_mutex);
        m_bFinished = false;
        pthread_mutex_unlock(&m_mutex);
    }

    int32_t Queue::getQueueSize() {
        int32_t ret = 0;
        pthread_mutex_lock(&m_mutex);
        ret = m_nPackets;
        pthread_mutex_unlock(&m_mutex);
        return ret;
    }
} //namespace vvav
