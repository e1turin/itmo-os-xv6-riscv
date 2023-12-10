///
/// Synchronized double-linked circular list
///
#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"
#include "synclist.h"

/// Initialize empty syncronized double-linked circular list
void 
synclist_init(struct synclist *lst)
{
  lst->next = lst;
  lst->prev = lst;
  lst->ref_cnt = 2;
  initlock(&lst->lock, "synclist_lock");
}

/// Function pushes element to list
/// lst->lock must be acquired
void
synclist_push(struct synclist *lst, struct synclist *e)
{
  *e = (struct synclist){.ref_cnt = 3};
  initlock(&e->lock, "synclist_elem_lock");
  
  e->next = lst->next;
  e->prev = lst;
  lst->next->prev = e;
  lst->next = e;
}

/// Function for indicating new reference to list element
/// lst->lock must be acquired
void
synclist_acquire(struct synclist *e)
{
  acquire(&e->lock);
  e->ref_cnt++;
  release(&e->lock);
}

/// Function remove element from list but may be not free memory.
/// lst->lock must be acquired
void
synclist_remove(struct synclist *lst, struct synclist *e)
{
  // remove from list
  e->prev->next = e->next;
  e->next->prev = e->prev;

  // honestly count references to the element
  if (e->next != lst)
    synclist_acquire(e->next);
  if (e->prev != lst)
    synclist_acquire(e->prev);

  acquire(&e->lock);
  e->ref_cnt -= 2;
  
  // freed before ref_count++
  // CHECK(e->ref_cnt > 0);

  release(&e->lock);

  if (e->ref_cnt == 0) {
    bd_free(e);
  }
}


/// lst->lock must be acquired
bool
synclist_empty(struct synclist *lst)
{
  return lst->next == lst && lst->prev == lst;
}

/// lst->lock must be acquired
void
synclist_release(struct synclist *lst, struct synclist *e)
{ 
  // list root must not be acquired
  if (lst == e)
    return;

  acquire(&e->lock);
  e->ref_cnt--;

  // CHECK(e->ref_cnt >= 0);

  if (e->ref_cnt == 0) {
    synclist_release(lst, e->next);
    synclist_release(lst, e->prev);
    release(&e->lock);
    bd_free(e);
  } else {
    release(&e->lock);
  }
}

/// Returns pointer to next list element and acquires it if necessary.
/// lst->lock must be acquired
struct synclist*
synclist_next(struct synclist *lst, struct synclist *e)
{ 
  // empty list root or
  // list root neighbour,
  // avoid acquiring the list root
  if (e->next == lst)
    return e->next;

  // regular list element
  synclist_acquire(e->next);
  return e->next;
}

/// First element of list or list root if list is empty.
/// lst->lock must be acquired
struct synclist*
synclist_begin(struct synclist *lst)
{
  return synclist_next(lst, lst);
}

/// Moves element pointer over circular list elements.
/// lst->lock must be acquired
void
synclist_iter_next(struct synclist *lst, struct synclist **e)
{
  struct synclist *next = synclist_next(lst, *e);

  synclist_release(lst, *e);

  *e = next;
}

