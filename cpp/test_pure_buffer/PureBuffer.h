//
// Created by mj on 18-3-29.
//

#ifndef KROOMAV_PUREBUFFER_H
#define KROOMAV_PUREBUFFER_H


#include <stdint.h>
namespace vvav {

    /**
     *  纯Buffer  不含线程同步
     *  只能在一个线程使用
     *
     *  注意 满了之后直接覆盖前面的数据
     */
    class PureBuffer {
    public:
        PureBuffer(int32_t capacity);

        PureBuffer();

        ~PureBuffer();

        void reset();
        void clear();
        int32_t size();

        bool isEmpty();

        void push(const void *buf, int32_t bufSize);

        int32_t pop(void *buf, int32_t bufSize);

    private:
        char *m_buffer;
        int32_t m_capacity;
        int32_t head;
        int32_t tail;
        bool mIsEmpty;
    };

} //namespace vvav


#endif //KROOMAV_PUREBUFFER_H
