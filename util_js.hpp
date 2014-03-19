//          Matthew Avery Coder 2012 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#ifndef UTIL_JS
#define UTIL_JS


#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <memory>
#include <stack>
#include <string>
#include <type_traits>
#include <util_bitmask.hpp>
#include <util_buffered_allocator.hpp>
#include <util_buffers.hpp>
#include <util_swiss.hpp>
#include <utility>
#include <vector>
#include <cpp11_polyfill.hpp>


/**
 * @file
 * javascript parser that is pretty fast.
 * Does not do _any_ memory management
 * therefore any pointers passed through
 * the iterface assumes no ownership of the data
 */
namespace util {
namespace JS {

  class NodeMemory {
    public:
    NodeMemory(BufferedAllocator<uint8_t> &buffered_allocator);
  
    BufferedAllocator<uint8_t>& buffered_allocator_;
    size_t threshold_;
    std::list<Buffer::SharedBuffer> large_buffers_; 
    Buffer::Buffers buffers_;
  };

  inline std::shared_ptr<NodeMemory> defaultMemory(size_t num = 100, size_t size = 10000) {
    static BufferedAllocator<uint8_t> buffered_allocator(num,size);
    return std::make_shared<NodeMemory>(buffered_allocator);
  }

  /**
   * @brief the list of types a node can be.
   * undefined is technically not part of the spec but is handy if an 
   * error occurs or a node with no default was created
   * Ints don't exist either -- therefore checking
   * if (type_info::is_number | type) == true
   * will let you know if the number is numerical
   */
enum class type : uint8_t { Undefined, Null, Obj , Array, Str, Int, Real, Bool};

CONSTEXPR_PF bool has_children(const type &t) {
  return (t == type::Array || t == type::Obj);
}

CONSTEXPR_PF bool is_stringable(const type &t) {
  return (t != type::Undefined && t != type::Array && t != type::Obj);
}

CONSTEXPR_PF bool is_number(const type &t) {
  return (t == type::Int && t == type::Real);
}

// from http://stackoverflow.com/questions/7724448/simple-json-string-escape-for-c
inline std::string escape_json_string(const char * input) {
    std::ostringstream ss;
    while (*input != '\0') {
        switch (*input) {
            case '\\': ss << "\\\\"; break;
            case '"': ss << "\\\""; break;
            case '/': ss << "\\/"; break;
            case '\b': ss << "\\b"; break;
            case '\f': ss << "\\f"; break;
            case '\n': ss << "\\n"; break;
            case '\r': ss << "\\r"; break;
            case '\t': ss << "\\t"; break;
            default: ss << *input; break;
        }
        ++input;
    }
    return ss.str();
}

//TODO: not perfect.
//it assumes any array or pointer is a string
//good enough for now though
template<class T>
CONSTEXPR_PF JS::type auto_type(const T &t) {
  typedef typename std::remove_cv<T>::type T_no_cv;
  return std::is_integral<T>::value          ? JS::type::Int:
      ( std::is_floating_point<T>::value     ? JS::type::Real:
      ( std::is_same<bool, T_no_cv >::value  ? JS::type::Bool:
      ( (std::is_pointer<T>::value  || std::is_array<T>::value) ?  JS::type::Str :
         JS::type::Null)));
 //when C++14 is svailable replace with this:
 // return std::is_integral<T>::value          ? JS::type::Int:
 //     ( std::is_floating_point<T>::value     ? JS::type::Real:
 //     ( std::is_same<bool, T_no_cv >::value  ? JS::type::Bool:
 //     ( ((std::is_pointer<T>::value && std::is_null_pointer<T>::value ) || std::is_array<T>::value) ?  JS::type::Str :
 //        JS::type::Null)));

}

/**
 * @brief some internal properties that a node has
 */
enum class settings : uint8_t {None = 0, HasKey = 1 << 1, Sorted = 1 << 2};


/**
 * @brief class that defines a json object. Nodes can be nested
 */
class Node {
  public:
    //returns the end of the string, or the end iterator
    /**
     * @brief parse the JSON string that begins and ends with quotes(")
     *
     * @param begin start of the string
     * @param end maximum end of the string
     *
     * @return the end of the string
     */
    static const char* parse_string(const char* begin, const char* end);

    /**
     * @brief parse the JSON float number
     *
     * @param begin start of the float
     * @param end maximum end of the float
     * @param type outputs the type -- if the float can fit in a 
     * 64 bit interger, it returns integer
     *
     * @return the end of the float (or int)
     */
    static const char* parse_float(const char* begin, const char* end, type &type);

    /**
     * @brief prunes whitespace as defined by JSON
     *
     * @param c the character to analyze
     *
     * @return false if the character is whitespace, true otherwise
     */
    static bool is_token (const char c);

