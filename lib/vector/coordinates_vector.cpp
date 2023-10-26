#include "coordinates_vector.hpp"

#include <iostream>
#include <cstring>

CoordinatesVector::CoordinatesVector() {
  max_point_ = {0, 0};
  min_point_ = {0, 0};
  positive_capacity_ = {2, 2};
  negative_capacity_ = {-2, -2};
  data_ = new uint64_t[25];
  std::memset(data_, 0, sizeof(uint64_t) * 25);
}

CoordinatesVector::~CoordinatesVector() {
  delete[] data_;
}

size_t CoordinatesVector::GetIndexByCoordinates(Point point) const {
  return static_cast<size_t>(point.x - negative_capacity_.x) *
         static_cast<size_t>(positive_capacity_.y - negative_capacity_.y + 1) +
         static_cast<size_t>(point.y - negative_capacity_.y);
}

bool CoordinatesVector::FitPoint(Point size) {
  bool do_resize = false;

  if (size.x > positive_capacity_.x) {
    positive_capacity_.x = size.x;
    do_resize = true;

    if (negative_capacity_.x < kMaxLimit) {
      positive_capacity_.x *= kExpansionMultiplier;
    }
  } else if (size.x < negative_capacity_.x) {
    negative_capacity_.x = size.x;
    do_resize = true;

    if (negative_capacity_.x > kMinLimit) {
      negative_capacity_.x *= kExpansionMultiplier;
    }
  }

  if (size.y > positive_capacity_.y) {
    positive_capacity_.y = size.y;
    do_resize = true;

    if (negative_capacity_.y < kMaxLimit) {
      positive_capacity_.y *= kExpansionMultiplier;
    }
  } else if (size.y < negative_capacity_.y) {
    negative_capacity_.y = size.y;
    do_resize = true;

    if (negative_capacity_.y > kMinLimit) {
      negative_capacity_.y *= kExpansionMultiplier;
    }
  }

  return do_resize;
}

void CoordinatesVector::Resize(Point size) {
  Point old_positive_capacity = positive_capacity_;
  Point old_negative_capacity = negative_capacity_;

  if (!FitPoint(size)) {
    return;
  }

  size_t new_size = static_cast<size_t>(positive_capacity_.x - negative_capacity_.x + 1) *
                  static_cast<size_t>(positive_capacity_.y - negative_capacity_.y + 1);

  uint64_t* new_data = new uint64_t[new_size];

  std::memset(new_data, 0, sizeof(uint64_t) * new_size);

  for (int16_t y = max_point_.y; y >= min_point_.y; --y) {
    for (int16_t x = min_point_.x; x <= max_point_.x; ++x) {
      size_t old_index = static_cast<size_t>(x - old_negative_capacity.x) *
             static_cast<size_t>(old_positive_capacity.y - old_negative_capacity.y + 1) +
             static_cast<size_t>(y - old_negative_capacity.y);
      new_data[GetIndexByCoordinates({x, y})] = data_[old_index];
    }
  }

  delete[] data_;
  data_ = new_data;
}

void CoordinatesVector::Trim() {
  Point old_positive_capacity = positive_capacity_;
  Point old_negative_capacity = negative_capacity_;
  positive_capacity_ = max_point_;
  negative_capacity_ = min_point_;

  size_t new_size = static_cast<size_t>(positive_capacity_.x - negative_capacity_.x + 1) *
                    static_cast<size_t>(positive_capacity_.y - negative_capacity_.y + 1);

  uint64_t* new_data = new uint64_t[new_size];

  for (size_t i = 0; i < new_size; i++) {
    new_data[i] = 0;
  }

  for (int16_t y = max_point_.y; y >= min_point_.y; --y) {
    for (int16_t x = min_point_.x; x <= max_point_.x; ++x) {
      size_t old_index = static_cast<size_t>(x - old_negative_capacity.x) *
                             static_cast<size_t>(old_positive_capacity.y - old_negative_capacity.y + 1) +
                         static_cast<size_t>(y - old_negative_capacity.y);
      new_data[GetIndexByCoordinates({x, y})] = data_[old_index];
    }
  }

  delete[] data_;
  data_ = new_data;
}

void CoordinatesVector::SetElementByCoordinates(Point point, uint64_t element) {
  Resize(point);

  if (point.x > max_point_.x) {
    max_point_.x = point.x;
  } else if (point.x < min_point_.x) {
    min_point_.x = point.x;
  }

  if (point.y > max_point_.y) {
    max_point_.y = point.y;
  } else if (point.y < min_point_.y) {
    min_point_.y = point.y;
  }

  data_[GetIndexByCoordinates(point)] = element;
}

uint64_t CoordinatesVector::GetElementByCoordinates(Point point) {
  return data_[GetIndexByCoordinates(point)];
}

Point CoordinatesVector::GetMaxPoint() {
  return max_point_;
}

Point CoordinatesVector::GetMinPoint() {
  return min_point_;
}
