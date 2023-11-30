/// Read-Write lock

void initlock(struct rwlock *lk) {
  initlock(&lk->read);
  initlock(&lk->write);
  lk->reader_cnt = 0;
}

void acquire_write(struct rwlock *lk) {
  acquire(&lk->write);
}

void release_write(struct rwlock *lk) {
  release(&lk->write);
}

void acquire_read(struct rwlock *lk) {
  acquire(&lk->read);
  lk->reader_cnt++;
  if(lk->reader_cnt == 1) {
    push_off();
  }
  release(&lk->read);
}

void release_write(struct rwlock *lk) {
  acquire(&lk->read);
  lk->reader_cnt--;
  if(lk->reader_cnt == 0) {
    pop_off();
  }
  release(&lk->read);
}
