//          Matthew Avery Coder 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#ifndef UTIL_BUFFER
#define UTIL_BUFFER

#include <memory>
#include <vector>
#include <tuple>
#include <iterator>
#include <iomanip>
#include <iostream>
#include <util_static_logging.hpp>
#include <cpp11_polyfill.hpp>


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

class Buffer {
public:
  Buffer();
  Buffer(const Buffer &other) = default;
  Buffer &operator=(const Buffer &other) = default;
  Buffer(Buffer &&other) /*= default*/;
  Buffer &operator=(Buffer &&other) /*= default*/;
  void print(std::ostream &os) const;

  Buffer(uint8_t *buffer, uint8_t *ebuffer);

  Buffer(uint8_t *buffer, uint8_t *ebuffer, uint8_t *useable,
         uint8_t *useable_end);

  Buffer(uint8_t *buffer, uint8_t *ebuffer, uint8_t *useable,
         uint8_t *useable_end, uint8_t *get, uint8_t *put);

  Buffer split();
  void rebase();
  size_t relocate_not_scanned(uint8_t *buffer);
  void set_header_data_split(uint8_t *split);
  
  //advanve put by x characters/bytes
  void putx(size_t num);

  //advance get by x characters/bytes
  void getx(size_t num);

  //Accessors
  //see the chart at the beginning of the header
  //for the meaning of these values
  size_t useable() const;
  size_t buffered() const;
  size_t not_buffered() const;
  size_t scanned() const;
  size_t not_scanned() const;
  size_t remaining() const;

  size_t buffer_size() const;
  size_t header_size() const;
  size_t data_size() const;

  uint8_t *header();
  uint8_t *header() const;

  uint8_t *data();
  uint8_t *data() const;


  uint8_t *buffer();
  uint8_t *buffer() const;

  uint8_t *ebuffer();
  uint8_t *ebuffer() const;

  uint8_t *begin();
  uint8_t *begin() const;

  uint8_t *end();
  uint8_t *end() const;

  uint8_t *get();
  uint8_t *get() const;

  uint8_t *put();
  uint8_t *put() const;


  virtual ~Buffer();
private:
  uint8_t *buffer_;
  uint8_t *ebuffer_;
  uint8_t *begin_;
  uint8_t *end_;
  uint8_t *get_;
  uint8_t *put_;
  uint8_t *header_data_split_;
};




inline Buffer::Buffer():
    buffer_(0), ebuffer_(0),
    begin_(0), end_(0),
    get_(0), put_(0),
    header_data_split_(0) { }
  
inline void Buffer::print(std::ostream &os) const {
 os << "|--used-" << begin_ - buffer_ << "------|--useable" << useable() << "--------------------------------------------|--used-" << ebuffer_ - end_ << "---|\n"
    << "                   |--buffered-" << buffered() << "---------------------|--not_buffered-" << not_buffered() << "-|\n"
    << "                   |--scanned-" << scanned() << "-|--not_scanned-" << not_scanned() << "-|\n"
    << "                                    |--remaining-" << remaining() << "-------------------------|\n"
    << "^buffer            ^begin           ^get                 ^put                  ^end            ^ebuffer\n";
}

inline Buffer Buffer::split() {
  uint8_t *temp_end = end_;
  uint8_t *temp_put = put_;
  end_ = get_;
  put_ = get_;
  LOG_TRACE("Buffer splitting");
  return Buffer(buffer_, ebuffer_, get_, temp_end, get_, temp_put);
}

inline Buffer::Buffer(uint8_t *buffer, uint8_t *ebuffer):
    buffer_(buffer), ebuffer_(ebuffer),
    begin_(buffer), end_(ebuffer),
    get_(buffer), put_(buffer),
    header_data_split_(buffer) { }

inline Buffer::Buffer(uint8_t *buffer, uint8_t *ebuffer, uint8_t *useable,
         uint8_t *useable_end):
    buffer_(buffer), ebuffer_(ebuffer),
    begin_(useable), end_(useable_end),
    get_(useable), put_(useable),
    header_data_split_(useable) { }

inline Buffer::Buffer(uint8_t *buffer, uint8_t *ebuffer, uint8_t *useable,
         uint8_t *useable_end, uint8_t *get, uint8_t *put):
    buffer_(buffer), ebuffer_(ebuffer),
    begin_(useable), end_(useable_end),
    get_(get), put_(put),
    header_data_split_(begin_) { }


inline void Buffer::rebase() {
    begin_ = get_;
    header_data_split_ = get_;
}

inline size_t Buffer::relocate_not_scanned(uint8_t *buffer) {
    std::copy(get_, put_, buffer);

    size_t ret = static_cast<size_t>(std::distance(get_, put_));
    put_ = get_;
    end_ = get_;
    return ret;
    LOG_TRACE("Buffer relocating");
}

inline void Buffer::putx(size_t num) { put_ += num; }
inline void Buffer::getx(size_t num) { get_ += num; }

inline size_t Buffer::scanned() const { return static_cast<size_t>(std::distance(begin_, get_)); }
inline size_t Buffer::not_scanned() const { return static_cast<size_t>(std::distance(get_, put_)); }
inline size_t Buffer::remaining() const { return static_cast<size_t>(std::distance(get_, end_)); }
inline size_t Buffer::not_buffered() const { return static_cast<size_t>(std::distance(put_, end_)); }
inline size_t Buffer::useable() const { return static_cast<size_t>(std::distance(begin_, end_)); }
inline size_t Buffer::buffered() const { return static_cast<size_t>(std::distance(begin_, put_)); }

inline void Buffer::set_header_data_split(uint8_t *split) { header_data_split_ = split; }
inline size_t Buffer::buffer_size() const { return static_cast<size_t>(std::distance(buffer_, ebuffer_)); }
inline size_t Buffer::data_size() const { return static_cast<size_t>(std::distance(header_data_split_, get_)); }
inline size_t Buffer::header_size() const { return static_cast<size_t>(std::distance(begin_, header_data_split_)); }

inline uint8_t *Buffer::data() { return header_data_split_; }
inline uint8_t *Buffer::data() const { return header_data_split_; }

inline uint8_t *Buffer::buffer() { return buffer_; }
inline uint8_t *Buffer::buffer() const { return buffer_; }

inline uint8_t *Buffer::ebuffer() { return ebuffer_; }
inline uint8_t *Buffer::ebuffer() const { return ebuffer_; }

inline uint8_t *Buffer::begin() { return begin_; }
inline uint8_t *Buffer::begin() const { return begin_; }

inline uint8_t *Buffer::end() { return end_; }
inline uint8_t *Buffer::end() const { return end_; }

inline uint8_t *Buffer::get() { return get_; }
inline uint8_t *Buffer::get() const { return get_; }

inline uint8_t *Buffer::put() { return put_; }
inline uint8_t *Buffer::put() const { return put_; }

inline uint8_t *Buffer::header() { return begin_; }
inline uint8_t *Buffer::header() const { return begin_; }

inline Buffer::~Buffer() {}

}
}

inline std::ostream &operator<<(std::ostream &os, const util::Buffer::Buffer &s)
{
  s.print(os);
  return os;
}

#endif
