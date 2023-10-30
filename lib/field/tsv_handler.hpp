#ifndef TSV_HANDLER_HPP
#define TSV_HANDLER_HPP

#include "coordinates_field.hpp"

class TsvHandler {
 public:
  TsvHandler();
  static bool ImportTSV(char* filename, CoordinatesField& field);
  static bool ExportTSV(char* dirname, CoordinatesField& field, int64_t number);
};

#endif //TSV_HANDLER_HPP
