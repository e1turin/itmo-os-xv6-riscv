///
/// Synchronized double-linked circular list
///

#ifndef KERNEL_SYNCLIST_H
#define KERNEL_SYNCLIST_H

#include "spinlock.h"

struct synclist {
    struct synclist *next;
    struct synclist *prev;
    uint ref_cnt;
    struct spinlock lock;
};

#endif // KERNEL_SYNCLIST_H
