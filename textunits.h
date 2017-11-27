// -*-C++-*-
#ifndef _XMIT_TEXTUNITS_H_
#define _XMIT_TEXTUNITS_H_

#include "textunit.h"
#include <isofwd>
#include <string>

class TextUnitDescrBase {
public:
  TextUnitDescrBase(uint16_t key,
                    const char* mnemonic,
                    const char* type,
                    const char* label,
                    bool singleton)
    : key_(key)
    , mnemonic_(mnemonic)
    , type_(type)
    , label_(label)
    , singleton_(singleton)
  {}

  virtual ~TextUnitDescrBase() {}

  uint16_t key() const {
    return key_;
  }

  const std::string& mnemonic() const {
    return mnemonic_;
  }

  const std::string& type() const {
    return type_;
  }

  const std::string& label() const {
    return label_;
  }

  bool isSingleton() const {
    return singleton_;
  }

  virtual void display(std::ostream& s, const TextUnit* unit) const = 0;


private:
  uint16_t key_;
  std::string mnemonic_;
  std::string type_;
  std::string label_;
  bool singleton_;
};

class StringTextUnitDescrBase : public TextUnitDescr {
 StringTextUnitDescrBase(uint16_t key,
                         const char* mnemonic,
                         const char* label,
                         bool singleton = true);

  virtual void display(std::ostream& s, const TextUnit* unit) const;
  virtual std::string value(const TextUnit* u) const;
  virtual std::string value(const TextUnitValue* v) const;
};
