// -*-C++-*-
#include "textunits.h"

#include <iostream>
#include <iomanip>
#include <cassert>
#include <string.h>

StringTextUnitDescrBase::StringTextUnitDescrBase(uint16_t key,
                                                 const char* mnemonic,
                                                 const char* label,
                                                 bool singleton)
  : TextUnitDescrBase(key, mnemonic, "string", label, singleton)
{}

void StringTextUnitDescrBase::display(std::ostream& s, const TextUnit* unit) const {
  assert(unit->key() == key_);
  s << label() << ": ";
  if (isSingleton() || (unit->number() == 1)) {
    assert(unit->number() == 1);
    s << value(unit->firstTextUnitValue());
  } else {
    s << "(";
    bool first = true;
    for (TextUnitValueIterator it = unit->begin(); it != unit->end(); ++it) {
      if (!first) {
        s << ", ";
      } else {
        first = false;
      }
      s << value(&(*it));
    }
    s << ")";
  }
}

std::string StringTextUnitDescrBase::value(const TextUnitValue* v) const
{
  return std::string((const char*)(v->data()), v->length());
}

std::string StringTextUnitDescrBase::value(const TextUnit* u) const
{
  return value(u->firstTextUnitValue());
}

NumericTextUnitDescrBase::NumericTextUnitDescrBase(uint16_t key,
                                                   const char* mnemonic,
                                                   const char* label,
                                                   bool hex,
                                                   bool singleton)
  : TextUnitDescrBase(key, mnemonic, "numeric", label, singleton)
  , hex_(hex)
{}

void NumericTextUnitDescrBase::display(std::ostream& s, const TextUnit* unit) const {
  assert(unit->key() == key_);
  s << label() << ": " << (hex_ ? std::hex : std::dec);
  if (isSingleton() || (unit->number() == 1)) {
    assert(unit->number() == 1);
    s << value(unit->firstTextUnitValue());
  } else {
    s << "(";
    bool first = true;
    for (TextUnitValueIterator it = unit->begin(); it != unit->end(); ++it) {
      if (!first) {
        s << ", ";
      } else {
        first = false;
      }
      s << value(&(*it));
    }
    s << ")" << std::dec;
  }
}

uint64_t NumericTextUnitDescrBase::value(const TextUnitValue* v) const
{
  uint64_t result = 0;
  assert(v->length() <= 8);
  char* rb = (char*) &result;
  const char* sb = (const char*) v->data();
  size_t offset = 8-v->length();
  memcpy(rb+offset, sb, v->length());
  return result;
}

uint64_t NumericTextUnitDescrBase::value(const TextUnit* u) const
{
  return value(u->firstTextUnitValue());
}

struct StringTextUnitDef {
  uint16_t key;
  const char* mnemonic;
  const char* label;
  bool singleton;
};

static StringTextUnitDef stringDefs[] = {
  {0x0001, "INMDDNAM", "DDNAME for the file", true},
  {0x0002, "INMDSNAM", "Name of the file", true},
  {0x0003, "INMMEMBR", "Member name list", true},
  {0x0022, "INMEXPDT", "Expiration date", true},
  {0x1001, "INMTNODE", "Target node name or node number", true},
  {0x1002, "INMTUID", "Target user ID", true},
  {0x1011, "INMFNODE", "Origin node name or node number", true},
  {0x1012, "INMFUID", "Origin user ID", true},
  {0x1020, "INMLREF", "Date last referenced", true},
  {0x1021, "INMLCHG", "Date last changed", true},
  {0x1022, "INMCREAT", "Creation date", true},
  {0x1024, "INMFTIME", "Origin timestamp", true},
  {0x1025, "INMTTIME", "Destination timestamp", true},
  {0x1026, "INMFACK", "Originator requested notification", true},
  {0x1027, "INMERRCD", "RECEIVE command error code", true},
  {0x1028, "INMUTILN", "Name of utility program", true},
  {0x1029, "INMUSERP", "User parameter string", true},
  {0, 0, 0, 0}
};


TextUnitDictionary::TextUnitDictionary() {
  StringTextUnitDef* sdef = stringDefs;
  while (sdef->key) {
    StringTextUnitDescrBase* desc =
      new StringTextUnitDescrBase(sdef->key, sdef->mnemonic, sdef->label, sdef->singleton);
    insert(std::make_pair(sdef->key, desc));
    ++sdef;
  }
}

TextUnitDictionary::~TextUnitDictionary() {
  for (auto it = begin(); it != end(); ++it) {
    delete it->second;
  }
  clear();
};

const TextUnitDescrBase* TextUnitDictionary::descriptorFor(uint16_t key) const {
  auto where = find(key);
  return (where == end() ? (const TextUnitDescrBase*) 0 : where->second);
}

