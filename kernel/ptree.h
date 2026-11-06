#ifndef _PTREE_H_
#define _PTREE_H_

#include "types.h"

struct ptreeinfo {
    int pid;
    int ppid;
    int state;
    uint64 memsize;     // Số byte bộ nhớ user của tiến trình
    char name[16];
};

// Thêm khai báo nguyên mẫu ở đây để các file khác gọi được
int fill_ptree(struct ptreeinfo *buf, int max);

#endif