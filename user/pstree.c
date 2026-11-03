#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/ptree.h"

#define MAX_PROC 64

void print_tree(struct ptreeinfo *procs, int count, int parent_pid, int depth)
{
    for (int i = 0; i < count; i++) {
        if (procs[i].ppid == parent_pid) {
            for (int d = 0; d < depth; d++) {
                printf("  ");
            }
            printf("%d %s state=%d mem=%d\n", 
                   procs[i].pid, 
                   procs[i].name, 
                   procs[i].state, 
                   (int)procs[i].memsize);
            
            print_tree(procs, count, procs[i].pid, depth + 1);
        }
    }
}

int main(int argc, char *argv[])
{
    struct ptreeinfo procs[MAX_PROC];
    int count = ptree(procs, MAX_PROC);

    if (count < 0) {
        printf("pstree: ptree failed\n");
        exit(1);
    }

    print_tree(procs, count, 0, 0);
    exit(0);
}