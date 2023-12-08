///
/// Synchronized double-linked circular list
///

#include "types.h"
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
/// lst->lock must be acquired
void
synclist_acquire(struct synclist *e)
{
  acquire(&e->lock);
  e->ref_cnt++;
  release(&e->lock);
}

/// Function tries to remove element from list.
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

  acquire(&e->lock);
  e->ref_cnt -= 2;
  
  //// Code out of list remove function
  // // freed before ref_count++
  // CHECK(e->ref_cnt > 0);
  // 
  // if (e->ref_cnt == 0) {
  //   pop_off();               <---| do it out of list implementation
  //   bd_free(e);              <---| (+release(&e->list_lock))
  // } else {
  //   release(&e->list_lock);
  // }

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
  //   pop_off();           <---| do it out of list implementation
  //   bd_free(e);          <---| (+release(&e->list_lock))
  // } else {
  //   release(&e->list_lock);
  // }

  release(&e->lock);

  if (e->ref_cnt == 0) {
    synclist_release(lst, e->next);
    synclist_release(lst, e->prev);
    bd_free(e);
  }
}

/// lst->lock must be acquired
struct synclist*
synclist_next(struct synclist *e)
{
  // avoid acquiring the list root
  if (e->next == e)
    return e;

  synclist_acquire(e->next);

  return e->next;
}

/// lst->lock must be acquired
void
synclist_iter_next(struct synclist *lst, struct synclist **e)
{
  struct synclist *next = synclist_next(*e);

  synclist_release(lst, *e);

  *e = next;
}

