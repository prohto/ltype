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
  operator const char*() const;
  LTVar& operator[](const size_t idx);
  const LTVar& operator[](const size_t idx) const;
  LTVar& operator[](const std::string& tag);
  const LTVar& operator[](const std::string& tag) const;

  LTVar& set(const char path[]);
  template <typename T>
  LTVar& set(const char path[], T value) {
    LTVar& rtn = set(path);
    rtn = value;
    return rtn;
  }
  LTVar get(const char path[]);
  template <typename T>
  T get(const char path[]) {
    static LTVar rtn(LTVar::kVoid);
    return rtn;
  }
  template <typename T>
  T get(const char path[], T default_value) {
    LTVar value = get(path);
    if (value.get_type() == Type::kVoid) return default_value;
    return static_cast<T>(value);
  }
  LTVarIterator begin() const;
  LTVarIterator end() const;
  LTVarIterator find(const std::string& tag) const;

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
