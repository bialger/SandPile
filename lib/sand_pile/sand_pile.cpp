#include "sand_pile.hpp"
#include "lib/bmp_writer/bmp_writer.hpp"
#include "lib/field/coordinates_field.hpp"
#include "lib/field/queue.hpp"
#include "lib/field/tsv_handler.hpp"

SandPile::SandPile() {
  pile_ = CoordinatesField();
  current_iter_ = 0;
}

void SandPile::BeginCollapsing(char* filename, char* dirname,
                               uint64_t max_iterations, uint64_t frequency, bool write_tsv) {
  TsvHandler::ImportTSV(filename, pile_);

  BmpWriter bmp_writer = BmpWriter();
  bmp_writer.ExportField(dirname, pile_, 0);

  if (write_tsv) {
    TsvHandler::ExportTSV(dirname, pile_, 0);
  }

  if (frequency == 0) {
    return;
  }

  while (current_iter_ < max_iterations && Collapse()) {
    ++current_iter_;

    if (current_iter_ % frequency == 0) {
      bmp_writer.ExportField(dirname, pile_,
                             static_cast<int64_t>(current_iter_));

      if (write_tsv) {
        TsvHandler::ExportTSV(dirname, pile_,
                              static_cast<int64_t>(current_iter_));
      }
    }
  }

  bmp_writer.ExportField(dirname, pile_,
                         static_cast<int64_t>(current_iter_));

  if (write_tsv) {
    TsvHandler::ExportTSV(dirname, pile_,
                          static_cast<int64_t>(current_iter_));
  }
}

bool SandPile::Collapse() {
  Queue unstable = Queue();
  bool has_unstable = false;

  for (int16_t y = pile_.GetMaxPoint().y; y >= pile_.GetMinPoint().y; --y) {
    for (int16_t x = pile_.GetMinPoint().x; x <= pile_.GetMaxPoint().x; ++x) {
      if (pile_[{x, y}] >= kMaxGrainsInCell) {
        unstable.Push({x, y});
        has_unstable = true;
      }
    }
  }

  while (!unstable.IsEmpty()) {
    const Point unstable_point = unstable.Pop();
    int16_t x = unstable_point.x;
    int16_t y = unstable_point.y;
    pile_[unstable_point] = pile_[unstable_point] - kMaxGrainsInCell;
    ++x;

    if (pile_.GetMaxPoint().x == unstable_point.x) {
      pile_.SetElementByCoordinates({x, y}, 1);
    } else {
      pile_[{x, y}] += 1;
    }

    x -= 2;

    if (pile_.GetMinPoint().x == unstable_point.x) {
      pile_.SetElementByCoordinates({x, y}, 1);
    } else {
      pile_[{x, y}] += 1;
    }

    ++x;
    ++y;

    if (pile_.GetMaxPoint().y == unstable_point.y) {
      pile_.SetElementByCoordinates({x, y}, 1);
    } else {
      pile_[{x, y}] += 1;
    }

    y -= 2;

    if (pile_.GetMinPoint().y == unstable_point.y) {
      pile_.SetElementByCoordinates({x, y}, 1);
    } else {
      pile_[{x, y}] += 1;
    }
  }

  return has_unstable;
}
