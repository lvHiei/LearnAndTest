//
// Created by mj on 16-2-19.
//

#ifndef MVBOX_NEW_QUEUE_H
#define MVBOX_NEW_QUEUE_H

#include <sys/types.h>
#include <pthread.h>

namespace vvav {
#define MAX_MEM_POOL_QUEUE_LENGTH  5

    typedef struct PacketList {
        void *data;            // 数据
        int32_t length;        // 数据长度
        int32_t capacity;      // 数据容量
        int64_t pts;           // 播放时长，用来音视频同步
        struct PacketList *next;
    } StruPacketList;

    class Queue {
    public:
        Queue();

        ~Queue();

        int32_t push(void *data, int32_t length, int64_t pos);

        int32_t pop(void *data, int32_t length, int64_t &pts);

        int32_t trypop(void *data, int32_t length, long timeOutwithms, int64_t &pts);

        int32_t trypopWithPts(void *data, int32_t length, long timeOutwithms, int64_t pts);

        void finish();

        void start();

        void clear();

        int32_t getQueueSize();

    private:
        // 获取内存池中的一个packet
        StruPacketList *getMemPoolPacket(int length);

        // 向内存池中添加一个packet,成功返回true，失败返回false
        bool addMemPoolPacket(StruPacketList *pkt);

        /**
         * @breif 获取最佳的buffer
         * @param length 想要的buffer长度
         * @return 最佳的buffer;如果没有找到则返回空
         */
        StruPacketList* getBestPacket(int length);

        /**
         * @breif 处理获取buffer之后的mem Pool
         * @param pkt 获取的packet
         * @param lastPkt 获取的packet的前一个packet
         */
        void handleGetPakcet(StruPacketList* pkt, StruPacketList* lastPkt);

        // 内部清空队列，不加锁
        void clearQueue();

        // 内部清空缓存队列，不加锁
        void clearMemPoolQueue();

    private:
        StruPacketList *m_pFirstPkt;        // 第一个pkt位置
        StruPacketList *m_pLastPkt;         // 最后一个pkt位置

        int32_t m_nPackets;                 // 队列里packet的个数

        StruPacketList *m_pMemPoolFirstPtk;    // 备用内存池的第一个packet地址
        StruPacketList *m_pMemPoolLastPtk;     // 备用内存池的最后一个packet地址
        int32_t m_iMemPoolMaxCapacity;           // 备用内存池内存最大长度
        int32_t m_iMemPoolMinCapacity;           // 备用内存池内存最小长度

        int32_t m_nMemPoolPackets;             // 备用内存池中内存块的个数

        bool m_bFinished;

        pthread_mutex_t m_mutex;
        pthread_cond_t m_cond;
    };


} //namespace vvav
#endif //MVBOX_NEW_QUEUE_H
