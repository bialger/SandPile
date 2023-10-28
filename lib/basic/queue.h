#ifndef SANDPILE_QUEUE_H
#define SANDPILE_QUEUE_H

#include <cstddef>
#include <cstdint>

class Queue {
private:
  uint64_t* data_;
  size_t size_;
  size_t front_;
  size_t capacity_;
public:
  Queue();
  Queue(const Queue& other);
  Queue& operator=(const Queue& other);
  ~Queue();
  void Push(uint64_t element);
  uint64_t Pop();
  uint64_t Peek();
  bool IsEmpty() const;
};

#endif // SANDPILE_QUEUE_H
