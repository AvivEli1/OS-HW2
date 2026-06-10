#include "hw2_test.h"

long set_ban(int ban_getpid, int ban_pipe, int ban_kill, int ban_sig) {
    long r = syscall(334, ban_getpid, ban_pipe, ban_kill, ban_sig);
    return r;
}

long get_ban(char ban) {
    long r = syscall(335, ban);
    return r;
}

long check_ban(pid_t pid, char ban) {
	long r = syscall(336, pid, ban);
    return r;
}

long flip_ban_branch(int height, char ban) {
	long r = syscall(337, height, ban);
    return r;
}
