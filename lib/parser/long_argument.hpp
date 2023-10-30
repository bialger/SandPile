#ifndef LONG_ARGUMENT_HPP_
#define LONG_ARGUMENT_HPP_

#include "argument.hpp"

#include <cstdint>

class LongArgument : Argument {
 public:
  LongArgument();
  LongArgument(const char* short_key, const char* long_key);
  void ValidateArgument(char* candidate, char* value, bool is_last);
  uint64_t GetValue() const;
  ArgumentParsingStatus GetValueStatus() const override;

 private:
  const char* short_key_;
  const char* long_key_;
  uint64_t value_;
  ArgumentParsingStatus value_status_;
};

#endif //LONG_ARGUMENT_HPP_
