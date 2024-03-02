struct pagerefctrl {
  struct spinlock lock;
  char *counts; // NPROC defined as 64, so a byte is enough
  uint64 size;          // to count all available references.
};

// Init empty pagereferencies for all available physical pages
void pagerefinit(struct pagerefctrl *, char *, uint64);

// Returns number of current readers of the page starts from address pa.
int pagerefcnt(struct pagerefctrl *, void *);

// Decriments number of current readers of the page starts from address pa
int pagerelease(struct pagerefctrl *, void *);

// kmem.pagereflock must be held.
int pageacquire(struct pagerefctrl *, void *);
