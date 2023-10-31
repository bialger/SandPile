#include "bool_argument.hpp"

#include <cstring>

BoolArgument::BoolArgument() {
  short_key_ = nullptr;
  long_key_ = nullptr;
  value_ = false;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  type_ = ArgumentType::kBoolArgument;
}

BoolArgument::BoolArgument(const char* short_key, const char* long_key) {
  short_key_ = short_key;
  long_key_ = long_key;
  value_ = false;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  type_ = ArgumentType::kBoolArgument;
}

void BoolArgument::ValidateArgument(char* candidate) {
  if (strcmp(candidate, short_key_) == 0 || strcmp(candidate, long_key_) == 0) {
    value_ = true;
    value_status_ = ArgumentParsingStatus::kSuccess;
  }
}

bool BoolArgument::GetValue() const {
  return value_;
}

ArgumentParsingStatus BoolArgument::GetValueStatus() const {
  return value_status_;
}


ArgumentType BoolArgument::GetType() const {
  return type_;
}
