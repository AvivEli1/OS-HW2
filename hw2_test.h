#ifndef test_hw2_H_
#define test_hw2_H_

#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

// System call wrappers
long set_ban(int ban_getpid, int ban_pipe, int ban_kill, int ban_sig);
long get_ban(char ban);
long check_ban(pid_t pid, char ban);
long flip_ban_branch(int height, char ban);

#endif // test_hw2_H_
