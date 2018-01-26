//
//  buf_manager.h
//  vsys
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//

#ifndef __vsys_buf_manager__ 
#define __vsys_buf_manager__ 

#include <sys/types.h>
#include <atomic>
#include <cassert>
#include <memory>
#include <limits>

#include "typedefs.h"
#include "log.h"

#ifndef CACHE_ALIGN
#define CACHE_ALIGN 64
#endif

static inline void* vsys_allocate(const vsys_size_t length){
    return length > 0 ? new char[length] : VSYS_NULL;
}

static inline vsys_float_t** vsys_allocate2(const vsys_uint_t row, const vsys_size_t length){
    vsys_float_t** p = new vsys_float_t*[row];
    vsys_float_t* temp = (vsys_float_t *)vsys_allocate(row * length * sizeof(vsys_float_t));
    if(!temp) return VSYS_NULL;
    for (vsys_uint_t i = 0; i < row; i++)
        p[i] = temp + i * length;
    return p;
}

static inline void vsys_free(void* p){
    if(p) delete[] (char *)p;
    p = VSYS_NULL;
}

static inline void vsys_free2(void** p){
    vsys_free(p[0]);
    vsys_free(p);
}

static inline vsys_bool_t vsys_reallocate(vsys_float_t** p,
                    const vsys_size_t ofset, const vsys_size_t size){    
    vsys_float_t* temp = (vsys_float_t *)vsys_allocate(size * sizeof(vsys_float_t));
    if(!temp) return VSYS_FALSE;
    memcpy(temp, *p, ofset * sizeof(vsys_float_t));
    vsys_free(*p);
    *p = temp;
   return VSYS_TRUE;
}

static inline vsys_bool_t vsys_reallocate2(vsys_float_t*** p,
                    const vsys_uint_t row, const vsys_size_t ofset, const vsys_size_t length){
    vsys_float_t** temp  = vsys_allocate2(row, length);
    if(!temp) return VSYS_FALSE;
    for (vsys_uint_t i = 0; i < row; i++)
        memcpy(temp[i], (*p)[i], ofset * sizeof(vsys_float_t));
    vsys_free2((void **)(*p));
    *p = temp;
    return VSYS_TRUE;
}

static inline vsys_bool_t put_buff(vsys_float_t** buff, vsys_size_t* ofset,
                    vsys_size_t* total_size, const vsys_float_t *data, const vsys_size_t length){
    if(!length) return VSYS_FALSE;
    if((length + *ofset) > *total_size){
        *total_size = length + *ofset;
        vsys_reallocate(buff, *ofset, *total_size);
    }
    memcpy(*buff + *ofset, data, length * sizeof(vsys_float_t));
    *ofset += length;
    return VSYS_TRUE;
}

static inline vsys_bool_t put_buff(vsys_float_t*** buff, const vsys_uint_t row,
                        vsys_size_t* ofset, vsys_size_t* total_size,
                        const vsys_float_t** data, const vsys_size_t length){
    if(!length) return VSYS_FALSE;
    if((length + *ofset) > *total_size){
        *total_size = length + *ofset;
        vsys_reallocate2(buff, row, *ofset, *total_size);
    }
    for (vsys_uint_t i = 0; i < row; i++)
        memcpy((*buff)[i] + *ofset, data[i], length * sizeof(vsys_float_t));
    *ofset += length;
    return VSYS_TRUE;
}
/*
 * ProducerConsumerQueue, borrowed from Ian NiLewis
 */
template <typename T>
class ProducerConsumerQueue {
public:
    explicit ProducerConsumerQueue(int size)
            : ProducerConsumerQueue(size, new T[size]) {}


    explicit ProducerConsumerQueue(int size, T* buffer)
            : size_(size), buffer_(buffer) {

        // This is necessary because we depend on twos-complement wraparound
        // to take care of overflow conditions.
        assert(size < std::numeric_limits<int>::max());
    }

    bool push(const T& item) {
        return push([&](T* ptr) -> bool {*ptr = item; return true; });
    }

    // get() is idempotent between calls to commit().
    T*getWriteablePtr() {
        T* result = nullptr;


        bool check  __attribute__((unused));//= false;

        check = push([&](T* head)-> bool {
            result = head;
            return false; // don't increment
        });

        // if there's no space, result should not have been set, and vice versa
        assert(check == (result != nullptr));

        return result;
    }

