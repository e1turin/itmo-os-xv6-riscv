///
/// Synchronized double-linked circular list
///

#ifndef KERNEL_SYNCLIST_H
#define KERNEL_SYNCLIST_H

#include <stdbool.h>

struct synclist {
    struct synclist *next;
    struct synclist *prev;
    uint ref_cnt;
    struct spinlock lock;
};

void synclist_init(struct synclist *lst);

void synclist_push(struct synclist *lst, struct synclist *e);

/// useful function for indicating new reference to list element
/// lst->lock must be acquired
void synclist_acquire(struct synclist *e);

/// Function tries to remove element from list.
/// lst->lock must be acquired
void synclist_remove(struct synclist *lst, struct synclist *e);

/// lst->lock must be acquired
bool synclist_empty(struct synclist *lst);

/// lst->lock must be acquired
void synclist_release(struct synclist *lst, struct synclist *e);

/// lst->lock must be acquired
struct synclist* synclist_next(struct synclist *e);

/// lst->lock must be acquired
void synclist_iter_next(struct synclist *lst, struct synclist **e);

#endif // KERNEL_SYNCLIST_H
