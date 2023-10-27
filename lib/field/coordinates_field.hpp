#ifndef COORDINATES_VECTOR_HPP
#define COORDINATES_VECTOR_HPP

#include <cstddef>
#include <cstdint>
#include <limits>

struct Point {
  int16_t x;
  int16_t y;
};

class CoordinatesField {
  private:
    static constexpr int16_t kMaxLimit = std::numeric_limits<int16_t>::max() / 3;
    static constexpr int16_t kMinLimit = std::numeric_limits<int16_t>::min() / 3;
    static constexpr int16_t kExpansionMultiplier = 2;
    Point max_point_{};
    Point min_point_{};
    Point positive_capacity_{};
    Point negative_capacity_{};
    uint64_t* data_;

    size_t GetIndexByCoordinates(const Point& point) const;
    bool FitPoint(const Point& size);
  public:
    CoordinatesField();
    CoordinatesField(const CoordinatesField& other);
    CoordinatesField& operator=(const CoordinatesField& other);
    ~CoordinatesField();
    void Resize(const Point& size);
    void Trim();
    void SetElementByCoordinates(const Point& point, uint64_t element);
    uint64_t GetElementByCoordinates(const Point& point);
    Point GetMaxPoint();
    Point GetMinPoint();
};

#endif // COORDINATES_VECTOR_HPP
