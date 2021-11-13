#include "ClockReplacer.h"

#include <cstdio>

ClockReplacer::ClockReplacer(int frame_num)
    : frame_num_(frame_num), size_(0), ptr_(nullptr) {}

ClockReplacer::~ClockReplacer() {
  if (ptr_ == nullptr)
    return;
  ListNode *p, *tmp;
  for (p = ptr_->next; p != ptr_; p = tmp) {
    tmp = p->next;
    delete p;
  }
  delete ptr_;
}

frame_id_t ClockReplacer::Victim() {
  frame_id_t vic;
  for (;;) {
    if (ptr_->second_chance == false) {
      vic = ptr_->val;
      Remove(vic);
      return vic;
    }
    ptr_->second_chance = false;
    ptr_ = ptr_->next;
  }
}

void ClockReplacer::Insert(frame_id_t frame_id) {
  if (exists(frame_id)) {
    return;
  }
  if (size_ == 0) {
    ptr_ = new ListNode(frame_id);
    ptr_->next = ptr_;
    ptr_->prev = ptr_;
    size_++;
    return;
  }
  if (size_ < frame_num_) {
    ListNode *tmp = ptr_->prev;
    ListNode *new_node = new ListNode(frame_id);
    ptr_->prev = new_node;
    new_node->next = ptr_;
    new_node->prev = tmp;
    tmp->next = new_node;
    size_++;
    return;
  }
  for (;;) {
    if (ptr_->second_chance == false) {
      ptr_->second_chance = true;
      ptr_->val = frame_id;
      return;
    }
    ptr_->second_chance = false;
    ptr_ = ptr_->next;
  }
}

void ClockReplacer::Remove(frame_id_t frame_id) {
  if (ptr_ == nullptr)
    return;
  if (ptr_->val == frame_id) {
    ListNode *tmp = (ptr_->next == ptr_ ? nullptr : ptr_->next);
    remove(ptr_);
    ptr_ = tmp;
    return;
  }
  ListNode *p;
  for (p = ptr_->next; p != ptr_; p = p->next) {
    if (p->val == frame_id) {
      remove(p);
      break;
    }
  }
}

void ClockReplacer::Print() {
  if (ptr_ == nullptr)
    return;
  ListNode *p;
  printf("%d(%d) -> ", ptr_->val, ptr_->second_chance);
  for (p = ptr_->next; p != ptr_; p = p->next) {
    printf("%d(%d) -> ", p->val, p->second_chance);
  }
  printf("\n");
}

int ClockReplacer::Size() { return size_; }

bool ClockReplacer::exists(frame_id_t frame_id) {
  if (ptr_ == nullptr)
    return false;
  if (ptr_->val == frame_id) {
    ptr_->second_chance = true;
    return true;
  }
  for (ListNode *p = ptr_->next; p != ptr_; p = p->next) {
    if (p->val == frame_id) {
      p->second_chance = true;
      return true;
    }
  }
  return false;
}

void ClockReplacer::remove(ListNode *node) {
  if (size_ == 1) {
    delete node;
    size_ = 0;
    return;
  }
  node->prev->next = node->next;
  node->next->prev = node->prev;
  size_--;
  delete node;
}
