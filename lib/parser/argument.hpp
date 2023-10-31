#ifndef ARGUMENT_HPP_
#define ARGUMENT_HPP_

enum class ArgumentParsingStatus {
  kNoArgument,
  kBrokenArgument,
  kSuccess
};

enum class ArgumentType {
  kBoolArgument,
  kCompositeArgument,
  kLongArgument,
  kStringArgument
};

struct ArgumentInformation {
  const char* short_key = nullptr;
  const char* long_key = nullptr;
  const char* name = nullptr;
  ArgumentType type = ArgumentType::kBoolArgument;
  bool is_required = false;
};

class Argument {
 public:
  virtual ~Argument() = default;
  virtual ArgumentParsingStatus GetValueStatus() const = 0;
  virtual ArgumentType GetType() const = 0;
  virtual const char* GetName() const = 0;
  virtual bool GetIsRequired() const = 0;
};

#endif //ARGUMENT_HPP_
