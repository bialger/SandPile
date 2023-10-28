#include "queue.h"

#include <cstring>
#include <limits>

Queue::Queue() {
  data_ = new Point[1];
  size_ = 0;
  front_ = 0;
  capacity_ = 1;
}

Queue::Queue(const Queue &other) {
  data_ = new Point[other.capacity_];
  size_ = other.size_;
  front_ = other.front_;
  capacity_ = other.capacity_;
  std::memcpy(data_, other.data_, sizeof(Point) * size_);
}

Queue &Queue::operator=(const Queue &other) {
  data_ = new Point[other.capacity_];
  size_ = other.size_;
  front_ = other.front_;
  capacity_ = other.capacity_;
  std::memcpy(data_, other.data_, sizeof(Point) * size_);

  return *this;
}

Queue::~Queue() {
  delete[] data_;
}

void Queue::Push(Point element) {
  if (size_ == capacity_) {
    capacity_ *= 2;

    if (capacity_ > std::numeric_limits<uint32_t>::max()) {
      capacity_ = std::numeric_limits<uint32_t>::max();
    }

    Point* new_data = new Point[capacity_];
    std::memset(new_data, 0, sizeof(Point) * capacity_);
    std::memcpy(new_data, data_, sizeof(Point) * size_);
    delete[] data_;
    data_ = new_data;
  }

  data_[size_] = element;
  ++size_;
}

Point Queue::Pop() {
  --size_;
  return data_[front_++];
}

Point Queue::Peek() {
  return data_[front_];
}

bool Queue::IsEmpty() const {
  return size_ == 0;
}
