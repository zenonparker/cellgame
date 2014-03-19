//          Matthew Avery Coder 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#ifndef UTIL_BUFFERED_ALLOCATOR
#define UTIL_BUFFERED_ALLOCATOR

#include <atomic>
#include <cstdint>
#include <cstddef>
#include <util_static_logging.hpp>
#include <memory>

namespace util {
//devilishly simple yet effective class. This lockless thread-safe (atomics!) class
//will pre-allocate 'size' amount of memory chunks that are each of size 'subsize'
//every time allocate is called, one of the chunks is pulled from the pool
//every time deallocate is called, one of the chunks is put back in the pool
//if the pool is fully used, allocate starts grabbing from the heap with a sytem call to new
//if deallocate is called when a heap-memory chunk is being released, delete will be called
//This does several things:
//  -limits context switches
//  -decreases the number of system calls
//  -prevents memory fragmentation,
//  -speeds up allocation/deallocation tremendously since it's no longer the system managing the memory
//


  /**
   * @brief Custom Buffered Allocatod that pre-allocates a large amount of memory that can than be allocated/deallocated efficiently.Thread-safe
   *
   * @tparam T the data type that the buffered allocator pre-allocates
   */
template<class T>
class BufferedAllocator {
public:

  /**
   * @brief Pre-allocates the data up-front
   *
   * @param size the total number of objects to pre-allocate for
   * @param subsize set to 1 if arrays of objects aren't needed.This determines the array size of each allocated object
   */
  BufferedAllocator(size_t size, size_t subsize);


  /**
   * @brief deallocate the object at the given memory location
   *
   * @param i memory location
   */
  void deallocate(T *i);

  /**
   * @brief allocate memory for the given object. If not enough memory is in the buffer, allocate from the heap
   *
   * @return pointer to the allocated memory
   */
  T *allocate();

  /**
   * @brief allocate memory for the given object. If not enough memory is in the buffer, allocate from the heap
   *
   * @return shared pointer to the allocated memory
   */
  std::shared_ptr<T> allocate_shared();


  /**
   * @brief size of the allocated object arrays
   *
   * @return size of the allocated object arrays
   */
  size_t subsize();

  ~BufferedAllocator();

private:
  T **queue_;
  T *start_;
  T *end_;
  std::atomic<int64_t> delta_;
  std::atomic<int64_t> read_;
  std::atomic<int64_t> write_;
  size_t  size_;
  size_t  subsize_;

};

template<class T>
size_t BufferedAllocator<T>::subsize() {
  return subsize_;
}

template<class T>
BufferedAllocator<T>::BufferedAllocator(size_t size, size_t subsize) :
  size_(size),
  subsize_(sizeof(T) * subsize)

{
  LOG_INFO("Buffered allocator initialized with size" << size << ", each of which are of size" << subsize);
  start_ = new T[size * subsize_];
  //todo remove this, force memory 
  memset(start_,0,size * subsize * sizeof(T));
  end_ =  start_ + size * subsize_;
  
  queue_ = new T*[size];
  for (size_t i = 0; i < size; ++i) {
    queue_[i] = start_ + (i * subsize_);
  }
  delta_ = static_cast<int64_t>(size_);
  read_ = 0;
  write_ = 0;
}

template<class T>
void BufferedAllocator<T>::deallocate(T *i) {
  if (i >= start_ && i < end_) {
    LOG_TRACE("dealloating from pool");
    int64_t mywrite = write_++;
    queue_[mywrite % size_] = i;
    ++delta_;
  } else {
    LOG_TRACE("dealloating from heap");
    delete [] i;
  }
}

template<class T>
T *BufferedAllocator<T>::allocate() {
  int64_t mydelta = --delta_;
  if (mydelta >= 0) {
    return queue_[read_++ % size_];
  } else {
    ++delta_;
    LOG_INFO("no longer in pool");
    return new T[subsize_];
  }
}

template<class T>
std::shared_ptr<T> BufferedAllocator<T>::allocate_shared() {
  int64_t mydelta = --delta_;
  if (mydelta >= 0) {
    LOG_TRACE("allocating from pool");
    return std::shared_ptr<T>(queue_[read_++ % size_],[this](T* ptr){deallocate(ptr);});
  } else {
    ++delta_;
    LOG_INFO("no longer in pool");
    return std::shared_ptr<T>(new T[subsize_],std::default_delete<T[]>());
  }
}

template<class T>
BufferedAllocator<T>::~BufferedAllocator() {
  if (start_ != 0) {
    delete [] start_;
    delete [] queue_;
  }
}

}

#endif
