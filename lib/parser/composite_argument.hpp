#ifndef COMPOSITE_ARGUMENT_HPP_
#define COMPOSITE_ARGUMENT_HPP_

#include "argument.hpp"

#include <cstdint>

class CompositeArgument : Argument {
 public:
  CompositeArgument();
  CompositeArgument(const char* short_key, const char* long_key);
  CompositeArgument(const CompositeArgument& other);
  CompositeArgument& operator=(const CompositeArgument& other);
  ~CompositeArgument() override;
  void ValidateArgument(char** argv, int32_t argc, char* candidate, char* value, int32_t position,
                        bool (* Validate)(char*), bool (* IsGood)(char*));
  char* GetValue() const;
  ArgumentParsingStatus GetValueStatus() const override;

 private:
  constexpr static char* const kError = nullptr;
  const char* short_key_;
  const char* long_key_;
  char* value_;
  ArgumentParsingStatus value_status_;
};

#endif //COMPOSITE_ARGUMENT_HPP_
