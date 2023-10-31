#include "bool_argument.hpp"

#include <cstring>

BoolArgument::BoolArgument() {
  short_key_ = nullptr;
  long_key_ = nullptr;
  name_ = nullptr;
  value_ = false;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  type_ = ArgumentType::kBoolArgument;
  is_required_ = false;
}

BoolArgument::BoolArgument(ArgumentInformation info) {
  short_key_ = info.short_key;
  long_key_ = info.long_key;
  name_ = info.name;
  value_ = false;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  type_ = ArgumentType::kBoolArgument;
  is_required_ = info.is_required;
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

const char* BoolArgument::GetName() const {
  return name_;
}

bool BoolArgument::GetIsRequired() const {
  return is_required_;
}
