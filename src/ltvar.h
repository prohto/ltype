#ifndef LTVAR_H
#define LTVAR_H

#include <memory>
#include <string>

#include "states/void.h"

class Void;
class Decoder;
class Encoder;

class LTVar {
 public:
  enum Type { kVoid, kBool, kDouble, kHash, kInteger, kArray, kText };
  LTVar();
  LTVar(const Type type);
  LTVar(const bool value);
  LTVar(const double value);
  LTVar(const int value);
  LTVar(const char* value);
  LTVar(const std::string& value);
  LTVar(const LTVarHash& value);
  LTVar(const LTVarArray& value);
  LTVar(const LTVar& src);
  LTVar(LTVar&& src);
  virtual ~LTVar();
  LTVar& operator=(const Type type);
  LTVar& operator=(const bool value);
  LTVar& operator=(const double value);
  LTVar& operator=(const int value);
  LTVar& operator=(const char*);
  LTVar& operator=(const std::string& value);
  LTVar& operator=(const LTVarHash& value);
  LTVar& operator=(const LTVarArray& value);
  LTVar& operator=(const LTVar& src);
  size_t size() const;
  bool operator==(const Type rhs) const;
  bool operator==(const LTVar& rhs) const;
  template <typename T>
  bool operator==(const T& rhs) const {
    return state_.get()->equal(rhs);
  }
  bool operator!=(const Type rhs) const;
  bool operator!=(const LTVar& rhs) const;
  template <typename T>
  bool operator!=(const T& rhs) const {
    return !state_.get()->equal(rhs);
  }
  Type get_type() const { return type_; }
  explicit operator bool() const;
  explicit operator double() const;
  explicit operator int() const;
  operator std::string() const;
  explicit operator const char*() const;
  LTVar& operator[](size_t idx);
  LTVar& operator[](int idx);
  LTVar& operator[](const char tag[]);
  LTVar& operator[](const LTVar& value);
  const LTVar& operator[](size_t idx) const;
  const LTVar& operator[](int idx) const;
  const LTVar& operator[](const char tag[]) const;
  const LTVar& operator[](const LTVar& value) const;

  LTVar& set(const char path[]);
  LTVar& set(const char path[], LTVar::Type type) {
    LTVar& rtn = set(path);
    if (rtn.get_type() != type) rtn = type;
    return rtn;
  }
  template <typename T>
  LTVar& set(const char path[], T value) {
    LTVar& rtn = set(path);
    rtn = value;
    return rtn;
  }
  LTVar get(const std::string& path) const { return get(path.c_str()); }
  LTVar get(const char* path) const;
  std::string get(const char path[], const char* default_value) const {
    LTVar value = get(path);
    if (value.get_type() == Type::kVoid) return std::string(default_value);
    return value;
  }
  template <typename T>
  T get(const char path[], T default_value) const {
    LTVar value = get(path);
    if (value.get_type() == Type::kVoid) return default_value;
    return static_cast<T>(value);
  }
  LTVarIterator begin() const;
  LTVarIterator end() const;
  LTVarIterator find(const char tag[]) const;

  friend class Decoder;
  friend class Encoder;
  template <typename T>
  friend bool operator==(T lhs, const LTVar& rhs) {
    return rhs == lhs;
  }

  template <typename T>
  friend bool operator!=(T lhs, const LTVar& rhs) {
    return rhs != lhs;
  }

 protected:
  std::unique_ptr<Void> state_;
  Type type_;
  void reset(Type type);
  virtual void encode(Encoder& ostream) const;
  virtual void decode(Decoder& istream);
};

#include "states/array.h"
#include "states/bool.h"
#include "states/double.h"
#include "states/hash.h"
#include "states/integer.h"
#include "states/text.h"

#endif  // LTVAR_H
