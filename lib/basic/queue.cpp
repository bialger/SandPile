#include "queue.h"

#include <cstring>
#include <limits>

Queue::Queue() {
  data_ = new uint64_t[1];
  size_ = 0;
  front_ = 0;
  capacity_ = 1;
}

Queue::Queue(const Queue &other) {
  data_ = new uint64_t[other.capacity_];
  size_ = other.size_;
  front_ = other.front_;
  capacity_ = other.capacity_;
  std::memcpy(data_, other.data_, sizeof(uint64_t) * size_);
}

Queue &Queue::operator=(const Queue &other) {
  data_ = new uint64_t[other.capacity_];
  size_ = other.size_;
  front_ = other.front_;
  capacity_ = other.capacity_;
  std::memcpy(data_, other.data_, sizeof(uint64_t) * size_);

  return *this;
}

Queue::~Queue() {
  delete[] data_;
}

void Queue::Push(uint64_t element) {
  if (size_ == capacity_) {
    capacity_ *= 2;

    if (capacity_ > std::numeric_limits<uint32_t>::max()) {
      capacity_ = std::numeric_limits<uint32_t>::max();
    }

    uint64_t* new_data = new uint64_t[capacity_];
    std::memset(new_data, 0, sizeof(uint64_t) * capacity_);
    std::memcpy(new_data, data_, sizeof(uint64_t) * size_);
    delete[] data_;
    data_ = new_data;
  }

  data_[size_] = element;
  ++size_;
}

uint64_t Queue::Pop() {
  --size_;
  return data_[front_++];
}

uint64_t Queue::Peek() {
  return data_[front_];
}

bool Queue::IsEmpty() const {
  return size_ == 0;
}
