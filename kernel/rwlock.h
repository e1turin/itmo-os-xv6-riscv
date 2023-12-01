/// Read-Write lock

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

