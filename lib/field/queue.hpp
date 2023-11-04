#ifndef SANDPILE_QUEUE_HPP
#define SANDPILE_QUEUE_HPP

#include "coordinates_field.hpp"

#include <cstddef>
#include <cstdint>

struct Node {
  Point data{};
  Node* next = nullptr;

  Node() = default;
  explicit Node(const Point& data) : data(data) {}
};

class Queue {
 public:
  Queue();
  Queue(const Queue& other);
  Queue& operator=(const Queue& other);
  ~Queue();
  void Push(Point element);
  void AddAllElements(Queue& other) const;
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
