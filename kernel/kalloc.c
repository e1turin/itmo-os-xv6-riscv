// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"
#include "cow.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;

  struct pagerefctrl refs;
} kmem;

// char* to the first free non-kernel byte
#define KERNEL_END (&kmem.refs.counts[kmem.refs.size])

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  // char is enough to store #NPROC references
  uint64 size = sizeof(char) * (((char *)PHYSTOP - end) / PGSIZE + 1);
  char *begin_ref_count = end;
  pagerefinit(&kmem.refs, begin_ref_count, size);
  freerange(KERNEL_END, (void *)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < KERNEL_END || (uint64)pa >= PHYSTOP)
    panic("kfree");

  if(pagerefcnt(&kmem.refs, pa) != 0)
    return;

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r) {
    if(pagerefcnt(&kmem.refs, r) != 0)
      panic("kalloc: not null");

    kdup(r);
    memset((char*)r, 5, PGSIZE); // fill with junk
  }

  return (void*)r;
}

void *kdup(void *p) {
  if(p) {
    pageacquire(&kmem.refs, p);
  }
}
