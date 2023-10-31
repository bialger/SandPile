#include "long_argument.hpp"
#include "lib/basic/basic_functions.hpp"

#include <limits>
#include <cstring>

LongArgument::LongArgument() {
  short_key_ = nullptr;
  long_key_ = nullptr;
  name_ = nullptr;
  value_ = 0;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  type_ = ArgumentType::kLongArgument;
  is_required_ = false;
}

LongArgument::LongArgument(ArgumentInformation info) {
  short_key_ = info.short_key;
  long_key_ = info.long_key;
  name_ = info.name;
  value_ = std::numeric_limits<uint64_t>::max();
  value_status_ = ArgumentParsingStatus::kNoArgument;
  type_ = ArgumentType::kLongArgument;
  is_required_ = info.is_required;
}

void LongArgument::ValidateArgument(char* candidate, char* value, bool is_last) {
  bool is_short = strcmp(candidate, short_key_) == 0 && !is_last;
  bool is_long = strncmp(candidate, long_key_, strlen(long_key_)) == 0;

  if (is_short || is_long) {
    char* string_value = is_short ? value : candidate + strlen(long_key_);

    if (string_value[0] == '\'' || string_value[0] == '"') {
      ++string_value;
      *(strrchr(string_value, '\0') - 1) = '\0';
    }

    int64_t pre_value = IntFromString(string_value, -1, 0);

    if (pre_value != -1l) {
      value_ = static_cast<uint64_t>(pre_value);
      value_status_ = ArgumentParsingStatus::kSuccess;
    } else {
      value_status_ = ArgumentParsingStatus::kBrokenArgument;
    }
  }
}

uint64_t LongArgument::GetValue() const {
  return value_;
}

ArgumentParsingStatus LongArgument::GetValueStatus() const {
  return value_status_;
}

ArgumentType LongArgument::GetType() const {
  return type_;
}

const char* LongArgument::GetName() const {
  return name_;
}

bool LongArgument::GetIsRequired() const {
  return is_required_;
}
