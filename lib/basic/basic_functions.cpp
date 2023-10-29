#include "basic_functions.hpp"

#include <climits>
#include <cstring>
#include <iostream>

int64_t IntFromString(char* int_literal, int64_t limit, int8_t base) {
  if (strncmp(int_literal, "0b", 2) == 0) {
    base = 2;
    int_literal += 2;
  } else if (strncmp(int_literal, "0o", 2) == 0) {
    base = 8;
    int_literal += 2;
  } else if (int_literal[0] == 'x') {
    base = 16;
    int_literal += 1;
  }

  /* Some more prefixes that are not supported by strtoll. */

  int32_t save_errno = errno;
  errno = 0;
  char* end;
  int64_t result = std::strtoll(int_literal, &end, base);

  if (end == int_literal || *end != '\0' ||
      ((result == LLONG_MIN || result == LLONG_MAX) && errno == ERANGE)) {
    result = -1;
    errno = save_errno;
  }

  /* C-style function error check: *end is a pointer to the beginning of the
   * part of the string that was not parsed, errno - C error handler, */

  if (result < 0 || (result > limit && limit != -1)) {
    result = -1;
  }

  return result;
}

char* i64toa(int64_t value, char* str, int32_t base) {
  uint64_t modulus_value;
  int32_t negative;
  char buffer[65];
  char* pos;
  int32_t digit;

  if (value < 0 && base == 10) {
    negative = 1;
    modulus_value = static_cast<uint64_t>(-value);
  } else {
    negative = 0;
    modulus_value = static_cast<uint64_t>(value);
  }

  pos = &buffer[64];
  *pos = '\0';

  do {
    digit = static_cast<int>(modulus_value % static_cast<uint64_t>(base));
    modulus_value = modulus_value / static_cast<uint64_t>(base);
    if (digit < 10) {
      *--pos = static_cast<char>('0' + digit);
    } else {
      *--pos = static_cast<char>('a' + digit - 10);
    }
  } while (modulus_value != 0L);

  if (negative) {
    *--pos = '-';
  }

  memcpy(str, pos, static_cast<size_t>(&buffer[64] - pos + 1));
  return str;
}

bool IsWindows() {
  return
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    true
#else
      false
#endif
      ;
}

bool IsValidFilename(char* pre_filename) {
  size_t filename_length = strlen(pre_filename);

  if (IsWindows() && filename_length > 2) {
    for (uint64_t position = 2; position < filename_length; ++position) {
      char current = pre_filename[position];
      if (!(std::isalnum(current) || current == '\\' || current == '.' ||
          current == '-' || current == ' ')) {
        return false;
      }
    }
  }

  /* This Windows-specific check is important because different code pages can
   * corrupt non-alphanumeric filenames, but UNIX-like systems (like MacOS or
   * Linux) handle unicode correctly. */

  for (uint64_t position = 0; position < filename_length - 1; ++position) {
    char current = pre_filename[position];
    char next = pre_filename[position + 1];

    if ((current == '\\' || current == '/') && next == current) {
      return false;
    }
  }

  return true;
}

void PrintHelp() { // TODO: write full help page
  std::cout << "SandPile model version 0.1 by bialger.\n";
  std::cout << "SandPile model help page.\n";
  std::cout << "Usage: SandPile [OPTIONS]...\n";
  std::cout << "SandPile saves the state of the model to a file.\n";
  std::cout << "SandPile does not support cyrillic filenames on Windows.\n";
  std::cout << "\nOPTIONS:\n";
  std::cout << "\t-h or --help\t\t\t\t\t";
  std::cout << "print this help page.\n";
  std::cout << std::endl;
}

/* The code provides dummy function definitions for Windows console-related
 * functions when the code is being compiled in a non-Windows environment.
 * This ensures that the code can compile and run without errors in such
 * environments. The dummy functions have minimal functionality and simply
 * return their input parameters. */

#if not (defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
int GetStdHandle(int a) { return a; }
int SetConsoleTextAttribute(int a, int b) { return a + b; }
#endif