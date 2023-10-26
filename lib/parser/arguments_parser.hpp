#ifndef ARGUMENTS_PARSER_HPP
#define ARGUMENTS_PARSER_HPP

#include <cstdint>

enum class ArgumentParsingStatus {
  kNoArgument,
  kBrokenArgument,
  kSuccess
};

class ArgumentsParser {
private:
  constexpr static char* const kError = nullptr;
  char* input_file_;
  char* output_directory_;
  uint64_t max_iterations_;
  uint64_t frequency_;
  bool write_tsv_;
  bool provide_help_;
  ArgumentParsingStatus input_file_status_;
  ArgumentParsingStatus output_directory_status_;
  ArgumentParsingStatus max_iterations_status_;
  ArgumentParsingStatus frequency_status_;
  ArgumentParsingStatus write_tsv_status_;
  ArgumentParsingStatus provide_help_status_;

  void ValidateInputFile(char** argv, int32_t argc, char*value,
                         char*pre_filename, int32_t position);
  void ValidateOutputDirectory(char** argv, int32_t argc, char*value,
                               char*pre_dirname, int32_t position);
  void ValidateMaxIterations(char* candidate, char* value, bool isLast);
  void ValidateFrequency(char* candidate, char* value, bool isLast);
  void ValidateWriteTsv(char* candidate);
  void ValidateProvideHelp(char* candidate);

  /** \n The HandleErrors function identifies and handles errors in the
   * ParsingResult by outputting error messages and setting an exit code. It
   * ensures that any errors or inconsistencies in the command-line arguments
   * are appropriately addressed and reported to the user. */

  int8_t HandleErrors();

public:
  ArgumentsParser();
  int8_t ParseArguments(char** argv, int32_t argc);
  char* GetInputFile();
  char* GetOutputDirectory();
  uint64_t GetMaxIterations() const;
  uint64_t GetFrequency() const;
  bool GetWriteTsv() const;
  bool GetProvideHelp() const;
  ~ArgumentsParser();
};

#endif // ARGUMENTS_PARSER_HPP
