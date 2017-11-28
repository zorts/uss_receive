// -*-C++-*-
#ifndef _XMIT_TEXTUNITS_H_
#define _XMIT_TEXTUNITS_H_

#include "textunit.h"
#include <iosfwd>
#include <string>
#include <unordered_map>

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

  void display(std::ostream& s) const;
  virtual void display(std::ostream& s, const TextUnit* unit) const = 0;

protected:

  virtual void display_(std::ostream& s) const;

private:
  uint16_t key_;
  std::string mnemonic_;
  std::string type_;
  std::string label_;
  bool singleton_;
};

class TextUnitDictionary : public std::unordered_map<uint16_t, TextUnitDescrBase*> {
public:
  TextUnitDictionary();
  ~TextUnitDictionary();

  const TextUnitDescrBase* descriptorFor(uint16_t key) const;

private:
  // Prohibit copy/assign; do not implement
  TextUnitDictionary(const TextUnitDictionary&);
  TextUnitDictionary& operator=(const TextUnitDictionary&);
};

class StringTextUnitDescrBase : public TextUnitDescrBase {
public:
  StringTextUnitDescrBase(uint16_t key,
                          const char* mnemonic,
                          const char* label,
                          bool singleton);

  virtual void display(std::ostream& s, const TextUnit* unit) const;
  virtual std::string value(const TextUnit* u) const;
  virtual std::string value(const TextUnitValue* v) const;
};

class NumericTextUnitDescrBase : public TextUnitDescrBase {
public:
  NumericTextUnitDescrBase(uint16_t key,
                           const char* mnemonic,
                           const char* label,
                           bool hex = false,
                           bool singleton = true);

  virtual void display(std::ostream& s, const TextUnit* unit) const;
  virtual uint64_t value(const TextUnit* u) const;
  virtual uint64_t value(const TextUnitValue* v) const;

protected:

  virtual void display_(std::ostream& s) const;

private:
  bool hex_;
};

#endif // _XMIT_TEXTUNITS_H_
