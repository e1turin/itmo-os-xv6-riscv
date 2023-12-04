/// Read-Write lock

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"
#include "rwlock.h"

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
    lk->write.cpu = 0; // no certain cpu is holding this lock
    acquire(&lk->write);
    release(&lk->read);
    pop_off(); // It's hazardous to allow interrupts under the read lock.
               // Interrupts under the lock lead to deadlock.
               // Needs to allow interrupts after acquiring read lock.
  } else {
    release(&lk->read);
  }
}

void
release_read(struct rwlock *lk)
{
  acquire(&lk->read);
  lk->reader_cnt--;
  if(lk->reader_cnt == 0) {
    lk->write.cpu = mycpu(); // to avoid "panic: release"
    push_off(); // Disallow interrupts as if the normal lock had been captured
                // for write lock. Protects from allowing interrupts
                // until the read lock is released.
    release(&lk->write);
  }
  release(&lk->read);
}