    /**
     * @brief parse data to a json document
     *
     * @param begin start of the data
     * @param end maximum end of the data
     * @param root output node that holds the document
     *
     * @return true if parsing succeeded, false otherwise
     */
    static bool parse(const uint8_t* begin, const uint8_t* end, Node &root);

    /**
     * @brief parse data to a json document
     *
     * @param begin start of the data
     * @param end maximum end of the data
     * @param root output node that holds the document
     *
     * @return true if parsing succeeded, false otherwise
     */
    static bool parse(const char* begin, const char* end, Node &root);
    
    /**
     * @brief print json to a stream
     *
     * @param out stream to print json
     * @param level indentation, 0 = no whitespace, !0 = pretty printing
     *
     * @return 
     */
    static std::ostream& print(std::ostream &out,const Node &node, size_t level = 0);


    /**
     * @brief sort keys that are part of an object, recursively
     */
    void sort_objects();
    
    
    /**
     * @brief print json to a stream
     *
     * @param out stream to print json
     * @param level indentation, 0 = no whitespace, !0 = pretty printing
     *
     * @return 
     */
    std::ostream& print(std::ostream &out, size_t level = 0) const;

    /**
     * @brief clear the key from the node
     *  this is useful when extracting a subset of an object
     *  that used to have a key to it that should be discarded
     *  as the node is no longer part of the object
     */
    void detach();

    /**
     * @brief clears the node
     */
    void reset();

    
    /**
     * @brief sets the properties of the node
     * like a copy constructor
     *
     * @param node the node to be copied
     */
    void set(const Node &node);
    
    /**
     * @brief default constructor that sets
     * the Node to an Undefined type and sets the memory pool
     */
    Node(std::shared_ptr<NodeMemory> memory = defaultMemory());

    /**
     * @brief underlying memory of the node
     *
     * @return returns a shared_ptr to the memory
     */
    std::shared_ptr<NodeMemory> memory() const;

    /**
     * @brief get type
     *
     * @return type
     */
    JS::type type() const;
    
    /**
     * @brief get begin of internal representation of the data
     *
     * @return internal begin
     */
    const char* begin() const;
    
    /**
     * @brief get begin of internal representation of the data
     *
     * @return internal begin
     */
    const char* begin() { return begin_;};


    /**
     * @brief get end of the internal representation of the data
     *
     * @return internal end
     */
    const char* end() const;


    /**
     * @brief size of the internal prepresentatio of the data
     *
     * @return size of the data representation
     */
    size_t size() const;

    /**
     * @brief get begin of internal representation of the key
     *
     * @return internal key begin
     */
    const char* key_begin() const;

    /**
     * @brief get end of internal representation of the key
     *
     * @return internal key end
     */
    const char* key_end() const;
    
    /**
     * @brief size of the internal prepresentatio of the key
     *
     * @return size of the key representation
     */
    size_t key_size() const;

    /**
     * @brief set node to an object
     */
    void make_obj();
    /**
     * @brief set node to an array
     */
    void make_array();
    /**
     * @brief set node to a string
     *
     * @param begin start of string
     * @param length string length
     */
    template<class T>
    void no_copy(const T begin, size_t length, JS::type type);
    /**
     * @brief set node to an int
     *
     * @param begin start of the string-formatted int
     * @param length length of the string-formatted int
     */

    /**
     * @brief set the key to the given string
     *
     * @param begin start of the key
     * @param length length of key
     */
    template<class T>
    void key_no_copy(const T begin, size_t length);

    template<size_t N>
    inline void no_copy(const char (&begin)[N], JS::type type);

    template<size_t N>
    void key_no_copy(const char (&begin)[N]);
    
    /**
     * @brief set the data
     *
     * @param begin start of the data 
     * @param length length of data
     */
    template<class T>
    void set_data(const T begin, size_t length);

    /**
     * @brief unset the key, like detach
     */
    void unset_key();
    

    /**
     * @brief return the children of an array or object
     *
     * @return vector of nodes that are children of an array or object
     */
    std::shared_ptr<std::vector<Node> > children() const;

    /**
     * @brief if this is an object, compares keys
     *
     * @param rhs node to compare keys with
     *
     * @return ordering of keys
     */
    bool operator <(const JS::Node& rhs) const;

    /**
     * returns a nullptr if the search didn't yield
     * a result
     */
    Node* operator[](const char *val);
    Node* operator[](size_t index);

    
    /**
     * @brief same as the bracket operators except easier 
     * to use with pointers
     *
     */
    Node* find(const char * val);
    Node* at(size_t index);


    
    /**
     * returns true on success of the action
     *
     */
    bool erase(const char * val);
    bool erase(size_t index);
    bool update(const char * val, const Node &node);
    bool update(size_t index,const Node &node);

    template<size_t N>
    bool add_no_copy(const char (&begin)[N], JS::type type);
    
    template<size_t N, typename ... Args>
    bool add_no_copy(const char (&begin)[N], JS::type type, const Args& ... args);

