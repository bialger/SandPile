#include "lib/parser/arguments_parser.hpp"

#include <iostream>

int main(int32_t argc, char** argv) {
  ArgumentsParser arguments_parser = ArgumentsParser();
  int8_t exit_code = arguments_parser.ParseArguments(argv, argc);
  return exit_code;
}