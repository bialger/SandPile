#ifndef BOOL_ARGUMENT_HPP_
#define BOOL_ARGUMENT_HPP_

#include "argument.hpp"

#include <cstdint>

class BoolArgument : Argument {
 public:
  BoolArgument();
  BoolArgument(ArgumentInformation info);
  void ValidateArgument(char* candidate);
  bool GetValue() const;
  ArgumentParsingStatus GetValueStatus() const override;
  ArgumentType GetType() const override;
  const char* GetName() const override;
  bool GetIsRequired() const override;

 private:
  const char* short_key_;
  const char* long_key_;
  const char* name_;
  bool value_;
  ArgumentParsingStatus value_status_;
  ArgumentType type_;
  bool is_required_;
};

#endif //BOOL_ARGUMENT_HPP_
