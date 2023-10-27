#ifndef BMP_WRITER_HPP
#define BMP_WRITER_HPP

#include "lib/field/coordinates_field.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>

class BmpWriter {
private:
  static constexpr uint32_t kFileHeaderSize = 14;
  static constexpr uint32_t kBmpInfoHeaderSize = 40;
  static constexpr uint32_t kColorTableSize = 42;
  uint16_t width_;
  uint16_t height_;
  uint8_t padding_amount_;
  uint32_t file_size_;
  uint8_t* data_;
  std::ofstream file_;

  void WriteHeader();
  void WritePixels();

public:
  BmpWriter();
  void ExportSandPile(char*dirname, CoordinatesField& sand_pile_field,
                      int64_t iteration);
  ~BmpWriter();
};

#endif // BMP_WRITER_HPP