    bool add_no_copy(const char *begin, size_t length, JS::type type);

    //neither the key nor the value are copied.
    bool add_no_copy(const char* key, size_t key_length, const char *begin, size_t length, JS::type type);

    bool add(const Node &node);
    bool add(Node &&node);
    
    /**
     * @brief adds an empty node to an 
     * object or array. If it's an object
     * the element is added in an unsorted fashion.
     * If the key is known and in-order, use 
     * add_empty with the key specifier
     *
     * @return nullptr if can't add, pointer to 
     * the added node if successful
     */
    Node* add_empty();
    
    /**
     * @brief for objects only --
     * adds an empty node with 
     *
     * @param node node to add as a child node
     *  for an object.
     *
     * @return nullptr if can't add, pointer to 
     * the added node if successful
     */
    template<class T>
    Node* add_empty(const T key);

    /**
     * @brief check equality of keys with other node
     *
     * @param rhs other node to compare
     *
     * @return  true if keys are equal
     */

    bool keys_equal(const Node &rhs) const;
    /**
     * @brief check equality of keys with another key
     *
     * @param begin begin of other key
     * @param end end of other key
     *
     * @return return true if the other key and this node's key are equal
     */
    bool keys_equal(const char * begin, const char *end) const;


    /**
     * @brief determines if the node has the key set
     *
     * @return true if the node has the key set
     */
    bool has_key() const;

    /**
     * @brief determine if the object has sorted keys
     *
     * @return true if keys are sorted, false otherwise
     */
    bool sorted() const;
  
    template<class T>
    bool write(const T& value);
    
    template<class T, class U>
    bool write(const T& key, const U& value);

    //these 2 don't allocate
    bool write(const bool& value);
    bool write();

    //all these allocate
    template<typename K, typename V, typename ... Args>
    bool write_kv(const K &key,const V &val, const Args& ... args);

    template<typename K, typename V>
    bool write_kv(const K &key,const V &val);
    
    template<typename K>
    bool write_kv(const K &key);

    template<typename V, typename ... Args>
    bool write_vals(const V &val, const Args& ... args);


    template<typename V>
    bool write_vals(const V &val);
    
    bool write_vals();

    template<class T>
    bool write_key(const T&value);


    
    /**
     * @brief getters -- cast the internal value
     * of the node to the given type
     */
    char character() const;
    int8_t int8() const;
    uint8_t uint8() const;
    int16_t int16() const;
    uint16_t uint16() const;
    int32_t int32() const;
    uint32_t uint32() const;
    int64_t int64() const;
    uint64_t uint64() const;
    double real() const;
    std::string str() const;
    bool boolean() const;
    //this is for pointer conversion so instead of typint node_ptr->operator[]("thing") you can do node_ptr->in()["thing"]
    Node& in();
    std::vector<Node>& obj();
    std::vector<Node>& array();

  private:

    std::vector<Node>::iterator find_it(const char * val);
    std::shared_ptr<std::vector<Node> > children_;
    std::shared_ptr<NodeMemory > memory_;

