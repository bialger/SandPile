#include <cstring>
#include "composite_argument.hpp"
#include "lib/basic/basic_functions.hpp"

CompositeArgument::CompositeArgument() {
  short_key_ = nullptr;
  long_key_ = nullptr;
  value_ = kError;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  type_ = ArgumentType::kCompositeArgument;
}

CompositeArgument::CompositeArgument(const char* short_key, const char* long_key) {
  value_ = kError;
  short_key_ = short_key;
  long_key_ = long_key;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  type_ = ArgumentType::kCompositeArgument;
}

CompositeArgument::CompositeArgument(const CompositeArgument& other) {
  short_key_ = other.short_key_;
  long_key_ = other.long_key_;
  value_ = other.value_;
  value_status_ = other.value_status_;
  type_ = ArgumentType::kCompositeArgument;
}

CompositeArgument& CompositeArgument::operator=(const CompositeArgument& other) {
  if (this == &other) {
    return *this;
  }

  short_key_ = other.short_key_;
  long_key_ = other.long_key_;
  value_ = other.value_;
  value_status_ = other.value_status_;
  type_ = ArgumentType::kCompositeArgument;

  return *this;
}

CompositeArgument::~CompositeArgument() {
  delete[] value_;
}

void CompositeArgument::ValidateArgument(char** argv,
                                         int32_t argc,
                                         char* candidate,
                                         char* value,
                                         int32_t position,
                                         bool (* Validate)(char*),
                                         bool (* IsGood)(char*)) {
  bool is_short = strcmp(candidate, short_key_) == 0 && position != argc - 1;
  bool is_long = strncmp(candidate, long_key_, strlen(long_key_)) == 0;

  if (!(is_short || is_long) ||
      value_status_ == ArgumentParsingStatus::kSuccess) {
    return;
  }

  char* pre_value = is_short ? value : candidate + strlen(long_key_);

  if (strlen(pre_value) > 1) {
    if (strncmp(pre_value, "./", 2) == 0 || strncmp(pre_value, "C:", 2) == 0 ||
        strncmp(pre_value, ".\\", 2) == 0 || pre_value[0] == '/') {
      value_ = new char[strlen(pre_value) + 1];
      strcpy(value_, pre_value);
    } else if (strncmp(pre_value, "file://", 7) == 0) {
      value_ = new char[strlen(pre_value + 7) + 1];
      strcpy(value_, pre_value + 7);
    }

    if (value_ != kError) {
      bool is_real_file = false;
      int32_t current = position;

      if (Validate(value_)) {
        is_real_file = IsGood(value_);
      } else {
        value_status_ = ArgumentParsingStatus::kBrokenArgument;
      }

      while (!is_real_file && current < argc - 1 &&
          value_status_ != ArgumentParsingStatus::kBrokenArgument &&
          strlen(value_) < 256) {
        size_t len = strlen(value_) + strlen(argv[current + 1]) + 2;
        char* new_candidate = new char[len];
        std::memset(new_candidate, 0, len);
        strcpy(new_candidate, value_);
        strcat(new_candidate, " ");
        strcat(new_candidate, argv[current + 1]);
        delete[] value_;
        value_ = new_candidate;

        if (Validate(value_)) {
          is_real_file = IsGood(value_);
        } else {
          value_status_ = ArgumentParsingStatus::kBrokenArgument;
        }

        ++current;
      }

      if (is_real_file) {
        value_status_ = ArgumentParsingStatus::kSuccess;
      } else {
        delete[] value_;
        value_ = kError;
        value_status_ = ArgumentParsingStatus::kBrokenArgument;
      }
    }
  }

  if (value_status_ != ArgumentParsingStatus::kSuccess) {
    char* simple_value = new char[strlen(pre_value) + (IsWindows() ? 1 : 3)];
    std::memset(simple_value, 0, strlen(pre_value) + (IsWindows() ? 1 : 3));
    strcat(simple_value, IsWindows() ? "" : "./");
    strcat(simple_value, pre_value);

    if (Validate(simple_value)) {
      if (IsGood(simple_value)) {
        value_ = simple_value;
        value_status_ = ArgumentParsingStatus::kSuccess;
      }
    }
  }
}

char* CompositeArgument::GetValue() const {
  return value_;
}

ArgumentParsingStatus CompositeArgument::GetValueStatus() const {
  return value_status_;
}

ArgumentType CompositeArgument::GetType() const {
  return type_;
}
