#include "lib/basic/basic_functions.hpp"
#include "lib/parser/arguments_parser.hpp"
#include "lib/sand_pile/sand_pile.hpp"

int main(int32_t argc, char** argv) {
  ArgumentsParser arguments_parser = ArgumentsParser();
  int8_t exit_code = arguments_parser.ParseArguments(argv, argc);

  if (exit_code == 0 && !arguments_parser.GetProvideHelp()) {
    SandPile sand_pile = SandPile();
    sand_pile.BeginCollapsing(arguments_parser.GetInputFile(),
                              arguments_parser.GetOutputDirectory(),
                              arguments_parser.GetMaxIterations(),
                              arguments_parser.GetFrequency(),
                              arguments_parser.GetWriteTsv());
  } else {
    PrintHelp();
  }

  return exit_code;
}