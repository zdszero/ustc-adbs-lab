#include <cstdio>

#include "LRUReplacer.h"

LRUReplacer::LRUReplacer(int max_size): max_size_(max_size) {
  size_ = 0;
  head_ = new ListNode();
  tail_ = new ListNode();
  head_->next = tail_;
  tail_->prev = head_;
}

LRUReplacer::~LRUReplacer() {
  ListNode *p = head_, *tmp;
  for (; p != nullptr; p = tmp) {
    tmp = p->next;
    delete p;
  }
}


frame_id_t LRUReplacer::Victim() {
  if (size_ == 0)
    return -1;
  frame_id_t ret;
  ret = head_->next->val;
  pop_node();
  return ret;
}

void LRUReplacer::Insert(frame_id_t frame_id) {
  auto iter = map_.find(frame_id);
  if (iter == map_.end()) {
    if (size_ == max_size_)
      pop_node();
    append_node(new ListNode(frame_id));
    return;
  }
  ListNode *tmp = iter->second;
  set_pointer(tmp->prev, tmp->next);
  size_--;
  append_node(tmp);
}

void LRUReplacer::Remove(frame_id_t frame_id) {
  auto iter = map_.find(frame_id);
  if (iter == map_.end()) {
    return;
  }
  ListNode *tmp = iter->second;
  set_pointer(tmp->prev, tmp->next);
  map_.erase(frame_id);
  size_--;
  delete tmp;
}

int LRUReplacer::Size() {
  return size_;
}

void LRUReplacer::Print() {
  printf("lru replacer:\n");
  for (ListNode *p = head_->next; p != tail_; p = p->next) {
    printf("%d ", p->val);
  }
  printf("\n");
}

void LRUReplacer::pop_node() {
  ListNode *tmp = head_->next;
  set_pointer(head_, tmp->next);
  map_.erase(tmp->val);
  delete tmp;
  size_--;
}

void LRUReplacer::append_node(ListNode *new_node) {
  ListNode *tmp = tail_->prev;
  map_[new_node->val] = new_node;
  set_pointer(tmp, new_node);
  set_pointer(new_node, tail_);
  size_++;
}
