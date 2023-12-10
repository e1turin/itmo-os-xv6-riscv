///
/// Synchronized double-linked circular list
///
/// With this implementation xv6 spends around 5-10 minutes in average
/// on usertest "forkforkfork". Such behavior appears because acquiring lock
/// in xv6 implementation entails desabling interrupts and in fact only the
/// global lock of list (list root lock) is used.
///

#ifndef KERNEL_SYNCLIST_H
#define KERNEL_SYNCLIST_H

#include <stdbool.h>

struct synclist {
    struct synclist *next;
    struct synclist *prev;
    uint ref_cnt;
    struct spinlock lock; // Unused in xv6 locking implementation
                          // as global list lock is always held.
};

/// Initialize empty syncronized double-linked circular list
void synclist_init(struct synclist *lst);

/// Function pushes element to list
void synclist_push(struct synclist *lst, struct synclist *e);

/// Function for indicating new reference to list element
/// lst->lock must be acquired
void synclist_acquire(struct synclist *e);

/// Function remove element from list but may be not free memory.
/// lst->lock must be acquired
void synclist_remove(struct synclist *lst, struct synclist *e);

/// lst->lock must be acquired
bool synclist_empty(struct synclist *lst);

/// Function deletes reference to element of list and may be free memory.
/// lst->lock must be acquired
void synclist_release(struct synclist *lst, struct synclist *e);

/// Returns pointer to next list element and acquires it if necessary.
/// lst->lock must be acquired
struct synclist* synclist_next(struct synclist *lst, struct synclist *e);

/// First element of list or list root if list is empty.
/// lst->lock must be acquired
struct synclist*
synclist_begin(struct synclist *lst);

/// Moves element pointer over circular list elements.
/// lst->lock must be acquired
void
synclist_iter_next(struct synclist *lst, struct synclist **e);

#endif // KERNEL_SYNCLIST_H

