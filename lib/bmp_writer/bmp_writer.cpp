#include "bmp_writer.hpp"
#include "lib/basic/basic_functions.hpp"

#include <cstring>

BmpWriter::BmpWriter() {
  width_ = 1;
  height_ = 1;
  padding_amount_ = 3;
  file_size_ = 1;
  data_ = new uint8_t[1];
  file_ = std::ofstream{};
}

BmpWriter::~BmpWriter() {
  delete[] data_;
}

void BmpWriter::ExportSandPile(char*dirname, CoordinatesField& sand_pile_field,
                               int64_t iteration) {
  char* new_filename = new char[strlen(dirname) + 1 + 8 + 1 + 64 + 4 + 1];
  char temp[65];
  memset(new_filename, 0, strlen(dirname) + 1 + 8 + 1 + 64 + 4 + 1);
  memset(temp, 0, 65);
  i64toa(iteration, temp, 10);
  strcpy(new_filename, dirname);
  if (dirname[strlen(dirname) - 1] != '/') {
    strcat(new_filename, "/");
  }
  strcat(new_filename, "output");
  strcat(new_filename, "_");
  strcat(new_filename, temp);
  strcat(new_filename, ".bmp");
  file_.open(new_filename, std::ios::out | std::ios::binary);

  if (!file_.is_open()) {
    std::cout << "Can't open file: " << new_filename << std::endl;
    return;
  }

  width_ = static_cast<uint16_t>(sand_pile_field.GetMaxPoint().x -
                                 sand_pile_field.GetMinPoint().x + 1);
  height_ = static_cast<uint16_t>(sand_pile_field.GetMaxPoint().y -
                                  sand_pile_field.GetMinPoint().y + 1);
  padding_amount_ = static_cast<uint8_t>(4 - (width_ / 2 + width_ % 2) % 4) % 4;
  file_size_ = kBmpInfoHeaderSize + kFileHeaderSize + kColorTableSize +
               static_cast<uint32_t>((width_ / 2 + width_ % 2) * height_) +
               padding_amount_ * height_;
  delete[] data_;
  data_ = new uint8_t[file_size_ -
                      (kFileHeaderSize + kBmpInfoHeaderSize + kColorTableSize)];
  memset(data_, 0, file_size_ -
                       (kFileHeaderSize + kBmpInfoHeaderSize + kColorTableSize));
  size_t counter = 0;

  for (int16_t y = sand_pile_field.GetMinPoint().y; y <= sand_pile_field.GetMaxPoint().y; ++y) {
    for (int16_t x = sand_pile_field.GetMinPoint().x; x <= sand_pile_field.GetMaxPoint().x; ++x) {
      Point current_point = {x, y};
      int8_t bit_shift = (counter % 2 == 0) ? 4 : 0;
      if (sand_pile_field.GetElementByCoordinates(current_point) == 0) {
        data_[counter / 2] |= 0 << bit_shift;
      } else if (sand_pile_field.GetElementByCoordinates(current_point) == 1) {
        data_[counter / 2] |= 1 << bit_shift;
      } else if (sand_pile_field.GetElementByCoordinates(current_point) == 2) {
        data_[counter / 2] |= 2 << bit_shift;
      } else if (sand_pile_field.GetElementByCoordinates(current_point) == 3) {
        data_[counter / 2] |= 3 << bit_shift;
      } else {
        data_[counter / 2] |= 7 << bit_shift;
      }
      ++counter;
    }

    if (counter % 2 != 0) {
      ++counter;
    }

    counter += padding_amount_ * 2;
  }

  WriteHeader();
  WritePixels();

  delete[] new_filename;
}

void BmpWriter::WriteHeader() {
  uint8_t bmp_header[kFileHeaderSize];
  uint8_t dib_header[kBmpInfoHeaderSize];
  uint8_t color_table[kColorTableSize];
  memset(bmp_header, 0, kFileHeaderSize);
  memset(dib_header, 0, kBmpInfoHeaderSize);
  memset(color_table, 0, kColorTableSize);

  /* Standard BMP header */

  bmp_header[0] = 'B';
  bmp_header[1] = 'M';
  bmp_header[2] = static_cast<uint8_t>(file_size_ & 0xff);
  bmp_header[3] = static_cast<uint8_t>(file_size_ >> 8 & 0xff);
  bmp_header[4] = static_cast<uint8_t>(file_size_ >> 16 & 0xff);
  bmp_header[5] = static_cast<uint8_t>(file_size_ >> 24);
  bmp_header[6] = 0;
  bmp_header[7] = 0;
  bmp_header[8] = 0;
  bmp_header[9] = 0;
  bmp_header[10] = (kFileHeaderSize + kBmpInfoHeaderSize + kColorTableSize) & 0xff;
  bmp_header[11] = 0;
  bmp_header[12] = 0;
  bmp_header[13] = 0;

  /* Standard BMP information header */

  dib_header[0] = kBmpInfoHeaderSize;
  dib_header[1] = 0;
  dib_header[2] = 0;
  dib_header[3] = 0;
  dib_header[4] = static_cast<uint8_t>(width_ & 0xff);
  dib_header[5] = static_cast<uint8_t>(width_ >> 8);
  dib_header[6] = 0;
  dib_header[7] = 0;
  dib_header[8] = static_cast<uint8_t>(height_ & 0xff);
  dib_header[9] = static_cast<uint8_t>(height_ >> 8);
  dib_header[10] = 0;
  dib_header[11] = 0;
  dib_header[12] = 1;
  dib_header[13] = 0;
  dib_header[14] = 4;
  dib_header[15] = 0;
  /* Other bytes are zero, except of color palette: it is five */
  dib_header[32] = 5;

  /* Color table for five colors: white, green, yellow, violet, black. */

  color_table[0] = 0xFF;
  color_table[1] = 0xFF;
  color_table[2] = 0xFF;
  color_table[3] = 0xFF;

  color_table[4] = 0;
  color_table[5] = 0xFF;
  color_table[6] = 0;
  color_table[7] = 0xFF;

  color_table[8] = 0;
  color_table[9] = 0xFF;
  color_table[10] = 0xFF;
  color_table[11] = 0xFF;

  color_table[12] = 0xFF;
  color_table[13] = 0;
  color_table[14] = 0xFF;
  color_table[15] = 0;

  color_table[16] = 0;
  color_table[17] = 0;
  color_table[18] = 0;
  color_table[19] = 0;

  color_table[20] = 0;
  color_table[21] = 0;
  color_table[22] = 0;
  color_table[23] = 0;
  color_table[24] = 0;
  color_table[25] = 0;
  color_table[26] = 0x43;
  color_table[27] = 0x79;
  color_table[28] = 0x70;
  color_table[29] = 0x69;
  color_table[30] = 0x73;
  color_table[31] = 0x20;
  color_table[32] = 0x6e;
  color_table[33] = 0x61;
  color_table[34] = 0x6a;
  color_table[35] = 0x6c;
  color_table[36] = 0x65;
  color_table[37] = 0x70;
  color_table[38] = 0x73;
  color_table[39] = 0x7a;
  color_table[40] = 0x79;
  color_table[41] = 0x21;

  file_.write(reinterpret_cast<char*>(bmp_header), kFileHeaderSize);
  file_.write(reinterpret_cast<char*>(dib_header), kBmpInfoHeaderSize);
  file_.write(reinterpret_cast<char*>(color_table), kColorTableSize);
}

void BmpWriter::WritePixels() {
  file_.write(reinterpret_cast<char*>(data_),
              file_size_ - (kFileHeaderSize + kBmpInfoHeaderSize + kColorTableSize));
}
