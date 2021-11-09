#include "Common.h"

class Replacer {
 public:
  Replacer() = default;
  virtual ~Replacer() = default;
  virtual frame_id_t Victim() = 0;
  virtual void Insert(frame_id_t frame_id) = 0;
  virtual void Remove(frame_id_t frame_id) = 0;
  virtual int Size() = 0;
};
