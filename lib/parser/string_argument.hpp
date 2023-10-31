#ifndef STRING_ARGUMENT_HPP_
#define STRING_ARGUMENT_HPP_

#include "argument.hpp"

#include <cstdint>

class StringArgument : Argument {
 public:
  StringArgument();
  StringArgument(const char* short_key, const char* long_key);
  StringArgument(const StringArgument& other);
  StringArgument& operator=(const StringArgument& other);
  ~StringArgument() override;
  void ValidateArgument(char* candidate, char* value, bool is_last, bool (* IsGood)(char*));
  char* GetValue() const;
  ArgumentParsingStatus GetValueStatus() const override;
  ArgumentType GetType() const override;

 private:
  constexpr static char* const kError = nullptr;
  const char* short_key_;
  const char* long_key_;
  char* value_;
  ArgumentParsingStatus value_status_;
  ArgumentType type_;
};

#endif //STRING_ARGUMENT_HPP_
