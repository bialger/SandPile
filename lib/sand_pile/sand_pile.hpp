#ifndef SAND_PILE_HPP
#define SAND_PILE_HPP

#include "lib/field/coordinates_field.hpp"

class SandPile {
 private:
  CoordinatesField pile_;
  uint64_t current_iter_;

  bool Collapse();
 public:
  SandPile();
  void BeginCollapsing(char* filename, char* dirname, uint64_t max_iterations,
                       uint64_t frequency);
};

#endif // SAND_PILE_HPP
