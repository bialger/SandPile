#ifndef COORDINATES_VECTOR_HPP
#define COORDINATES_VECTOR_HPP

#include <cstddef>
#include <cstdint>

struct Point {
  int16_t x;
  int16_t y;
};

class CoordinatesVector {
  private:
    Point positive_capacity_{};
    Point negative_capacity_{};
    uint64_t* data_;

    size_t GetIndexByCoordinates(Point point) const;
    bool FitPoint(Point size);
  public:
    Point max_point_{};
    Point min_point_{};

    CoordinatesVector();
    ~CoordinatesVector();
    void Resize(Point size);
    void SetElementByCoordinates(Point point, uint64_t element);
    uint64_t GetElementByCoordinates(Point point);
};

#endif // COORDINATES_VECTOR_HPP
