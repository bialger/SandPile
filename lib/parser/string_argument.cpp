#include "string_argument.hpp"

#include <cstring>

StringArgument::StringArgument() {
  short_key_ = nullptr;
  long_key_ = nullptr;
  name_ = nullptr;
  value_ = kError;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  type_ = ArgumentType::kStringArgument;
}

StringArgument::StringArgument(const char* short_key, const char* long_key, const char* name) {
  value_ = kError;
  short_key_ = short_key;
  long_key_ = long_key;
  name_ = name;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  type_ = ArgumentType::kStringArgument;
}

StringArgument::StringArgument(const StringArgument& other) {
  short_key_ = other.short_key_;
  long_key_ = other.long_key_;
  name_ = other.name_;
  value_ = other.value_;
  value_status_ = other.value_status_;
  type_ = ArgumentType::kStringArgument;
}

StringArgument& StringArgument::operator=(const StringArgument& other) {
  if (this == &other) {
    return *this;
  }

  short_key_ = other.short_key_;
  long_key_ = other.long_key_;
  name_ = other.name_;
  value_ = other.value_;
  value_status_ = other.value_status_;
  type_ = ArgumentType::kStringArgument;

  return *this;
}

StringArgument::~StringArgument() {
  delete[] value_;
}

void StringArgument::ValidateArgument (char* candidate, char* value, bool is_last, bool (* IsGood)(char*)) {
  bool is_short = strcmp(candidate, short_key_) == 0 && !is_last;
  bool is_long = strncmp(candidate, long_key_, strlen(long_key_)) == 0;

  if (!(is_short || is_long) ||
      value_status_ == ArgumentParsingStatus::kSuccess) {
    return;
  }

  char* pre_value = is_short ? value : candidate + strlen(long_key_);

  value_ = new char[strlen(pre_value) + 1];
  strcpy(value_, pre_value);
  value_status_ = ArgumentParsingStatus::kSuccess;

  if (!IsGood(value)) {
    value_ = kError;
    value_status_ = ArgumentParsingStatus::kBrokenArgument;
  }
}

char* StringArgument::GetValue() const {
  return value_;
}

ArgumentParsingStatus StringArgument::GetValueStatus() const {
  return value_status_;
}

ArgumentType StringArgument::GetType() const {
  return type_;
}

const char* StringArgument::GetName() const {
  return name_;
}
