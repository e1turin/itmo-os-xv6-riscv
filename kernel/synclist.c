///
/// Synchronized double-linked circular list
///

#include "types.h"
#include "defs.h"
#include "synclist.h"
#include <stdbool.h>

/// Initialize empty syncronized double-linked circular list
void 
synclist_init(struct synclist *lst)
{
  lst->next = lst;
  lst->prev = lst;
  lst->ref_cnt = 2;
  initlock(&lst->lock, "synclist_lock");
}

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

/// useful function for indicating new reference to list element
void
synclist_acquire(struct synclist *e)
{
  acquire(&e->lock);
  e->ref_cnt++;
  release(&e->lock);
}

/// lst->lock must be acquired
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

  acquire(&e->lock); // XXX: save invarian
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


/// lst->lock must be acquired
bool
synclist_empty(struct synclist *lst) {
  return lst->next == lst && lst->prev == lst;
}

/// lst->lock must be acquired
void
synclist_release(struct synclist *lst, struct synclist *e)
{
  if (lst == e)
    return;

  acquire(&e->lock);
  e->ref_cnt--;

  //// Code out of synclist function
  // CHECK(e->ref_cnt >= 0);
  // 
  // if (e->ref_cnt == 0) {
  //   s_lst_release_element(lst, (struct sync_list*)e->lst.next);
  //   s_lst_release_element(lst, (struct sync_list*)e->lst.prev);
  //   pop_off();
  //   bd_free(e);
  // } else {
  //   release(&e->list_lock);
  // }
}

/// lst->lock must be acquired
struct synclist*
synclist_next(struct synclist *e)
{
  struct synclist *next = e->next;

  // avoid acquiring the list root
  if (next == e)
    return e;

  synclist_acquire(next);

  return next;
}

/// lst->lock must be acquired
void
synclist_move_next(struct synclist *lst, struct synclist **e)
{
  struct synclist *next;

  // XXX: synclist_release alreade checks if element is list root
  // Unnecessery check!
  //
  // avoid acquiring the list root
  if ((*e)->next == lst) {
    next = (*e)->next;
  } else {
    next = synclist_next(*e);
  }

  // and avoid necessary release
  if (*e != lst)
    synclist_release(lst, *e);

  *e = next;
}

