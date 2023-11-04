#include "lib/basic/basic_functions.hpp"
#include "lib/parser/arguments_parser.hpp"
#include "lib/sand_pile/sand_pile.hpp"

int main(int32_t argc, char** argv) {
  constexpr size_t kArgumentCount = 6;
  ArgumentInformation* arguments = new ArgumentInformation[kArgumentCount];
  arguments[0] = {"-i", "--input=", "Path to input file",
                  ArgumentType::kCompositeArgument, true,
                  &IsValidFilename, &IsRegularFile};
  arguments[1] = {"-o", "--output=", "Path to output directory",
                  ArgumentType::kCompositeArgument, true,
                  &IsValidFilename, &IsDirectory};
  arguments[2] = {"-m", "--max-iter=", "Maximum number of iterations",
                  ArgumentType::kLongArgument, false,
                  &IsValidFilename, &IsRegularFile};
  arguments[3] = {"-f", "--freq=", "Frequency of output",
                  ArgumentType::kLongArgument, false,
                  &IsValidFilename, &IsRegularFile};
  arguments[4] = {"-w", "--write-tsv", "Write output in TSV format",
                  ArgumentType::kBoolArgument, false,
                  &IsValidFilename, &IsRegularFile};
  arguments[5] = {"-h", "--help", "Provide help",
                  ArgumentType::kBoolArgument, false,
                  &IsValidFilename, &IsRegularFile};

  ArgumentsParser arguments_parser(arguments, kArgumentCount);
  int8_t exit_code = arguments_parser.ParseArguments(argv, argc);

  if (exit_code == 0 && !arguments_parser.GetBoolValue(5)) {
    SandPile sand_pile = SandPile();
    sand_pile.BeginCollapsing(arguments_parser.GetCompositeValue(0),
                              arguments_parser.GetCompositeValue(1),
                              arguments_parser.GetLongValue(2),
                              arguments_parser.GetLongValue(3),
                              arguments_parser.GetBoolValue(4));
  } else {
    PrintHelp();
  }

  return exit_code;
}