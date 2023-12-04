///
/// Read-Write lock.
///
/// Only one thread can write to the resource 
/// and many threads can read from the resource.
/// No write can be performed while resource is read by any thread.
///

#ifndef KERNEL_RWLOCK_H
#define KERNEL_RWLOCK_H

struct rwlock {
  struct spinlock read;
  struct spinlock write;
  int reader_cnt;
};

void initrwlock(struct rwlock *lk, char *name);

void acquire_write(struct rwlock *lk);

void release_write(struct rwlock *lk);

void acquire_read(struct rwlock *lk);

void release_read(struct rwlock *lk);


#endif // KERNEL_RWLOCK_H
