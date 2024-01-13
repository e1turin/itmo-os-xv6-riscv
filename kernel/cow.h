
struct pagerefctrl {
  struct spinlock pagereflock;
  int *pprefcnt_begin;
  int *pprefcnt_end;
};

// Init empty pagereferencies for all available physical pages
void pagerefinit(struct pagerefctrl*);

// Returns number of current readers of the page starts from address pa.
int pagerefcnt(struct pagerefctrl*, uint64);

// Decriments number of current readers of the page starts from address pa
int pagerelease(struct pagerefctrl*, uint64);

// kmem.pagereflock must be held.
int pageacquire(struct pagerefctrl*, uint64);
