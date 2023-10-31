#ifndef SANDPILE_QUEUE_HPP
#define SANDPILE_QUEUE_HPP

#include "coordinates_field.hpp"

#include <cstddef>
#include <cstdint>

struct Node {
  Point data;
  Node* next;
};

class Queue {
 public:
  Queue();
  ~Queue();
  void Push(Point element);
  Point Pop();
  Point Peek();
  Point GetTail();
  size_t GetSize() const;
  bool IsEmpty() const;

 private:
  Node* head_;
  Node* tail_;
  size_t size_;
};

#endif // SANDPILE_QUEUE_HPP
