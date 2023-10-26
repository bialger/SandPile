#include "coordinates_vector.hpp"

#include <iostream>
#include <limits>

CoordinatesVector::CoordinatesVector() {
  max_point_ = {0, 0};
  min_point_ = {0, 0};
  positive_capacity_ = {2, 2};
  negative_capacity_ = {-2, -2};
  data_ = new uint64_t[25];
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
    if (size.x < std::numeric_limits<int16_t>::max() / 2) {
      positive_capacity_.x = size.x;
      positive_capacity_.x *= 2;
    } else {
      positive_capacity_.x = size.x;
    }

    do_resize = true;
  } else if (size.x < negative_capacity_.x) {
    if (size.x < std::numeric_limits<int16_t>::max() / 2) {
      negative_capacity_.x = size.x;
      negative_capacity_.x *= 2;
    } else {
      negative_capacity_.x = size.x;
    }

    do_resize = true;
  }

  if (size.y > positive_capacity_.y) {
    if (size.y < std::numeric_limits<int16_t>::max() / 2) {
      positive_capacity_.y = size.y;
      positive_capacity_.y *= 2;
    } else {
      positive_capacity_.y = size.y;
    }

    do_resize = true;
  } else if (size.y < negative_capacity_.y) {
    if (size.y < std::numeric_limits<int16_t>::max() / 2) {
      negative_capacity_.y = size.y;
      negative_capacity_.y *= 2;
    } else {
      negative_capacity_.y = size.y;
    }

    do_resize = true;
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
