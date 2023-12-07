///
/// Synchronized double-linked circular list
///

#include "types.h"
#include "defs.h"
#include "synclist.h"

void 
synclist_init(struct synclist *lst)
{
  lst->next = lst;
  lst->prev = lst;
  lst->ref_cnt = 2;
  initlock(&lst->lock, "synclist_lock");
}

// lst->lock must be acquired
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

void
synclist_acquire(struct synclist *e)
{
  acquire(&e->lock);
  e->ref_cnt++;
  release(&e->lock);
}

// lst->lock must be acquired
void
synclist_remove(struct synclist *lst, struct synclist *e)
{
  // remove from list
  e->prev->next = e->next;
  e->next->prev = e->prev;

  if (e->next != lst)
    synclist_acquire(e->next);

  if (e->prev != lst)
    synclist_acquire(e->prev);

  acquire(&e->lock); // XXX: save invariant
  e->ref_cnt -= 2;
  
  //// Code out of list remove function
  // // freed before ref_count++
  // CHECK(e->ref_cnt > 0);
  // 
  // if (e->ref_cnt == 0) {
  //   pop_off();
  //   bd_free(e);
  // } else {
  //   release(&e->list_lock);
  // }
}

