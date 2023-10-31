#ifndef BOOL_ARGUMENT_HPP_
#define BOOL_ARGUMENT_HPP_

#include "argument.hpp"

#include <cstdint>

class BoolArgument : Argument {
 public:
  BoolArgument();
  BoolArgument(const char* short_key, const char* long_key, const char* name);
  void ValidateArgument(char* candidate);
  bool GetValue() const;
  ArgumentParsingStatus GetValueStatus() const override;
  ArgumentType GetType() const override;
  const char* GetName() const override;

 private:
  const char* short_key_;
  const char* long_key_;
  const char* name_;
  bool value_;
  ArgumentParsingStatus value_status_;
  ArgumentType type_;
};

#endif //BOOL_ARGUMENT_HPP_
