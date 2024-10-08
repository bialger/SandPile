#ifndef COORDINATES_VECTOR_HPP
#define COORDINATES_VECTOR_HPP

#include <cstddef>
#include <cstdint>
#include <limits>

struct Point {
  int16_t x = 0;
  int16_t y = 0;
};

class CoordinatesField {
 public:
  CoordinatesField();
  CoordinatesField(const CoordinatesField& other);
  CoordinatesField& operator=(const CoordinatesField& other);
  ~CoordinatesField();
  void Resize(const Point& size);
  void Trim();
  void SetElementByCoordinates(const Point& point, uint64_t element);
  uint64_t GetElementByCoordinates(const Point& point) const;
  uint64_t& operator[](const Point& point) const;
  Point GetMaxPoint() const;
  Point GetMinPoint() const;

 private:
  static constexpr int16_t kMaxLimit = std::numeric_limits<int16_t>::max() / 3;
  static constexpr int16_t kMinLimit = std::numeric_limits<int16_t>::min() / 3;
  static constexpr int16_t kExpansionMultiplier = 2;
  Point max_point_;
  Point min_point_;
  Point positive_capacity_;
  Point negative_capacity_;
  uint64_t* data_;

  size_t GetIndexByCoordinates(const Point& point) const;
  bool FitPoint(const Point& size);
  void ActualizeData(const Point& old_positive_capacity, const Point& old_negative_capacity);
};

#endif // COORDINATES_VECTOR_HPP
