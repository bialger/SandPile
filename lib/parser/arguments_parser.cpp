#include "arguments_parser.hpp"
#include "lib/basic/basic_functions.hpp"

#include <iostream>

ArgumentsParser::ArgumentsParser() {
  input_file_ = CompositeArgument({"-i", "--input=", "Path to input file",
                                   ArgumentType::kCompositeArgument, true});
  output_directory_ = CompositeArgument({"-o", "--output=", "Path to output directory",
                                         ArgumentType::kCompositeArgument, true});
  max_iterations_ = LongArgument({"-m", "--max-iter=", "Maximum number of iterations",
                                  ArgumentType::kLongArgument, false});
  frequency_ = LongArgument({"-f", "--freq=", "Frequency of output",
                             ArgumentType::kLongArgument, false});
  write_tsv_ = BoolArgument({"-w", "--write-tsv", "Write output in TSV format",
                             ArgumentType::kBoolArgument, false});
  provide_help_ = BoolArgument({"-h", "--help", "Provide help",
                                ArgumentType::kBoolArgument, false});
}

int8_t ArgumentsParser::ParseArguments(char** argv, int32_t argc) {
  bool (* is_valid_filename_pointer)(char*) = &IsValidFilename;
  bool (* is_regular_file_pointer)(char*) = &IsRegularFile;
  bool (* is_directory_pointer)(char*) = &IsDirectory;

  for (int32_t i = 1; i < argc; ++i) {
    char* candidate = argv[i];
    char* value = nullptr;
    bool is_last = (i == argc - 1);

    if (!is_last) {
      value = argv[i + 1];
    }

    input_file_.ValidateArgument(argv, argc, candidate, value, i,
                                 is_valid_filename_pointer, is_regular_file_pointer);
    output_directory_.ValidateArgument(argv, argc, candidate, value, i,
                                       is_valid_filename_pointer, is_directory_pointer);
    max_iterations_.ValidateArgument(candidate, value, is_last);
    frequency_.ValidateArgument(candidate, value, is_last);
    write_tsv_.ValidateArgument(candidate);
    provide_help_.ValidateArgument(candidate);
  }

  return HandleErrors();
}

int8_t ArgumentsParser::HandleErrors() {
  int8_t exitCode = 0;
  std::cout << (IsWindows() ? "" : "\x1B[31m");

  /* Changes the color of the console output to red if not running
   * on Windows. */

  if (IsWindows()) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 12);
  }

  /* Conditional block to set the console text color to red if the program is
   * running on Windows, using the Windows API functions. */

  if (input_file_.GetValueStatus() != ArgumentParsingStatus::kSuccess &&
      provide_help_.GetValueStatus() != ArgumentParsingStatus::kSuccess) {
    if (input_file_.GetValueStatus() == ArgumentParsingStatus::kNoArgument) {
      std::cout << "Error! Input file name not specified. \n\n";
    } else if (input_file_.GetValueStatus() == ArgumentParsingStatus::kBrokenArgument) {
      std::cout << "Error! Input file name is incorrect. "
                   "Make sure that it is not a directory. \n\n";
    }

    exitCode = 1;
  }

  if (output_directory_.GetValueStatus() != ArgumentParsingStatus::kSuccess &&
      provide_help_.GetValueStatus() != ArgumentParsingStatus::kSuccess) {
    if (output_directory_.GetValueStatus() == ArgumentParsingStatus::kNoArgument) {
      std::cout << "Error! Output directory not specified. \n\n";
    } else if (output_directory_.GetValueStatus() == ArgumentParsingStatus::kBrokenArgument) {
      std::cout << "Error! Output directory is incorrect. \n\n";
    }

    exitCode = 1;
  }

  if (max_iterations_.GetValueStatus() == ArgumentParsingStatus::kBrokenArgument) {
    std::cout << "Error! Maximum iterations argument is incorrect. \n\n";
    exitCode = 1;
  }

  if (frequency_.GetValueStatus() == ArgumentParsingStatus::kBrokenArgument) {
    std::cout << "Error! Frequency argument is incorrect. \n\n";
    exitCode = 1;
  }

  if (IsWindows()) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 15);
  }

  std::cout << (IsWindows() ? "" : "\x1B[0m");
  return exitCode;
}

char* ArgumentsParser::GetInputFile() {
  return input_file_.GetValue();
}

char* ArgumentsParser::GetOutputDirectory() {
  return output_directory_.GetValue();
}

uint64_t ArgumentsParser::GetMaxIterations() const {
  return max_iterations_.GetValue();
}

uint64_t ArgumentsParser::GetFrequency() const {
  return frequency_.GetValue();
}

bool ArgumentsParser::GetWriteTsv() const {
  return write_tsv_.GetValue();
}

bool ArgumentsParser::GetProvideHelp() const {
  return provide_help_.GetValue();
}
