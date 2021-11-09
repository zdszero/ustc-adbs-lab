#include "Replacer.h"

#include <unordered_map>

struct ListNode {
  ListNode() = default;
  ListNode(frame_id_t _val): val(_val) {}
  frame_id_t val;
  ListNode *prev, *next;
};

class LRUReplacer: public Replacer {
public:
  LRUReplacer(int max_size);
  ~LRUReplacer();
  frame_id_t Victim();
  void Insert(frame_id_t frame_id);
  void Remove(frame_id_t frame_id);
  int Size();

private:
  inline void set_pointer(ListNode *p, ListNode *q) {
    p->next = q;
    q->prev = p;
  }

  void pop_node();
  void append_node(ListNode *new_node);

  std::unordered_map<int, ListNode *> map_;
  ListNode *head_, *tail_;
  int size_;
  int max_size_;
};
