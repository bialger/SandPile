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

class Argument {
 public:
  virtual ~Argument() = default;
  virtual ArgumentParsingStatus GetValueStatus() const = 0;
  virtual ArgumentType GetType() const = 0;
};

#endif //ARGUMENT_HPP_
