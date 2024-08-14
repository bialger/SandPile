#include <fstream>
#include <iostream>
#include <cstring>
#include <charconv>
#include <filesystem>
#include "tsv_handler.hpp"
#include "lib/basic/basic_functions.hpp"

TsvHandler::TsvHandler() = default;

bool TsvHandler::ImportTSV(char* filename, CoordinatesField& field) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    std::cout << "Can't open file: " << filename << std::endl;
    return false;
  }

  char buffer[64]{};
  char temp[64]{};

  while (file.getline(buffer, 64)) {
    char* buffer_ptr = buffer;
    Point current_point = {0, 0};
    uint64_t current_element = 0;

    std::from_chars(buffer_ptr, buffer_ptr + strlen(buffer_ptr),
                    current_point.x);
    buffer_ptr += strlen(i64toa(current_point.x,
                                buffer, 10)) + 1;
    std::from_chars(buffer_ptr, buffer_ptr + strlen(buffer_ptr),
                    current_point.y);
    buffer_ptr += strlen(i64toa(current_point.y,
                                buffer, 10)) + 1;
    std::from_chars(buffer_ptr, buffer_ptr + strlen(buffer_ptr),
                    current_element);
    field.SetElementByCoordinates(current_point, current_element);
    memset(buffer, 0, 64);
    memset(temp, 0, 64);
  }

  return true;
}

bool TsvHandler::ExportTSV(char* dirname, CoordinatesField& field, int64_t number) {
  char suffix[65]{};
  i64toa(number, suffix, 10);
  std::filesystem::path path(dirname);
  path.append("output_");
  path.concat(suffix);
  path.concat(".tsv");

  std::ofstream file(path, std::ios::out);

  if (!file.is_open()) {
    std::cout << "Can't open file: " << path.string() << std::endl;
    return false;
  }

  for (int16_t y = field.GetMaxPoint().y; y >= field.GetMinPoint().y; --y) {
    for (int16_t x = field.GetMinPoint().x; x <= field.GetMaxPoint().x; ++x) {
      if (field[{x, y}] != 0) {
        char temp[64]{};
        file << i64toa(x, temp, 10);
        file.write("\t", 1);
        file << i64toa(y, temp, 10);
        file.write("\t", 1);
        file << ui64toa(field[{x, y}], temp, 10);
        file.write("\n", 1);
      }
    }
  }

  return true;
}