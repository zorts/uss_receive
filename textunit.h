// -*-C++-*-
#ifndef _XMIT_TEXTUNIT_H_
#define _XMIT_TEXTUNIT_H_

#include <stdint.h>
#include <stdexcept>
#include <string>
#include <vector>

class TextUnitValue {
public:
  inline uint16_t length() const {
    return length_;
  }

  inline const void* data() const {
    const char* base = (const char*) this;
    return base+sizeof(length_);
  }

  inline const TextUnitValue* next() const {
    const char* base = (const char*) this;
    return (const TextUnitValue*)(base+sizeof(length_)+length_);
  }

  inline TextUnitValue* next() {
    char* base = (char*) this;
    return (TextUnitValue*)(base+sizeof(length_)+length_);
  }

  static inline const TextUnitValue* at(const void* where) {
    return (const TextUnitValue*) where;
  }

  // Primarily for testing
  static TextUnitValue* at(void* where);
  static TextUnitValue* create(uint16_t size);
  static TextUnitValue* create(const char* source);
  static void destroy(TextUnitValue* what);
  void* data();
  void setLength(uint16_t length);
  
private:
  // Prohibit copy/assign; do not implement
  TextUnitValue(const TextUnitValue&);
  TextUnitValue& operator=(const TextUnitValue&);

  uint16_t length_;
};

typedef std::vector<std::string> StringVector;
class TextUnitValueIterator;
class TextUnit {
public:

  inline uint16_t key() const {
    return key_;
  }

  inline uint16_t number() const {
    return number_;
  }

  inline const TextUnitValue* firstTextUnitValue() const {
    const char* base = (const char*) this;
    return TextUnitValue::at(base+sizeof(key_)+sizeof(number_));
  }

  inline TextUnitValue* firstTextUnitValue() {
    char* base = (char*) this;
    return TextUnitValue::at(base+sizeof(key_)+sizeof(number_));
  }

  static inline const TextUnit* at(const void* where) {
    return (const TextUnit*) where;
  }

  const TextUnitValueIterator begin() const;
  const TextUnitValueIterator end() const;

  // Primarily for testing
  static TextUnit* create(uint16_t key, const StringVector& values);
  static void destroy(TextUnit* unit);
  void setKey(uint16_t key) {
    key_ = key;
  }
  void setNumber(uint16_t number) {
    number_ = number;
  }

private:
  // At the moment, it's not possible to create text units.
  TextUnit();

  // Prohibit copy/assign; do not implement
  TextUnit(const TextUnit&);
  TextUnit& operator=(const TextUnit&);

  uint16_t key_;
  uint16_t number_;
};

class TextUnitValueIterator {
public:
  TextUnitValueIterator(const TextUnit& base)
    : base_(&base)
    , pos_(0)
    , ptr_(base.firstTextUnitValue())
  {}

  TextUnitValueIterator(const TextUnitValueIterator& rhs)
    : base_(rhs.base_)
    , pos_(rhs.pos_)
    , ptr_(rhs.ptr_)
  {}

  TextUnitValueIterator& operator=(const TextUnitValueIterator& rhs) {
    base_ = rhs.base_;
    pos_ = rhs.pos_;
    ptr_ = rhs.ptr_;
    return *this;
  }

  inline const TextUnitValue& operator*() const {
    if (pos_ >= base_->number()) {
      throw std::out_of_range("TextUnitValue");
    }
    return *ptr_;
  }

  inline const TextUnitValue* operator->() const {
    return &(*(*this));
  }

  inline bool operator==(const TextUnitValueIterator& rhs) const {
    return (base_ == rhs.base_) && (pos_ == rhs.pos_);
  }

  inline bool operator!=(const TextUnitValueIterator& rhs) const {
    return !(*this == rhs);
  }

  inline TextUnitValueIterator& operator++() {
    if (pos_ >= base_->number()) {
      throw std::out_of_range("TextUnitValue");
    }
    ++pos_;
    ptr_ = ptr_->next();
    return *this;
  }
  
private:
  friend class TextUnit;

  TextUnitValueIterator(const TextUnit& base, uint16_t index, const TextUnitValue* ptr);

  const TextUnit* base_;
  uint16_t pos_;
  const TextUnitValue* ptr_;
};


#endif // _XMIT_TEXTUNIT_H_
