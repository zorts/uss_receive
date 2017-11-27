// -*-C++-*-
#include "textunits.h"

#include <iostream>
#include <cassert>

StringTextUnitDescrBase::StringTextUnitDescrBase(uint16_t key,
                                                 const char* mnemonic,
                                                 const char* label,
                                                 bool singleton = true)
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
      s << value(*it);
    }
    s << ")"
      }
}

std::string StringTextUnitDescrBase::value(const TextUnitValue* v) const
{
  return std::string((const char*)v->firstTextUnitValue()->data(), v->firstTextUnitValue()->length());
}

std::string StringTextUnitDescrBase::value(const TextUnit* u) const
{
  return value(u->firstTextUnitValue());
}
