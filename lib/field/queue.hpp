#ifndef SANDPILE_QUEUE_HPP
#define SANDPILE_QUEUE_HPP

#include "coordinates_field.hpp"

#include <cstddef>
#include <cstdint>

class Queue {
 public:
  Queue();
  Queue(const Queue& other);
  Queue& operator=(const Queue& other);
  ~Queue();
  void Push(Point element);
  Point Pop();
  Point Peek();
  bool IsEmpty() const;

 private:
  Point* data_;
  size_t size_;
  size_t front_;
  size_t capacity_;
};

#endif // SANDPILE_QUEUE_HPP
