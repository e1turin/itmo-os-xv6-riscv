#include "types.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"
#include "cow.h"

void pagerefinit(struct pagerefctrl *c) {
  initlock(&c->pagereflock, "pagereflock");
  for (int *i = c->pprefcnt_begin; i < c->pprefcnt_end; ++i) {
    *i = 0;
  }
}

int pagerefcnt(struct pagerefctrl *c, uint64 pa) {
  acquire(&c->pagereflock);
  uint64 pnum = pa >> 12;
  uint cnt = c->pprefcnt_begin[pnum];
  release(&c->pagereflock);
  return cnt;
}

int pagerelease(struct pagerefctrl *c, uint64 pa) {
  acquire(&c->pagereflock);
  uint64 pnum = pa >> 12;

  uint cnt = c->pprefcnt_begin[pnum];
  if (cnt == 0)
    return -1;

  c->pprefcnt_begin[pnum]--;

  release(&c->pagereflock);
  return 0;
}

int pageacquire(struct pagerefctrl *c, uint64 pa) {
  acquire(&c->pagereflock);
  uint64 pnum = pa >> 12;

  uint cnt = c->pprefcnt_begin[pnum];
  if (cnt == (uint)-1)
    return -1;

  c->pprefcnt_begin[pnum]++;

  release(&c->pagereflock);
  return 0;
}

