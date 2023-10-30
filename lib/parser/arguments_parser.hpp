#ifndef ARGUMENTS_PARSER_HPP
#define ARGUMENTS_PARSER_HPP

#include <cstdint>

#include "argument.hpp"
#include "composite_argument.hpp"
#include "long_argument.hpp"
#include "bool_argument.hpp"

class ArgumentsParser {
 public:
  ArgumentsParser();
  int8_t ParseArguments(char** argv, int32_t argc);
  char* GetInputFile();
  char* GetOutputDirectory();
  uint64_t GetMaxIterations() const;
  uint64_t GetFrequency() const;
  bool GetWriteTsv() const;
  bool GetProvideHelp() const;

 private:
  CompositeArgument input_file_;
  CompositeArgument output_directory_;
  LongArgument max_iterations_;
  LongArgument frequency_;
  BoolArgument write_tsv_;
  BoolArgument provide_help_;

  /** \n The HandleErrors function identifies and handles errors in the
   * ParsingResult by outputting error messages and setting an exit code. It
   * ensures that any errors or inconsistencies in the command-line arguments
   * are appropriately addressed and reported to the user. */

  int8_t HandleErrors();
};

#endif // ARGUMENTS_PARSER_HPP
