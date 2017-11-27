#include "textunit.h"

#include <string.h>

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

const TextUnitValueIterator TextUnitBase::begin() const {
  return TextUnitValueIterator(*this);
}

const TextUnitValueIterator TextUnitBase::end() const {
  return TextUnitValueIterator(*this, 0, firstTextUnit());
}

TextUnitValueIterator::TextUnitValueIterator(const TextUnitBase& base, uint16_t index, const TextUnitValue* ptr)
  : base_(&base)
  , pos_(index)
  , ptr_(ptr)
{}
