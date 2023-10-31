#include <cstring>
#include "composite_argument.hpp"
#include "lib/basic/basic_functions.hpp"

CompositeArgument::CompositeArgument() {
  short_key_ = nullptr;
  long_key_ = nullptr;
  name_ = nullptr;
  value_ = kError;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  type_ = ArgumentType::kCompositeArgument;
  is_required_ = false;
  Validate_ = nullptr;
  IsGood_ = nullptr;
}

CompositeArgument::CompositeArgument(ArgumentInformation info) {
  value_ = kError;
  short_key_ = info.short_key;
  long_key_ = info.long_key;
  name_ = info.name;
  value_status_ = ArgumentParsingStatus::kNoArgument;
  type_ = ArgumentType::kCompositeArgument;
  is_required_ = info.is_required;
  Validate_ = info.Validate;
  IsGood_ = info.IsGood;
}

CompositeArgument::CompositeArgument(const CompositeArgument& other) {
  short_key_ = other.short_key_;
  long_key_ = other.long_key_;
  name_ = other.name_;
  value_ = other.value_;
  value_status_ = other.value_status_;
  type_ = ArgumentType::kCompositeArgument;
  is_required_ = other.is_required_;
  Validate_ = other.Validate_;
  IsGood_ = other.IsGood_;
}

CompositeArgument& CompositeArgument::operator=(const CompositeArgument& other) {
  if (this == &other) {
    return *this;
  }

  short_key_ = other.short_key_;
  long_key_ = other.long_key_;
  name_ = other.name_;
  value_ = other.value_;
  value_status_ = other.value_status_;
  type_ = ArgumentType::kCompositeArgument;
  is_required_ = other.is_required_;
  Validate_ = other.Validate_;
  IsGood_ = other.IsGood_;

  return *this;
}

CompositeArgument::~CompositeArgument() {
  delete[] value_;
}

void CompositeArgument::ValidateArgument(char** argv,
                                         int32_t argc,
                                         char* candidate,
                                         char* value,
                                         int32_t position) {
  bool is_short = strcmp(candidate, short_key_) == 0 && position != argc - 1;
  bool is_long = strncmp(candidate, long_key_, strlen(long_key_)) == 0;

  if (!(is_short || is_long) ||
      value_status_ == ArgumentParsingStatus::kSuccess) {
    return;
  }

  char* pre_value = is_short ? value : candidate + strlen(long_key_);

  if (strlen(pre_value) > 1) {
    if (strncmp(pre_value, "./", 2) == 0 || strncmp(pre_value, "C:", 2) == 0 ||
        strncmp(pre_value, ".\\", 2) == 0 || (!IsWindows() && strncmp(pre_value, "../", 3) == 0)
        || pre_value[0] == '/') {
      value_ = new char[strlen(pre_value) + 1];
      strcpy(value_, pre_value);
    } else if (strncmp(pre_value, "file://", 7) == 0) {
      value_ = new char[strlen(pre_value + 7) + 1];
      strcpy(value_, pre_value + 7);
    }

    if (value_ != kError) {
      bool is_real_file = false;
      int32_t current = position;

      if (Validate_(value_)) {
        is_real_file = IsGood_(value_);
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

        if (Validate_(value_)) {
          is_real_file = IsGood_(value_);
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

    if (Validate_(simple_value)) {
      if (IsGood_(simple_value)) {
        value_ = simple_value;
        value_status_ = ArgumentParsingStatus::kSuccess;
      }
    }
  }

  if (value_status_ == ArgumentParsingStatus::kNoArgument) {
    value_status_ = ArgumentParsingStatus::kBrokenArgument;
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

const char* CompositeArgument::GetName() const {
  return name_;
}

bool CompositeArgument::GetIsRequired() const {
  return is_required_;
}
