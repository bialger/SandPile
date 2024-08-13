#ifndef SAND_PILE_HPP
#define SAND_PILE_HPP

#include "lib/field/coordinates_field.hpp"
#include "lib/field/queue.hpp"

/** \n This class represents the Abelian sandpile model. It
 * contains methods for collapsing the given sandpile. */

class SandPile {
 public:
  SandPile();

  /** \n This function takes Abelian sandpile parameters and
   * iteration restrictions and begins the process of collapsing. */

  void BeginCollapsing(char* filename, char* dirname, uint64_t max_iterations, uint64_t frequency, bool write_tsv);

 private:
  static constexpr uint64_t kMaxGrainsInCell = 4;
  CoordinatesField pile_;
  Queue unstable_;
  uint64_t current_iter_;

  bool Collapse();
};

#endif // SAND_PILE_HPP
