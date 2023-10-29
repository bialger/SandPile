#include "arguments_parser.hpp"
#include "lib/basic/basic_functions.hpp"
#include <cstring>
#include <filesystem>
#include <iostream>

ArgumentsParser::ArgumentsParser() {
  input_file_ = kError;
  output_directory_ = kError;
  max_iterations_ = std::numeric_limits<uint64_t>::max();
  frequency_ = std::numeric_limits<uint64_t>::max();
  write_tsv_ = false;
  provide_help_ = false;
  input_file_status_ = ArgumentParsingStatus::kNoArgument;
  output_directory_status_ = ArgumentParsingStatus::kNoArgument;
  max_iterations_status_ = ArgumentParsingStatus::kNoArgument;
  frequency_status_ = ArgumentParsingStatus::kNoArgument;
  write_tsv_status_ = ArgumentParsingStatus::kNoArgument;
  provide_help_status_ = ArgumentParsingStatus::kNoArgument;
}

int8_t ArgumentsParser::ParseArguments(char** argv, int32_t argc) {
  for (int32_t i = 1; i < argc; ++i) {
    char* candidate = argv[i];
    char* value = kError;
    bool is_last = (i == argc - 1);

    if (!is_last) {
      value = argv[i + 1];
    }

    ValidateInputFile(argv, argc, candidate, value, i);
    ValidateOutputDirectory(argv, argc, candidate, value, i);
    ValidateMaxIterations(candidate, value, is_last);
    ValidateFrequency(candidate, value, is_last);
    ValidateWriteTsv(candidate);
    ValidateProvideHelp(candidate);
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

  if (input_file_status_ != ArgumentParsingStatus::kSuccess &&
      provide_help_status_ != ArgumentParsingStatus::kSuccess) {
    if (input_file_status_ == ArgumentParsingStatus::kNoArgument) {
      std::cout << "Error! Input file name not specified. \n\n";
    } else if (input_file_status_ == ArgumentParsingStatus::kBrokenArgument) {
      std::cout << "Error! Input file name is incorrect. "
                   "Make sure that it is not a directory. \n\n";
    }

    exitCode = 1;
  }

  if (output_directory_status_ != ArgumentParsingStatus::kSuccess &&
      provide_help_status_ != ArgumentParsingStatus::kSuccess) {
    if (output_directory_status_ == ArgumentParsingStatus::kNoArgument) {
      std::cout << "Error! Output directory not specified. \n\n";
    } else if (output_directory_status_ == ArgumentParsingStatus::kBrokenArgument) {
      std::cout << "Error! Output directory is incorrect. \n\n";
    }

    exitCode = 1;
  }

  if (max_iterations_status_ == ArgumentParsingStatus::kBrokenArgument) {
    std::cout << "Error! Maximum iterations argument is incorrect. \n\n";
    exitCode = 1;
  }

  if (frequency_status_ == ArgumentParsingStatus::kBrokenArgument) {
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

void ArgumentsParser::ValidateInputFile(char** argv, int32_t argc,
                                        char* candidate, char* value,
                                        int32_t position) {
  bool is_short = strcmp(candidate, "-i") == 0 && position != argc - 1;
  bool is_long = strncmp(candidate, "--input=", 8) == 0;

  if (!(is_short || is_long) ||
      input_file_status_ == ArgumentParsingStatus::kSuccess) {
    return;
  }

  char* pre_filename = is_short ? value : candidate + 8;

  if (strlen(pre_filename) > 1) {
    if (strncmp(pre_filename, "./", 2) == 0 || strncmp(pre_filename, "C:", 2) == 0 ||
        strncmp(pre_filename, ".\\", 2) == 0 || pre_filename[0] == '/') {
      input_file_ = new char[strlen(pre_filename) + 1];
      strcpy(input_file_, pre_filename);
    } else if (strncmp(pre_filename, "file://", 7) == 0) {
      input_file_ = new char[strlen(pre_filename + 7) + 1];
      strcpy(input_file_, pre_filename + 7);
    }

    if (input_file_ != kError) {
      bool is_real_file = false;
      int32_t current = position;

      if (IsValidFilename(input_file_)) {
        std::filesystem::path path(input_file_);
        is_real_file = std::filesystem::is_regular_file(path);
      } else {
        input_file_status_ = ArgumentParsingStatus::kBrokenArgument;
      }

      while (!is_real_file && current < argc - 1 &&
          input_file_status_ != ArgumentParsingStatus::kBrokenArgument &&
          strlen(input_file_) < 256) {
        size_t len = strlen(input_file_) + strlen(argv[current + 1]) + 2;
        char* new_candidate = new char[len];
        std::memset(new_candidate, 0, len);
        strcpy(new_candidate, input_file_);
        strcat(new_candidate, " ");
        strcat(new_candidate, argv[current + 1]);
        delete[] input_file_;
        input_file_ = new_candidate;

        if (IsValidFilename(input_file_)) {
          std::filesystem::path testPath(input_file_);
          is_real_file = std::filesystem::is_regular_file(testPath);
        } else {
          input_file_status_ = ArgumentParsingStatus::kBrokenArgument;
        }

        ++current;
      }

      if (is_real_file) {
        input_file_status_ = ArgumentParsingStatus::kSuccess;
      } else {
        delete[] input_file_;
        input_file_ = kError;
        input_file_status_ = ArgumentParsingStatus::kBrokenArgument;
      }
    }
  }

  if (input_file_status_ != ArgumentParsingStatus::kSuccess) {
    char* simple_value = new char[strlen(pre_filename) + (IsWindows() ? 1 : 3)];
    std::memset(simple_value, 0, strlen(pre_filename) + (IsWindows() ? 1 : 3));
    strcat(simple_value, IsWindows() ? "" : "./");
    strcat(simple_value, pre_filename);

    if (IsValidFilename(simple_value)) {
      std::filesystem::path path(simple_value);

      if (std::filesystem::is_regular_file(path)) {
        input_file_ = simple_value;
        input_file_status_ = ArgumentParsingStatus::kSuccess;
      }
    }
  }

  if (strcmp(strrchr(input_file_, '.'), ".tsv") != 0) {
    input_file_ = kError;
    input_file_status_ = ArgumentParsingStatus::kBrokenArgument;
  }
}

void ArgumentsParser::ValidateOutputDirectory(char** argv, int32_t argc,
                                              char* candidate, char* value,
                                              int32_t position) {
  bool is_short = strcmp(candidate, "-o") == 0 && position != argc - 1;
  bool is_long = strncmp(candidate, "--output=", 9) == 0;

  if (!(is_short || is_long) ||
      output_directory_status_ == ArgumentParsingStatus::kSuccess) {
    return;
  }

  char* pre_dirname = is_short ? value : candidate + 9;

  if (strlen(pre_dirname) > 1) {
    if (strncmp(pre_dirname, "./", 2) == 0 || strncmp(pre_dirname, "C:", 2) == 0 ||
        strncmp(pre_dirname, ".\\", 2) == 0 || pre_dirname[0] == '/') {
      output_directory_ = new char[strlen(pre_dirname) + 1];
      strcpy(output_directory_, pre_dirname);
    } else if (strncmp(pre_dirname, "file://", 7) == 0) {
      output_directory_ = new char[strlen(pre_dirname + 7) + 1];
      strcpy(output_directory_, pre_dirname + 7);
    }

    if (output_directory_ != kError) {
      bool is_directory = false;
      int32_t current = position;

      if (IsValidFilename(output_directory_)) {
        std::filesystem::path path(output_directory_);
        is_directory = std::filesystem::is_directory(path);
      } else {
        output_directory_status_ = ArgumentParsingStatus::kBrokenArgument;
      }

      while (!is_directory && current < argc - 1 &&
          output_directory_status_ != ArgumentParsingStatus::kBrokenArgument &&
          strlen(output_directory_) < 256) {
        size_t len = strlen(output_directory_) + strlen(argv[current + 1]) + 2;
        char* new_candidate = new char[len];
        std::memset(new_candidate, 0, len);
        strcpy(new_candidate, output_directory_);
        strcat(new_candidate, " ");
        strcat(new_candidate, argv[current + 1]);
        delete[] output_directory_;
        output_directory_ = new_candidate;

        if (IsValidFilename(output_directory_)) {
          std::filesystem::path testPath(output_directory_);
          is_directory = std::filesystem::is_directory(testPath);
        } else {
          output_directory_status_ = ArgumentParsingStatus::kBrokenArgument;
        }

        ++current;
      }

      if (is_directory) {
        output_directory_status_ = ArgumentParsingStatus::kSuccess;
      } else {
        delete[] output_directory_;
        output_directory_ = kError;
        output_directory_status_ = ArgumentParsingStatus::kBrokenArgument;
      }
    }
  }

  if (output_directory_status_ != ArgumentParsingStatus::kSuccess) {
    char* simple_value = new char[strlen(pre_dirname) + (IsWindows() ? 1 : 3)];
    std::memset(simple_value, 0, strlen(pre_dirname) + (IsWindows() ? 1 : 3));
    strcat(simple_value, IsWindows() ? "" : "./");
    strcat(simple_value, pre_dirname);

    if (IsValidFilename(simple_value)) {
      std::filesystem::path path(simple_value);

      if (std::filesystem::is_directory(path)) {
        output_directory_ = simple_value;
        output_directory_status_ = ArgumentParsingStatus::kSuccess;
      }
    }
  }
}

void ArgumentsParser::ValidateMaxIterations(char* candidate, char* value,
                                            bool is_last) {
  bool is_short = strcmp(candidate, "-m") == 0 && !is_last;
  bool is_long = strncmp(candidate, "--max-iter=", 11) == 0;

  if (is_short || is_long) {
    char* pre_max_iterations = is_short ? value : candidate + 11;

    if (pre_max_iterations[0] == '\'' || pre_max_iterations[0] == '"') {
      ++pre_max_iterations;
      *(strrchr(pre_max_iterations, '\0') - 1) = '\0';
    }

    int64_t max_iterations = IntFromString(pre_max_iterations, -1, 0);

    if (max_iterations != -1l) {
      max_iterations_ = static_cast<uint64_t>(max_iterations);
      max_iterations_status_ = ArgumentParsingStatus::kSuccess;
    } else {
      max_iterations_status_ = ArgumentParsingStatus::kBrokenArgument;
    }
  }
}

void ArgumentsParser::ValidateFrequency(char* candidate, char* value,
                                        bool is_last) {
  bool is_short = strcmp(candidate, "-f") == 0 && !is_last;
  bool is_long = strncmp(candidate, "--freq=", 7) == 0;

  if (is_short || is_long) {
    char* pre_frequency = is_short ? value : candidate + 7;

    if (pre_frequency[0] == '\'' || pre_frequency[0] == '"') {
      ++pre_frequency;
      *(strrchr(pre_frequency, '\0') - 1) = '\0';
    }

    int64_t frequency = IntFromString(pre_frequency, -1, 0);

    if (frequency != -1l) {
      frequency_ = static_cast<uint64_t>(frequency);
      frequency_status_ = ArgumentParsingStatus::kSuccess;
    } else {
      frequency_status_ = ArgumentParsingStatus::kBrokenArgument;
    }
  }
}

void ArgumentsParser::ValidateWriteTsv(char* candidate) {
  if (strcmp(candidate, "-w") == 0 || strcmp(candidate, "--write-tsv") == 0) {
    write_tsv_ = true;
    write_tsv_status_ = ArgumentParsingStatus::kSuccess;
  }
}

void ArgumentsParser::ValidateProvideHelp(char* candidate) {
  if (strcmp(candidate, "-h") == 0 || strcmp(candidate, "--help") == 0) {
    provide_help_ = true;
    provide_help_status_ = ArgumentParsingStatus::kSuccess;
  }
}

char* ArgumentsParser::GetInputFile() {
  return input_file_;
}

char* ArgumentsParser::GetOutputDirectory() {
  return output_directory_;
}

uint64_t ArgumentsParser::GetMaxIterations() const {
  return max_iterations_;
}

uint64_t ArgumentsParser::GetFrequency() const {
  return frequency_;
}

bool ArgumentsParser::GetWriteTsv() const {
  return write_tsv_;
}

bool ArgumentsParser::GetProvideHelp() const {
  return provide_help_;
}

ArgumentsParser::~ArgumentsParser() {
  delete[] input_file_;
  delete[] output_directory_;
}
