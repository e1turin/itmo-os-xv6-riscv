/// Read-Write lock

#include "rwlock.h"
#include "defs.h"

void
initrwlock(struct rwlock *lk, char *name)
{
  initlock(&lk->read, name);
  initlock(&lk->write, name);
  lk->reader_cnt = 0;
}

void
acquire_write(struct rwlock *lk)
{
  acquire(&lk->write);
}

void
release_write(struct rwlock *lk)
{
  release(&lk->write);
}

void
acquire_read(struct rwlock *lk)
{
  acquire(&lk->read);
  lk->reader_cnt++;
  if(lk->reader_cnt == 1) {
    acquire(&lk->write);
    pop_off(); // XXX: Is it necessary to desable interrupts?
  }
  release(&lk->read);
}

void
release_read(struct rwlock *lk)
{
  acquire(&lk->read);
  lk->reader_cnt--;
  if(lk->reader_cnt == 0) {
    push_off();
    release(&lk->write);
  }
  release(&lk->read);
}

