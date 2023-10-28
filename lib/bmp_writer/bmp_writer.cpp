#include "bmp_writer.hpp"
#include "lib/basic/basic_functions.hpp"

#include <cstring>

BmpWriter::BmpWriter() {
  width_ = 0;
  height_ = 0;
  padding_amount_ = 4;
  file_size_ = 0;
  file_ = std::ofstream{};
}

void BmpWriter::ExportField(char* dirname, CoordinatesField& field,
                               int64_t number) {
  char* new_filename = new char[strlen(dirname) + 1 + 8 + 1 + 64 + 4 + 1];
  char suffix[65];
  memset(new_filename, 0, strlen(dirname) + 1 + 8 + 1 + 64 + 4 + 1);
  memset(suffix, 0, 65);
  i64toa(number, suffix, 10);
  strcpy(new_filename, dirname);

  if (dirname[strlen(dirname) - 1] != '/') {
    strcat(new_filename, "/");
  }

  strcat(new_filename, "output_");
  strcat(new_filename, suffix);
  strcat(new_filename, ".bmp");
  file_.open(new_filename, std::ios::out | std::ios::binary);

  if (!file_.is_open()) {
    std::cout << "Can't open file: " << new_filename << std::endl;
    return;
  }

  width_ = static_cast<uint16_t>(field.GetMaxPoint().x - field.GetMinPoint().x + 1);
  height_ = static_cast<uint16_t>(field.GetMaxPoint().y - field.GetMinPoint().y + 1);
  padding_amount_ = static_cast<uint8_t>(4 - (width_ / 2 + width_ % 2) % 4) % 4;
  file_size_ = kBmpInfoHeaderSize + kFileHeaderSize + kColorTableSize +
               static_cast<uint32_t>((width_ / 2 + width_ % 2) * height_) +
               padding_amount_ * height_;

  WriteHeader();
  WritePixels(field);

  file_.close();
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
  color_table[3] = 0;

  color_table[4] = 0;
  color_table[5] = 0xFF;
  color_table[6] = 0;
  color_table[7] = 0;

  color_table[8] = 0;
  color_table[9] = 0xFF;
  color_table[10] = 0xFF;
  color_table[11] = 0;

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

void BmpWriter::WritePixels(CoordinatesField& field) {
  uint32_t counter = 0;
  size_t byte_string_size = static_cast<size_t>(width_ / 2 + width_ % 2 +
                                                padding_amount_);
  uint8_t* byte_string = new uint8_t[byte_string_size];
  memset(byte_string, 0, byte_string_size);
  uint8_t shift;

  for (int16_t y = field.GetMinPoint().y; y <= field.GetMaxPoint().y; ++y) {
    for (int16_t x = field.GetMinPoint().x; x <= field.GetMaxPoint().x; ++x) {
      Point current_point = {x, y};
      uint64_t current_element = field.GetElementByCoordinates(current_point);
      shift = (counter % 2 == 0) ? 4 : 0;
      if (current_element < 4) {
        byte_string[counter / 2] |= static_cast<uint8_t>(current_element << shift);
      } else {
        byte_string[counter / 2] |= 4 << shift;
      }

      ++counter;
    }

    file_.write(reinterpret_cast<char*>(byte_string),
                static_cast<ptrdiff_t>(byte_string_size));
    memset(byte_string, 0, byte_string_size);
    counter = 0;
  }

  delete[] byte_string;
}
