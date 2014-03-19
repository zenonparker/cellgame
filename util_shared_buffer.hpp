//          Matthew Avery Coder 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#ifndef UTIL_SHARED_BUFFER
#define UTIL_SHARED_BUFFER

#include <memory>
#include <vector>
#include <cstdint>
#include <tuple>
#include <iterator>
#include <iostream>
#include <util_static_logging.hpp>

#include <util_buffer.hpp>

//buffer buffer layout
// |--used-------|--useable-----------------------------------|--used----|
//               |--buffered-----------------|--not_buffered--|
//               |--scanned--|--not_scanned--|
//                           |--remaining---------------------|
// ^buffer       ^begin      ^get            ^put             ^end       ^ebuffer

namespace util {
namespace Buffer {


class SharedBuffer {
public:
  SharedBuffer() = default;
  SharedBuffer(std::shared_ptr<uint8_t> data, size_t size, size_t amount_read = 0);
  SharedBuffer(uint8_t *begin, uint8_t *end);
  SharedBuffer(const Buffer &other);
  SharedBuffer(const SharedBuffer &other);
  SharedBuffer(SharedBuffer &&other);

  /**
   * @brief constructor used when the underlying memory comes from
   *        a shared buffer, but the tracking pointers come from a buffer
   *        object. Only use it if you know what you're doing.
   *        Dangerous yet powerful.
   *
   * @param other shared -- will increase the ref count
   * @param buffer buffer with the status of the shared pointer
   */
  SharedBuffer(const SharedBuffer &other, const Buffer &buffer);

  SharedBuffer &operator=(SharedBuffer && other);

  SharedBuffer &operator=(const SharedBuffer &other);
  std::shared_ptr<uint8_t> underlying_memory();
  void reset_underlying_memory();
  size_t ref_count();
  const Buffer& buffer() const;
  Buffer& buffer();
  ~SharedBuffer();
private:
  Buffer buffer_;
  std::shared_ptr<uint8_t> underlying_memory_;
};
  
inline const Buffer& SharedBuffer::buffer() const { return buffer_; }

inline Buffer& SharedBuffer::buffer() { return buffer_; }
  
inline SharedBuffer::SharedBuffer(std::shared_ptr<uint8_t> data, size_t size, size_t amount_read):
  buffer_(data.get(), data.get() + size
        ,data.get(), data.get() + size
        ,data.get() + amount_read
        ,data.get() + amount_read)
  ,underlying_memory_(data) {
}

inline SharedBuffer::SharedBuffer(uint8_t *begin, uint8_t *end):
  buffer_(begin, end),
  underlying_memory_(begin, std::default_delete<uint8_t[]>()) {
}

inline SharedBuffer::SharedBuffer(const Buffer &other):
  buffer_(other),
  underlying_memory_(other.buffer(), std::default_delete<uint8_t[]>()) {
}

inline SharedBuffer::SharedBuffer(const SharedBuffer &other):
  buffer_(other.buffer()),
  underlying_memory_(other.underlying_memory_) {
}

/**
 * @brief constructor used when the underlying memory comes from
 *        a shared buffer, but the tracking pointers come from a buffer
 *        object. Only use it if you know what you're doing.
 *        Dangerous yet powerful.
 *
 * @param other shared -- will increase the ref count
 * @param buffer buffer with the status of the shared pointer
 */
inline SharedBuffer::SharedBuffer(const SharedBuffer &other, const Buffer &buffer):
  buffer_(buffer),
  underlying_memory_(other.underlying_memory_)
{
}

inline SharedBuffer::SharedBuffer(SharedBuffer &&other):
  buffer_(other.buffer()),
  underlying_memory_(std::move(other.underlying_memory_)) {
}

inline SharedBuffer &SharedBuffer::operator=(SharedBuffer && other) {
  buffer_ = other.buffer();
  underlying_memory_ = std::move(other.underlying_memory_);
  return *this;
}

inline SharedBuffer &SharedBuffer::operator=(const SharedBuffer &other) {
  buffer_ = other.buffer();
  underlying_memory_ = other.underlying_memory_;
  return *this;
}

inline std::shared_ptr<uint8_t> SharedBuffer::underlying_memory() {
  return underlying_memory_;
}

inline void SharedBuffer::reset_underlying_memory() {
  underlying_memory_.reset();
}

inline size_t SharedBuffer::ref_count() {
  return underlying_memory_.use_count();
}

inline SharedBuffer::~SharedBuffer() { }

}
}

inline std::ostream &operator<<(std::ostream &os,
                         const util::Buffer::SharedBuffer &s)
{
  s.buffer().print(os);
  return os;
}

#endif
