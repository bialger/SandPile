#include "sand_pile.hpp"
#include "lib/basic/basic_functions.hpp"
#include "lib/basic/queue.h"
#include "lib/bmp_writer/bmp_writer.hpp"
#include "lib/field/coordinates_field.hpp"

#include <charconv>
#include <cstring>
#include <fstream>
#include <iostream>

SandPile::SandPile() {
  pile_ = CoordinatesField();
  current_iter_ = 0;
}

void SandPile::BeginCollapsing(char* filename, char* dirname,
                               uint64_t max_iterations, uint64_t frequency) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    std::cout << "Can't open file: " << filename << std::endl;
    return;
  }

  char buffer[64];
  char temp[64];
  memset(buffer, 0, 64);
  memset(temp, 0, 64);

  while (file.getline(buffer, 64)) {
    char* buffer_ptr = buffer;
    Point current_point = {0, 0};
    uint64_t current_element = 0;

    std::from_chars(buffer_ptr, buffer_ptr + strlen(buffer_ptr),
                    current_point.x);
    buffer_ptr += strlen(i64toa(static_cast<int64_t>(current_point.x),
                                buffer, 10)) + 1;
    std::from_chars(buffer_ptr, buffer_ptr + strlen(buffer_ptr),
                    current_point.y);
    buffer_ptr += strlen(i64toa(static_cast<int64_t>(current_point.y),
                                buffer, 10)) + 1;
    std::from_chars(buffer_ptr, buffer_ptr + strlen(buffer_ptr),
                    current_element);
    pile_.SetElementByCoordinates(current_point, current_element);
    memset(buffer, 0, 64);
    memset(temp, 0, 64);
  }

  BmpWriter bmp_writer = BmpWriter();
  bmp_writer.ExportField(dirname, pile_, 0);

  while (current_iter_ < max_iterations && Collapse()) {
    ++current_iter_;

    if (current_iter_ % frequency == 0) {
      bmp_writer.ExportField(dirname, pile_,
                             static_cast<int64_t>(current_iter_));
    }
  }

  bmp_writer.ExportField(dirname, pile_,
                         static_cast<int64_t>(current_iter_));
}

bool SandPile::Collapse() {
  Queue unstable = Queue();
  bool has_unstable = false;

  for (int16_t y = pile_.GetMaxPoint().y; y >= pile_.GetMinPoint().y; --y) {
    for (int16_t x = pile_.GetMinPoint().x; x <= pile_.GetMaxPoint().x; ++x) {
      if (pile_.GetElementByCoordinates({x, y}) >= 4) {
        unstable.Push({x, y});
        has_unstable = true;
      }
    }
  }

  while (!unstable.IsEmpty()) {
    const Point unstable_point = unstable.Pop();
    int16_t x = unstable_point.x;
    int16_t y = unstable_point.y;
    pile_.SetElementByCoordinates(unstable_point,pile_.GetElementByCoordinates(
                                      unstable_point) - 4);
    ++x;

    if (pile_.GetMaxPoint().x == unstable_point.x) {
      pile_.SetElementByCoordinates({x, y}, 1);
    } else {
      pile_.SetElementByCoordinates({x, y},pile_.GetElementByCoordinates(
                                                {x, y}) + 1);
    }

    x -= 2;

    if (pile_.GetMinPoint().x == unstable_point.x) {
      pile_.SetElementByCoordinates({x, y}, 1);
    } else {
      pile_.SetElementByCoordinates({x, y}, pile_.GetElementByCoordinates(
                                                {x, y}) + 1);
    }

    ++x;
    ++y;

    if (pile_.GetMaxPoint().y == unstable_point.y) {
      pile_.SetElementByCoordinates({x, y}, 1);
    } else {
      pile_.SetElementByCoordinates({x, y},pile_.GetElementByCoordinates(
                                                {x, y}) + 1);
    }

    y -= 2;

    if (pile_.GetMinPoint().y == unstable_point.y) {
      pile_.SetElementByCoordinates({x, y}, 1);
    } else {
      pile_.SetElementByCoordinates({x, y}, pile_.GetElementByCoordinates(
                                                {x, y}) + 1);
    }
  }

  return has_unstable;
}
