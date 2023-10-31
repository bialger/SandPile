#include "queue.hpp"

#include <cstring>
#include <limits>

Queue::Queue() {
  size_ = 0;
  head_ = nullptr;
  tail_ = nullptr;
}

Queue::~Queue() {
  while (size_ > 0) {
    Pop();
  }
}

void Queue::Push(Point element) {
  Node* new_node = new Node{element, nullptr};

  if (size_ != 0) {
    tail_->next = new_node;
  } else {
    head_ = new_node;
  }

  tail_ = new_node;
  ++size_;
}

Point Queue::Pop() {
  if (size_ == 0) {
    return {};
  }

  --size_;
  Node* element = head_;
  Point head_data = head_->data;
  head_ = head_->next;
  delete element;

  return head_data;
}

Point Queue::Peek() {
  if (size_ == 0) {
    return {};
  }

  return head_->data;
}

Point Queue::GetTail() {
  return tail_->data;
}

size_t Queue::GetSize() const {
  return size_;
}

bool Queue::IsEmpty() const {
  return size_ == 0;
}
