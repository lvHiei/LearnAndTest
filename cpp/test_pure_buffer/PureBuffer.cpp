//
// Created by mj on 18-3-29.
//

#include <malloc.h>
#include <string.h>
#include "PureBuffer.h"


namespace vvav {

#define DEFULT_BUFFER_SIZE 4096*20


    PureBuffer::PureBuffer(int32_t capacity) :
            m_buffer(NULL), m_capacity(capacity), head(0), tail(0), mIsEmpty(true) {
        m_buffer = (char *) malloc(m_capacity);
    }

    PureBuffer::PureBuffer()  :
            m_buffer(NULL), m_capacity(DEFULT_BUFFER_SIZE), head(0), tail(0), mIsEmpty(true){
        m_buffer = (char *) malloc(m_capacity);
    }


    PureBuffer::~PureBuffer() {
        m_capacity = 0;
        free(m_buffer);
        m_buffer = NULL;
        head = 0, tail = 0;
    }

    void PureBuffer::reset() {
        clear();
    }

    void PureBuffer::clear() {
        head = tail = 0;
        mIsEmpty = true;
    }

    int32_t PureBuffer::size() {
        int32_t res = 0;
        int32_t dur = tail - head;
        if (dur > 0) {
            res = dur;
        } else if (dur < 0) {
            res = m_capacity + dur;
        } else {
            res = isEmpty() ? 0 : m_capacity;
        }
        return res;
    }

    bool PureBuffer::isEmpty() {
        return mIsEmpty;
    }

    void PureBuffer::push(const void *vbuf, int32_t bufSize) {
        if (bufSize <= 0 || NULL == vbuf) {
            return;
        }

        char* buf = (char*) vbuf;

        int32_t subsize = 0;
        while (bufSize != 0) {
            if (tail >= head) {
                subsize = m_capacity - tail > bufSize ? bufSize : m_capacity - tail;
            } else {
                subsize = head - tail > bufSize ? bufSize : head - tail;
            }
            memcpy(m_buffer + tail, buf, subsize);
            tail = (tail + subsize) % m_capacity;
            bufSize -= subsize;
            buf += subsize;
            mIsEmpty = false;
        }
    }

    int32_t PureBuffer::pop(void *vbuf, int32_t bufSize) {
        int32_t ret = 0;
        int32_t subsize = 0;
        char* buf = (char*) vbuf;
        while (bufSize > 0) {
            if (tail > head) {
                subsize = tail - head > bufSize ? bufSize : tail - head;
            } else {
                subsize = m_capacity - head > bufSize ? bufSize : m_capacity - head;
            }
            memcpy(buf + ret, m_buffer + head, subsize);
            ret += subsize;
            bufSize -= subsize;
            head = (head + subsize) % m_capacity;
            if (head == tail) {
                mIsEmpty = true;
                break;
            }
        }
        return ret;
    }


} // namespace vvav