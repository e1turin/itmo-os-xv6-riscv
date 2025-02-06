#include "kernel/types.h"
#include "user.h"

uint64 rvvtest(void);

int main() {
    printf("starting...");
    uint64 result = rvvtest();
    printf("result: %u", result);
    return 0;
}