    const char* begin_;
    const char* end_;
    const char* key_begin_;
    const char* key_end_;
    JS::settings settings_;
    JS::type type_;
};


//returns the end of the string, or the end iterator
inline const char* Node::parse_string(const char* begin, const char* end) {
  //stop when there's an escaped character or when the string ends
  const char* i = begin;
  if (begin >= end) { return end; }
  if(*i == '"') return i;
  if(++i == end) return end;
  if(*i == '"' && *(i-1) != '\\') return i;
  while( (i = std::find(i,end,'"')) != end  ) {
    const char* j = i;
    while(*(--j) == '\\'){}
    if( std::distance(j,i) % 2 ) {
      return i;
    }
    ++i;
  }
  return end;
}

//parses a number, including floats
//sets type to int or real
inline const char* Node::parse_float(const char* begin, const char* end, JS::type &type) {
  const char* i = begin;
  if(*i == '-') { ++i; }
  if(i == end || !isdigit(*i)) {
    type = JS::type::Undefined;
    return end;
  }
  ++i;
      
  type = JS::type::Int;
  while(isdigit(*i)) { 
    ++i;
    if(i == end) { return end; }
  }

  if(*i == '.') { 
    type = JS::type::Real;
    ++i;
    while(isdigit(*i)) { ++i; if(i == end) { return end; }  }
  }

  if(*i == 'e' || *i == 'E') {
    type = JS::type::Real;
    ++i;
    if(*i == '+' || *i == '-') { ++i; }
    while(isdigit(*i)) { ++i; if(i == end) return end; }
  }
  return i;
}

inline bool Node::is_token (const char c) {
  //bool ret = !(c == ' ' || c == '\n' || c == '\t' || c == '\r');
  //if(ret) std::cout << "`" << c << "`\n";
  //return ret;
  return !(c == ' ' || c == '\n' || c == '\t' || c == '\r');
}

inline bool Node::parse(const uint8_t* begin, const uint8_t* end, Node &root) {
  using util::Swiss::to_char;
  return parse(to_char(begin),to_char(end),root);
}


inline void Node::sort_objects() {
  if(has_children(type_)) {
    if(type_ == JS::type::Obj && !sorted() ) {
      std::sort(children_->begin(), children_->end());
      Bitmask::set(settings_,JS::settings::Sorted,true);
    }
    for(auto child:*children_) {
        child.sort_objects();
    }
  }
}


inline std::ostream& Node::print(std::ostream &out, size_t level) const{
  return Node::print(out,*this, level);
}


inline void Node::detach() {
  Bitmask::set(settings_, JS::settings::HasKey,false);
}

inline void Node::reset() {
  children_.reset();
  type_ = JS::type::Undefined;
  settings_ = JS::settings::None;
}

inline void Node::set(const Node &node) {
  type_ = node.type_;
  key_begin_ = node.key_begin_;
  key_end_ = node.key_end_;
  begin_ = node.begin_;
  end_ = node.end_;
  children_ = node.children_;
  settings_ = node.settings_;
}

inline Node::Node(std::shared_ptr<NodeMemory> memory):type_(JS::type::Undefined) ,settings_(JS::settings::None) ,memory_(memory) {
}

inline std::shared_ptr<NodeMemory> Node::memory() const{
  return memory_;
}

inline JS::type Node::type() const { return type_; }

inline const char* Node::begin() const { return begin_; }
inline const char* Node::end() const { return end_; }
inline size_t Node::size() const{ return static_cast<size_t>(std::distance(begin_,end_)); }
inline const char* Node::key_begin() const { return key_begin_; }
inline const char* Node::key_end() const { return key_end_; }
inline size_t Node::key_size() const{ return static_cast<size_t>(std::distance(key_begin_, key_end_)); }


inline std::shared_ptr<std::vector<Node> > Node::children() const{ return children_; }

inline bool Node::operator <(const JS::Node& rhs) const{
  return std::lexicographical_compare(
                  key_begin_,key_end_
                 ,rhs.key_begin(),rhs.key_end());
}

inline Node* Node::operator[](const char *val) {
  if(type_ == JS::type::Obj) {
    auto it = find_it(val);
    if(it != children_->end()) {
      return &*it;
    }
  }
  return nullptr;
}

inline Node* Node::operator[](size_t index) {
  if(type_ == JS::type::Array && index < children_->size()) {
      return &children_->operator[](index);
  }
  return nullptr;
}

inline Node* Node::find(const char * val) {
  return operator[](val);
}
inline Node* Node::at(size_t index) {
  return operator[](index);
}

inline bool Node::erase(const char * val) {
  if(type_ == JS::type::Obj) {
    auto it = find_it(val);
    if(it != children_->end()) {
      children_->erase(it);
      return true;
    }
    return false;
  }
}

inline bool Node::erase(size_t index) {
  if(type_ == JS::type::Array && index < children_->size()) {
    children_->erase(children_->begin() + index);
    return true;
  }
  return false;
}

template<size_t N>
bool Node::add_no_copy(const char (&begin)[N], JS::type type) {
  return add_no_copy(&begin[0],N-1,type);
}
    
template<size_t N, typename ... Args>
bool Node::add_no_copy(const char (&begin)[N], JS::type type, const Args& ... args) {
  if(add_no_copy(begin, type)) {
    return add_no_copy(args ...);
  } else {
    return false;
  }
}



inline bool Node::add_no_copy(const char *begin, size_t length, JS::type type) {
  if(type_ != JS::type::Array) {return false;}
  add_empty()->no_copy(begin,length,type);
  return true;
}

inline bool Node::add_no_copy(const char* key, size_t key_length, const char *begin, size_t length, JS::type type) {
  if(type_ != JS::type::Array) {return false;}
  Node *temp = add_empty();
  temp->key_no_copy(key, key_length);
  temp->no_copy(begin,length,type);
}

inline bool Node::add(const Node &node) {
  if(!node.has_key() && type_ == JS::type::Array) {
    children_->push_back(node);
    return true;
  } else if(node.has_key() && type_ == JS::type::Obj) {
    if(  sorted()
        && children_->size() > 0 
        && ( node < children_->back() )) {
      Bitmask::set(settings_,JS::settings::Sorted,false);
    }
    children_->push_back(node);
    return true;
  }

  return false;
}

inline bool Node::add(Node &&node) {
  if(!node.has_key() && type_ == JS::type::Array) {
    children_->emplace_back(std::move(node));
    return true;
  } else if(node.has_key() && type_ == JS::type::Obj) {
    if(  sorted()
        && children_->size() > 0 
        && ( node < children_->back() )) {
      Bitmask::set(settings_,JS::settings::Sorted,false);
    }
    children_->emplace_back(std::move(node));
    return true;
  }

  return false;
}

inline Node* Node::add_empty() {
  children_->emplace_back(memory_);
  if(type_ == JS::type::Obj && sorted()) {
    Bitmask::set(settings_,JS::settings::Sorted,false);
  }
  return &children_->back();
}

template<class T>
Node* Node::add_empty(const T key) {
  if(type_ != JS::type::Obj){ return nullptr; }
  children_->emplace_back(memory_);
  Node *ret = &children_->back();
  ret->write_key(key);
  if( sorted()
       && children_->size() > 0 
       && ( *ret < children_->back() )) {
     Bitmask::set(settings_,JS::settings::Sorted,false);
  }
  return ret;
}

inline bool Node::keys_equal(const Node &rhs) const {
  return std::lexicographical_compare(
                  key_begin_,key_end_
                 ,rhs.key_begin(),rhs.key_end());
}

inline bool Node::keys_equal(const char * begin, const char *end) const{
  //todo c++14 change this
  if(std::distance(begin,end) != std::distance(key_begin_, key_end_)) {
    return false;
  }
  return std::equal( key_begin_, key_end_, begin);
}


inline bool Node::has_key() const { return Bitmask::is_set(settings_, JS::settings::HasKey); }
inline bool Node::sorted() const { return Bitmask::is_set(settings_, JS::settings::Sorted); }
inline char Node::character() const{ return *begin_; }
inline int8_t Node::int8() const{ return static_cast<int8_t>(strtol(begin_,nullptr,10)); }
inline uint8_t Node::uint8() const { return static_cast<uint8_t>(strtoul(begin_, nullptr, 10)); }
inline int16_t Node::int16() const { return static_cast<int16_t>(strtol(begin_, nullptr, 10)); }
inline uint16_t Node::uint16() const { return static_cast<uint16_t>(strtoul(begin_, nullptr, 10)); }
inline int32_t Node::int32() const { return strtol(begin_,nullptr,10); }
inline uint32_t Node::uint32() const { return strtoul(begin_,nullptr,10); }
inline int64_t Node::int64() const { return strtoll(begin_,nullptr,10); }
inline uint64_t Node::uint64() const { return strtoull(begin_,nullptr,10); }
inline double Node::real() const { return strtod(begin_,nullptr); }
inline std::string Node::str() const { return std::string(begin_,end_); }
inline bool Node::boolean() const { return (*begin_ == 't' ) ? true : false; }
inline Node& Node::in() { return *this; }
inline std::vector<Node>& Node::obj() { return *children_; }
inline std::vector<Node>& Node::array() { return *children_; }

inline std::vector<Node>::iterator Node::find_it(const char * val) {
    if(!sorted()) {
      std::sort(children_->begin(), children_->end());
      Bitmask::set(settings_,JS::settings::Sorted,true);
    }
    auto val_end = val + strlen(val);
    const auto comp = [this,val_end](const Node& node, const char* val) {
          return std::lexicographical_compare(
                          node.key_begin_,node.key_end_
                         ,val,val_end);
        };
    auto it = std::lower_bound(children_->begin(),children_->end()
        ,val,comp);
    if(it == children_->end() || !it->keys_equal(val,val_end)) {
      return children_->end();
    } else {
      return it;
    }

}

inline void Node::make_obj() {
  type_ = JS::type::Obj;
  children_ = std::make_shared<std::vector<Node> >();
  Bitmask::set(settings_, JS::settings::Sorted, true);
}

inline void Node::make_array() {
  type_ = JS::type::Array;
  children_ = std::make_shared<std::vector<Node> >();
  Bitmask::set(settings_, JS::settings::Sorted, true);
}

template<class T>
inline void Node::no_copy(const T begin, size_t length, JS::type type) {
  type_ = type;
  begin_ = begin;
  end_ = begin + length;
}

template<class T>
void Node::key_no_copy(const T begin, size_t length) {
  key_begin_ = reinterpret_cast<const char *>(begin);
  key_end_ = reinterpret_cast<const char *>(begin + length);
  Bitmask::set(settings_, JS::settings::HasKey, true);
}

template<size_t N>
inline void Node::no_copy(const char (&begin)[N], JS::type type) {
  type_ = type;
  begin_ = &begin[0];
  end_ = &begin[N];
}

template<size_t N>
void Node::key_no_copy(const char (&begin)[N]) {
  key_begin_ = &begin[0];
  key_end_ = &begin[N];
  Bitmask::set(settings_, JS::settings::HasKey, true);
}

template<class T>
void Node::set_data(const T begin, size_t length) {
  begin_ = reinterpret_cast<const char *>(begin);
  end_ = reinterpret_cast<const char *>(begin + length);
}

inline void Node::unset_key() {
  Bitmask::set(settings_, JS::settings::HasKey, false);
}

inline std::ostream& Node::print(std::ostream &out,const Node &node, size_t level) {
  std::string indent = "";
  if(level) {
    indent = std::string(level,' ');
    level += 2;
  }
  if(  node.has_key()) {
    out << '"' << std::string(node.key_begin_,node.key_end_) << '"' << ":";
  }

  if(has_children(node.type_)) {
    out << ((node.type_ == JS::type::Obj) ? "{" : "[");
    auto n = node.children_->cbegin();
    while(n != node.children_->cend()){
        if(level) { out << '\n' << indent; }
        Node::print(out,*n,level);
        ++n;
      while(n != node.children_->cend()) {
        out << ",";
        if(level) { out << '\n' << indent; }
        Node::print(out,*n,level);
        ++n;
      }
    }

    if(level) { 
      out << '\n';
      level -= 4;
      indent = std::string(level,' ');
    }
    out << indent << ((node.type_ == JS::type::Obj) ? "}" : "]");
  } else if(is_stringable(node.type_)) {
      if(node.type_ == JS::type::Str) {
        out << '"' << std::string(node.begin_,node.end_) << '"';
      } else {
        out << std::string(node.begin_,node.end_);
      }
  }

  return out;
}

inline bool Node::parse(const char* begin, const char* end, Node &root) {
  //stop on [n]ull,[t]rue,[f]alse,["]string,number,array, or object
  struct ParentNode {
    std::vector<Node> *vector;
    size_t offset;
    JS::type type;
    Node& get() { return (*vector)[offset]; }
    ParentNode(std::vector<Node> *v, size_t o, JS::type t):
      vector(v)
      ,offset(o)
      ,type(t){ }
  };

  std::stack<ParentNode> parents;

  //for readability
  size_t depth = 0;

  Node node(root.memory());
  Node *parent = nullptr;

  const char* i = begin;
  while( (i = std::find_if(i,end,Node::is_token)) != end) {
    //root.print(std::cout); std::cout << std::endl;
    if(depth > 1 ) {
      parent = &parents.top().get();
    } else {
      parent = &root;
    }

    if(depth > 0) {
      if(*i == ',' ) {
        if( parent->children_->size() == 0 || ((i = std::find_if(++i,end,Node::is_token)) == end) || (*i == ',') ) {
          root.reset();
          THREAD_LOCAL_PF std::string error = "unexpected end in object, or no value between commas";
          root.begin_ = error.c_str();
          root.end_ = error.c_str() + error.size();
          return false;
        }
      } else if (    (*i == '}' && parent->type_ == JS::type::Obj)
                  || (*i == ']' && parent->type_ == JS::type::Array)) {
        if(depth == 1) {
          return true;
        } else {
          parents.pop();
          --depth;
          ++i;
        }
        continue;
      } else if (parent->children_->size() != 0) {
          root.reset();
          THREAD_LOCAL_PF std::string error = "No separator between elements";
          root.begin_ = error.c_str();
          root.end_ = error.c_str() + error.size();
          return false;
      }
    }

    node.reset();
    
    if (depth > 0 && parent->type_ == JS::type::Obj) {
      //parse the key and continue
      node.key_begin_ = ++i;
      if( (i = Node::parse_string(i,end)) == end) {
        root.reset();
        THREAD_LOCAL_PF std::string error = "key doesn't end";
        root.begin_ = error.c_str();
        root.end_ = error.c_str() + error.size();
        return false;
      }
          

      node.key_end_ = i;
      Bitmask::set(node.settings_,JS::settings::HasKey,true);
      //std::cout << "key: " << std::string(node.key_begin_, node.key_end_) << "`" << *i << "`\n";
      ++i;
      if( ((i = std::find_if(i,end,is_token)) == end ) 
          || (*i != ':') 
          || ((i = std::find_if(++i,end,is_token)) == end)
          || *i == '}' || *i == ']') {
        root.reset();
        THREAD_LOCAL_PF std::string error = "coulnd't find the begining of the value in object";
        root.begin_ = error.c_str();
        root.end_ = error.c_str() + error.size();
        return false;
      }
      
      if(  parent->sorted()
        && parent->children_->size() > 0 
        && ( node < parent->children_->back() )) {
      Bitmask::set(parent->settings_,JS::settings::Sorted,false);
    }

    }


    if ( *i == '{') {
      node.children_ = std::make_shared<std::vector<Node> >();
      Bitmask::set(node.settings_, JS::settings::Sorted, true);
      node.type_ = JS::type::Obj;
      ++i;
      if( depth == 0 ) {
        root.set(node);
      } else {
        parent->children_->emplace_back(std::move(node));
        parents.emplace(ParentNode(parent->children_.get()
              ,parent->children_->size() - 1
              ,parent->children_->back().type_));
        parent = &parents.top().get();
      }
      ++depth;
      continue;
    } else if(*i == '[') {
      node.children_ = std::make_shared<std::vector<Node> >();
      Bitmask::set(node.settings_, JS::settings::Sorted, true);
      node.type_ = JS::type::Array;
      ++i;
      if( depth == 0 ) {
        root.set(node);
      } else {
        parent->children_->emplace_back(std::move(node));
        parents.emplace(ParentNode(parent->children_.get()
              ,parent->children_->size() - 1
              ,parent->children_->back().type_));
      }
      ++depth;
      continue;
    } else if ( *i == 'n' ) {
      node.begin_ = i;
      if(i+4 > end || *++i != 'u' 
          || *++i != 'l' || *++i != 'l') {
        root.reset();
        THREAD_LOCAL_PF std::string error = "invalid primitive";
        root.begin_ = error.c_str();
        root.end_ = error.c_str() + error.size();
        return false;
      }
      node.end_ = ++i;
      node.type_ = JS::type::Null;
    } else if ( *i == 't') {
      node.begin_ = i;
      if(i+4 > end || *++i != 'r' 
          || *++i != 'u' || *++i != 'e') {
        root.reset();
        THREAD_LOCAL_PF std::string error = "invalid primitive";
        root.begin_ = error.c_str();
        root.end_ = error.c_str() + error.size();
        return false;
      }
      node.end_ = ++i;
      node.type_ = JS::type::Bool;
    } else if ( *i == 'f') {
      node.begin_ = i;
      if(i+5 > end || *++i != 'a' 
          || *++i != 'l' || *++i != 's' 
          || *++i != 'e') {
        root.reset();
        THREAD_LOCAL_PF std::string error = "invalid primitive";
        root.begin_ = error.c_str();
        root.end_ = error.c_str() + error.size();
        return false;
      }
      node.end_ = ++i;
      node.type_ = JS::type::Bool;
    } else if ( *i == '"') {
      node.begin_ = ++i;
      i = Node::parse_string(i,end);
      if(i == end) {
        root.reset();
        THREAD_LOCAL_PF std::string error = "invalid primitive";
        root.begin_ = error.c_str();
        root.end_ = error.c_str() + error.size();
        return false;
      }
      node.end_ = i;
      node.type_ = JS::type::Str;
      ++i;
    } else {
      node.begin_ = i;
      node.end_ = Node::parse_float(i,end,node.type_);
      i = node.end_;
      if( node.type_ == JS::type::Undefined ) {
        root.reset();
        THREAD_LOCAL_PF std::string error = "isn't parseable";
        root.begin_ = error.c_str();
        root.end_ = error.c_str() + error.size();
        return false;
      }
    }
    
    if( depth == 0 ) { 
      parent->set(node);
      return true; 
    }
    parent->children_->emplace_back(std::move(node));
  }

  //this code should never be reached in a valid json object
  root.reset();
  {
    THREAD_LOCAL_PF std::string error = "unexpected end";
    root.begin_ = error.c_str();
    root.end_ = error.c_str() + error.size();
  }
  return false;
}

inline NodeMemory::NodeMemory(BufferedAllocator<uint8_t> &buffered_allocator):
  buffered_allocator_(buffered_allocator),
  threshold_((buffered_allocator.subsize() * 5)/100){
  }


template<class T>
bool Node::write(const T& value) {
  size_t num_bytes_written = 0;

  if(memory_->buffers_.shared_buffers().size() == 0) {
    memory_->buffers_.shared_buffers().emplace_back(Buffer::SharedBuffer(memory_->buffered_allocator_.allocate_shared(),memory_->buffered_allocator_.subsize()));
  }

  auto &b = memory_->buffers_.back_buffer();
  //this statement is taken when the buffer is full and more memory
  //is needed
  if(!Swiss::to_buffer(b.put(), b.end(), value,  num_bytes_written)) {
    if(num_bytes_written < 0) {
      return false;
    } else {
      //probably just ran out of memory
      //num_bytes_written contains the number of bytes required for the data to be written
      if(num_bytes_written > b.not_buffered()) {
        if(num_bytes_written > memory_->buffered_allocator_.subsize()) {
          uint8_t *new_buffer = new uint8_t[num_bytes_written];
          memory_->large_buffers_.emplace_back(new_buffer,new_buffer+num_bytes_written);
          b = memory_->large_buffers_.back().buffer();
          if(!Swiss::to_buffer(b.put(), b.end(), value,  num_bytes_written)) {
            //todo erase the buffer
            return false;
          }
        } else {
          memory_->buffers_.shared_buffers().emplace_back(Buffer::SharedBuffer(memory_->buffered_allocator_.allocate_shared(),memory_->buffered_allocator_.subsize()));
          b = memory_->buffers_.back_buffer();
          if(!Swiss::to_buffer(b.put(), b.end(), value,  num_bytes_written)) {
            return false;
          }
        }
      }
    }
  }

  type_ = auto_type(value);

  b.putx(num_bytes_written);
  set_data(b.get(),b.not_scanned());
  b.getx(num_bytes_written);
  return true;
}

template<class T>
bool Node::write_key(const T& value) {
  size_t num_bytes_written = 0;

  if(memory_->buffers_.shared_buffers().size() == 0) {
    memory_->buffers_.shared_buffers().emplace_back(Buffer::SharedBuffer(memory_->buffered_allocator_.allocate_shared(),memory_->buffered_allocator_.subsize()));
  }

  auto &b = memory_->buffers_.back_buffer();
  //this statement is taken when the buffer is full and more memory
  //is needed
  if(!Swiss::to_buffer(b.put(), b.end(), value,  num_bytes_written)) {
    if(num_bytes_written < 0) {
      return false;
    } else {
      //probably just ran out of memory
      //num_bytes_written contains the number of bytes required for the data to be written
      if(num_bytes_written > b.not_buffered()) {
        if(num_bytes_written > memory_->buffered_allocator_.subsize()) {
          uint8_t *new_buffer = new uint8_t[num_bytes_written];
          memory_->large_buffers_.emplace_back(new_buffer,new_buffer+num_bytes_written);
          b = memory_->large_buffers_.back().buffer();
          if(!Swiss::to_buffer(b.put(), b.end(), value,  num_bytes_written)) {
            //todo erase the buffer
            return false;
          }
        } else {
          memory_->buffers_.shared_buffers().emplace_back(Buffer::SharedBuffer(memory_->buffered_allocator_.allocate_shared(),memory_->buffered_allocator_.subsize()));
          b = memory_->buffers_.back_buffer();
          if(!Swiss::to_buffer(b.put(), b.end(), value,  num_bytes_written)) {
            return false;
          }
        }
      }
    }
  }

  b.putx(num_bytes_written);
  key_no_copy(b.get(),b.not_scanned());
  b.getx(num_bytes_written);
  return true;
}
  
template<class T, class U>
bool Node::write(const T& key, const U& value) {
  return write_key(key) && write(value);
}

inline bool Node::write(const bool& val) {
  THREAD_LOCAL_PF const char true_str[] = "true";
  THREAD_LOCAL_PF const char false_str[] = "false";
  type_ = JS::type::Bool;
  if(val) {
    begin_ = true_str;
    end_ = true_str + sizeof(true_str) - 1;
  } else {
    begin_ = false_str;
    end_ = false_str + sizeof(false_str) - 1;
  }
  return true;
}

inline bool Node::write() {
  THREAD_LOCAL_PF const char null_str[] = "null";
  begin_ = &null_str[0];
  end_ = null_str + sizeof(null_str) - 1;
  type_ = JS::type::Null;
  return true;
}

inline bool Node::write_vals() {
  Node* temp = add_empty();
  if(temp == nullptr) { return false; }
  return temp->write();
}

template<typename V>
bool Node::write_vals(const V &val) {
  Node* temp = add_empty();
  if(temp == nullptr) { return false; }
  return temp->write(val);
}

template<typename V, typename ... Args>
bool Node::write_vals(const V &val,const Args& ... args) {
  Node* temp = add_empty();
  if(temp == nullptr) { return false; }
  if(temp->write(val)) {
    return write_vals(args ...);
  } else {
    return false;
  }
}

template<typename K, typename V>
bool Node::write_kv(const K &key,const V &val) {
  Node* temp = add_empty(key);
  if(temp == nullptr) { return false; }
  return temp->write(val);
}

template<typename K>
bool Node::write_kv(const K &key) {
  Node* temp = add_empty(key);
  if(temp == nullptr) { return false; }
  return temp->write();
}

template<typename K, typename V, typename ... Args>
bool Node::write_kv(const K &key,const V &val,const Args& ... args) {
  Node* temp = add_empty(key);
  if(temp == nullptr) { return false; }
  if(temp->write(val)) {
    return write_kv(args ...);
  } else {
    return false;
  }
}

}
}

inline std::ostream& operator<<(std::ostream &os, const util::JS::Node& node) {
  return node.print(os);
}

inline std::ostream& operator<<(std::ostream &os, const util::JS::type type) {
  if( type == util::JS::type::Null) {
    return os << "Null";
  } else if ( type == util::JS::type::Obj) {
    return os << "Obj";
  }
  else if ( type == util::JS::type::Array ) {
    return os << "Array";
  }
  else if ( type == util::JS::type::Str ) {
    return os << "Str";
  }
  else if ( type == util::JS::type::Int ) {
    return os << "Int";
  }
  else if ( type == util::JS::type::Real ) {
    return os << "Real";
  }
  else if ( type == util::JS::type::Bool ) {
    return os << "Bool";
  }
  else if ( type == util::JS::type::Undefined ) {
    return os << "Undefined";
  }
  return os << "enum_undefined";
}

#endif
