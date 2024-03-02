#include "types.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "cow.h"

void pagerefinit(struct pagerefctrl *ctrl, char *begin, uint64 size) {
  ctrl->counts = begin;
  ctrl->size = size;

  initlock(&ctrl->lock, "pagereflock");
  for (uint64 i = 0; i < ctrl->size; ++i) {
    ctrl->counts[i] = 0;
  }
}

int pagerefcnt(struct pagerefctrl *ctrl, void *pa) {
  acquire(&ctrl->lock);
  uint64 pnum = (uint64)pa >> 12; // align by 4KB
  int cnt = ctrl->counts[pnum];
  release(&ctrl->lock);
  return cnt;
}

int pagerelease(struct pagerefctrl *ctrl, void *pa) {
  acquire(&ctrl->lock);
  uint64 pnum = (uint64)pa >> 12; // align by 4KB
  int cnt = ctrl->counts[pnum];
  if (cnt == 0)
    panic("pagerelease");

  ctrl->counts[pnum] -= 1;

  release(&ctrl->lock);
  return 0;
}

int pageacquire(struct pagerefctrl *ctrl, void *pa) {
  acquire(&ctrl->lock);
  uint64 pnum = (uint64)pa >> 12; // align by 4KB
  int cnt = ctrl->counts[pnum];
  if (cnt < 0 && cnt >= NPROC)
    panic("pageacquire");

  ctrl->counts[pnum] += 1;

  release(&ctrl->lock);
  return 0;
}

