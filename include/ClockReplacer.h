#pragma once

#include "Replacer.h"

class ClockReplacer: public Replacer {
  struct ListNode {
    ListNode(int _val = -1): second_chance(true), val(_val) {}
    bool second_chance;
    frame_id_t val;
    ListNode *next, *prev;
  };
public:
  explicit ClockReplacer(int frame_num);
  virtual ~ClockReplacer();
  frame_id_t Victim();
  void Insert(frame_id_t frame_id);
  void Remove(frame_id_t frame_id);
  void Print();
  int Size();

private:
  bool exists(frame_id_t frame_id);
  void remove(ListNode *node);
  int frame_num_;
  ListNode *ptr_;
  int size_;
};
