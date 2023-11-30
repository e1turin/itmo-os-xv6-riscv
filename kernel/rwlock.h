/// Read-Write lock

struct rwlock {
  struct spinlock read;
  struct spinlock write;
  int64 reader_cnt;
};

void initlock(struct rwlock *lk);

void acquire_write(struct rwlock *lk);

void release_write(struct rwlock *lk);

void acquire_read(struct rwlock *lk);

void release_write(struct rwlock *lk);

