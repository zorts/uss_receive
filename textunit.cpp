// -*-C++-*-
#include "textunit.h"

#include <string.h>
#include <algorithm>

TextUnitValue* TextUnitValue::at(void* where) {
  return (TextUnitValue*) where;
}

void* TextUnitValue::data() {
  char* base = (char*) this;
  return base+sizeof(length_);
}

TextUnitValue* TextUnitValue::create(uint16_t size) {
  char* space = new char[size+sizeof(length_)];
  TextUnitValue* result = at(space);
  result->length_ = size;
  memset(result->data(), 0, size);
  return result;
}

TextUnitValue* TextUnitValue::create(const char* source) {
  uint16_t size = (uint16_t) strlen(source);
  char* space = new char[size+sizeof(length_)];
  TextUnitValue* result = at(space);
  result->length_ = size;
  memcpy(result->data(), source, size);
  return result;
}

void TextUnitValue::destroy(TextUnitValue* what) {
  char* space = (char*)what;
  delete [] space;
}

void TextUnitValue::setLength(uint16_t length) {
  length_ = length;
}

const TextUnitValueIterator TextUnit::begin() const {
  return TextUnitValueIterator(*this);
}

const TextUnitValueIterator TextUnit::end() const {
  return TextUnitValueIterator(*this, 0, firstTextUnitValue());
}

TextUnitValueIterator::TextUnitValueIterator(const TextUnit& base, uint16_t index, const TextUnitValue* ptr)
  : base_(&base)
  , pos_(index)
  , ptr_(ptr)
{}

struct LengthSummation {
  size_t total_;
  LengthSummation() : total_(0) {}
  void operator()(const std::string& s) {
    total_ += s.size()+sizeof(uint16_t);
  }
};

TextUnit* TextUnit::create(uint16_t key, const StringVector& values) {
  LengthSummation len = std::for_each(values.begin(), values.end(), LengthSummation());
  size_t space = sizeof(TextUnit)+len.total_;
  TextUnit* result = (TextUnit*) (new char[space]);
  result->setKey(key);
  result->setNumber((uint16_t)values.size());
  TextUnitValue* value = result->firstTextUnitValue();
  for (StringVector::const_iterator it = values.begin(); it != values.end(); ++it) {
    value->setLength((uint16_t)it->size());
    memcpy(value->data(), it->data(), value->length());
    value = value->next();
  }
  return result;
}

void TextUnit::destroy(TextUnit* unit) {
  delete [] ((char*)unit);
}
