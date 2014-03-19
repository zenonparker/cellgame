//          Matthew Avery Coder 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#ifndef UTIL_BUFFERS
#define UTIL_BUFFERS

#include <memory>
#include <vector>
#include <cstdint>
#include <tuple>
#include <iterator>
#include <iostream>
#include <util_static_logging.hpp>
#include <util_buffer.hpp>
#include <util_shared_buffer.hpp>


//buffer buffer layout
// |--used-------|--useable-----------------------------------|--used----|
//               |--buffered-----------------|--not_buffered--|
//               |--scanned--|--not_scanned--|
//                           |--remaining---------------------|
// ^buffer       ^begin      ^get            ^put             ^end       ^ebuffer

//A buffer sequence has a shared buffer, followed by a set of fixed size buffers and a shared end buffer
//There is lots of undefined behavior if not used correctly
namespace util {
namespace Buffer {

class Buffers {
public:
  Buffers() = default;
  Buffers(const Buffers &) = default;
  Buffers &operator=(const Buffers& other) = default;
  Buffers(SharedBuffer beginning);
  template<class T>
  Buffers(T other);
  size_t size_with_fixed_buffer() const;
  size_t size_with_variable_buffer() const;
  size_t data_only_size() const;
  void flush();
  void split_last_buffer();


  const std::vector<SharedBuffer> &shared_buffers() const;
  std::vector<SharedBuffer> &shared_buffers();

  const Buffer &back_buffer() const;
  Buffer &back_buffer();

  const SharedBuffer &back_shared_buffer() const;
  SharedBuffer &back_shared_buffer();
  
  std::shared_ptr<uint8_t> data_copy_one_piece() const;
  void print(std::ostream &os) const;

private:
  std::vector<SharedBuffer> buffers_;
};

inline const std::vector<SharedBuffer> &Buffers::shared_buffers() const { return buffers_; }
inline std::vector<SharedBuffer> &Buffers::shared_buffers(){ return buffers_; }

inline const SharedBuffer &Buffers::back_shared_buffer() const { return buffers_.back(); }
inline SharedBuffer &Buffers::back_shared_buffer() { return buffers_.back(); }

inline const Buffer &Buffers::back_buffer() const { return buffers_.back().buffer(); }
inline Buffer &Buffers::back_buffer() { return buffers_.back().buffer(); }
  
inline Buffers::Buffers(SharedBuffer beginning) {
  buffers_.emplace_back(std::move(beginning));
  LOG_TRACE("Buffers start with shared buffer");
}


template<class T>
inline Buffers::Buffers(T other) {
    auto size = other.size();
    auto temp = std::make_shared<T>(std::move(other));
    uint8_t* ptr = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(temp->data()));
    buffers_.emplace_back( SharedBuffer({temp,ptr},size,size) );
}

inline size_t Buffers::size_with_fixed_buffer() const {
  if (buffers_.size() > 0) {
    return buffers_.size() * buffers_[0].buffer().buffer_size();
  } else {
    return 0;
  }
}

inline size_t Buffers::size_with_variable_buffer() const {
  size_t size = 0;
  for (const auto & s : buffers_) {
    size += s.buffer().useable();
  }
  return size;
}

inline size_t Buffers::data_only_size() const {
  size_t size = 0;
  for (const auto & s : buffers_) {
    size += s.buffer().data_size();
  }
  return size;
}

inline void Buffers::flush() {
  LOG_TRACE("flushing buffers");
  if(buffers_.size() > 0) {
    buffers_ = { buffers_.back() };
    buffers_.back().buffer().rebase();
  }
}

inline void Buffers::split_last_buffer() {
  Buffer buffer = buffers_.back().buffer().split();
  buffers_.emplace_back(buffers_.back(), buffer);
}


//try and use this function once since each time
//this is called, a copy is made
inline std::shared_ptr<uint8_t> Buffers::data_copy_one_piece() const {
  LOG_DEBUG("in data_copy_one_piece");
  LOG_DEBUG("data only size: " << data_only_size());
  uint8_t *arr = new uint8_t[data_only_size()];
  uint8_t *start = arr;
  for (const auto & s : buffers_) {
    std::copy(s.buffer().data(), s.buffer().data() + s.buffer().data_size(), start);
    start += s.buffer().data_size();
  }
  //can't use make_shared - custom deleter
  return std::shared_ptr<uint8_t>(arr, std::default_delete<uint8_t[]>());

}

inline void Buffers::print(std::ostream &os) const {
  for (const auto & b : buffers_) {
    b.buffer().print(os);
  }
}

}
}

inline std::ostream &operator<<(std::ostream &os, const util::Buffer::Buffers &s)
{
  s.print(os);
  return os;
}
#endif