    bool commitWriteablePtr(T *ptr) {
        bool result = push([&](T* head)-> bool {
            // this writer func does nothing, because we assume that the caller
            // has already written to *ptr after acquiring it from a call to get().
            // So just double-check that ptr is actually at the write head, and
            // return true to indicate that it's safe to advance.

            // if this isn't the same pointer we got from a call to get(), then
            // something has gone terribly wrong. Either there was an intervening
            // call to push() or commit(), or the pointer is spurious.
            assert(ptr == head);
            return true;
        });
        return result;
    }

    // writer() can return false, which indicates that the caller
    // of push() changed its mind while writing (e.g. ran out of bytes)
    template<typename F>
    bool push(const F& writer) {
        bool result = false;
        int readptr = read_.load(std::memory_order_acquire);
        int writeptr = write_.load(std::memory_order_relaxed);

        // note that while readptr and writeptr will eventually
        // wrap around, taking their difference is still valid as
        // long as size_ < MAXINT.
        int space = size_ - (int)(writeptr - readptr);
        if (space >= 1) {
            result = true;

            // writer
            if (writer(buffer_.get() + (writeptr % size_))) {
                ++writeptr;
                write_.store(writeptr, std::memory_order_release);
            }
        }
        return result;
    }
    // front out the queue, but not pop-out
    bool front(T* out_item) {
        return front([&](T* ptr)-> bool {*out_item = *ptr; return true;});
    }

    void pop(void) {
        int readptr = read_.load(std::memory_order_relaxed);
        ++readptr;
        read_.store(readptr, std::memory_order_release);
    }

    template<typename F>
    bool front(const F& reader) {
        bool result = false;

        int writeptr = write_.load(std::memory_order_acquire);
        int readptr = read_.load(std::memory_order_relaxed);

        // As above, wraparound is ok
        int available = (int)(writeptr - readptr);
        if (available >= 1) {
            result = true;
            reader(buffer_.get() + (readptr % size_));
        }

        return result;
    }
    uint32_t size(void) {
        int writeptr = write_.load(std::memory_order_acquire);
        int readptr = read_.load(std::memory_order_relaxed);

        return (uint32_t)(writeptr - readptr);
    }

private:
    int size_;
    std::unique_ptr<T> buffer_;

    // forcing cache line alignment to eliminate false sharing of the
    // frequently-updated read and write pointers. The object is to never
    // let these get into the "shared" state where they'd cause a cache miss
    // for every write.
    alignas(CACHE_ALIGN) std::atomic<int> read_ { 0 };
    alignas(CACHE_ALIGN) std::atomic<int> write_ { 0 };
};

struct sample_buf {
    uint8_t    *buf_;       // audio sample container
    uint32_t    cap_;       // buffer capacity in byte
    uint32_t    size_;      // audio sample size (n buf) in byte
};

using AudioQueue = ProducerConsumerQueue<sample_buf*>;

__inline__ void releaseSampleBufs(sample_buf* bufs, uint32_t& count) {
    if(!bufs || !count) {
        return;
    }
    for(uint32_t i=0; i<count; i++) {
        if(bufs[i].buf_) delete [] bufs[i].buf_;
    }
    delete [] bufs;
}
__inline__ sample_buf *allocateSampleBufs(uint32_t count, uint32_t sizeInByte){
    if (count <= 0 || sizeInByte <= 0) {
        return nullptr;
    }
    sample_buf* bufs = new sample_buf[count];
    assert(bufs);
    memset(bufs, 0, sizeof(sample_buf) * count);

    uint32_t allocSize = (sizeInByte + 3) & ~3;   // padding to 4 bytes aligned
    uint32_t i ;
    for(i =0; i < count; i++) {
        bufs[i].buf_ = new uint8_t [allocSize];
        if(bufs[i].buf_ == nullptr) {
            ALOGW("====Requesting %d buffers, allocated %d in %s",count, i,  __FUNCTION__);
            break;
        }
        bufs[i].cap_ = sizeInByte;
        bufs[i].size_ = 0;        //0 data in it
    }
    if(i < 2) {
        releaseSampleBufs(bufs, i);
        bufs = nullptr;
    }
    count = i;
    return bufs;
}

#endif /* __vsys_buf_manager__ */
