/// Read-Write lock

struct rwlock {
  struct spinlock read;
  struct spinlock write;
  int reader_cnt;
};

