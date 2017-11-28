// -*-C++-*-
#include "textunits.h"

#include <iostream>
#include <iomanip>
#include <cassert>
#include <string.h>

void TextUnitDescrBase::display(std::ostream& s) const {
  s << "key:" << std::hex << key() << std::dec
    << "; mnemonic: " << mnemonic()
    << "; label: " << label()
    << "; type: " << type()
    << (isSingleton() ? "; singleton; " : "; ");
  display_(s);
  s << std::endl;
}

void TextUnitDescrBase::display_(std::ostream&) const {
}

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

void NumericTextUnitDescrBase::display_(std::ostream& s) const {
  s << (hex_ ? "hex" : "");
}

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
  {0x0002, "INMDSNAM", "Name of the file", false},
  {0x0003, "INMMEMBR", "Member name list", false},
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

struct NumericTextUnitDef {
  uint16_t key;
  const char* mnemonic;
  const char* label;
  bool hex;
  bool singleton;
};

static NumericTextUnitDef numericDefs[] = {
  {0x000B, "INMSECND", "Secondary space quantity", false, true},
  {0x000C, "INMDIR", "Number of directory blocks", false, true},
  {0x0028, "INMTERM", "Data transmitted as a message", false, true},
  {0x0030, "INMBLKSZ", "Block size", false, true},
  {0x003C, "INMDSORG", "File organization", false, true},
  {0x0042, "INMLRECL", "Logical record length", false, true},
  {0x0049, "INMRECFM", "Record format", false, true},
  {0x1023, "INMFVERS", "Origin version number of the data format", false, true},
  {0x102A, "INMRECCT", "Transmitted record count", false, true},
  {0x102C, "INMSIZE", "File size in bytes", false, true},
  {0x102D, "INMFFM", "Filemode number", false, true},
  {0x102F, "INMNUMF", "Number of files transmitted", false, true},
  {0x8012, "INMTYPE", "Data set type", false, true},
  {0x8018, "INMLSIZE", "Data set size in megabytes.", false, true},
  {0x8028, "INMEATTR", "Extended attribute status", false, true},
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
  NumericTextUnitDef* ndef = numericDefs;
  while (ndef->key) {
    NumericTextUnitDescrBase* desc =
      new NumericTextUnitDescrBase(ndef->key, ndef->mnemonic, ndef->label, ndef->hex, ndef->singleton);
    insert(std::make_pair(ndef->key, desc));
    ++ndef;
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

