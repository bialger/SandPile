#ifndef BOOL_ARGUMENT_HPP_
#define BOOL_ARGUMENT_HPP_

#include "argument.hpp"

#include <cstdint>

class BoolArgument : Argument {
 public:
  BoolArgument();
  BoolArgument(const char* short_key, const char* long_key);
  void ValidateArgument(char* candidate);
  bool GetValue() const;
  ArgumentParsingStatus GetValueStatus() const override;

 private:
  const char* short_key_;
  const char* long_key_;
  bool value_;
  ArgumentParsingStatus value_status_;
};

#endif //BOOL_ARGUMENT_HPP_
